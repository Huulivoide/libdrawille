#if defined(__i386__) || defined(__x86_64__)

#include <x86intrin.h>

#include "x86.h"
#include "../utils.h"

static inline __m128i to_fixed_sse(float a, float b, float c) {
    __m128 m = _mm_mul_ps(_mm_set_ps(0, c, b, a), _mm_set_ps1(16.0f));
    return _mm_cvtps_epi32(m);
}

void fill_triangle_sse4(Canvas* restrict canvas, const Point v1, const Point v2,
                       const Point v3, void(*set_pixel)(Canvas*, const int, const int)) {
    // Block size, standard 8x8 (must be power of two)
    const int q = 8;

    // 28.4 fixed-point coordinates
    __m128i Xs = to_fixed_sse(v1.x, v2.x, v3.x);
    __m128i Ys = to_fixed_sse(v1.y, v2.y, v3.y);

    // Bounding rectangle. Also clip to canvas size
    int xarr[4] __attribute__((aligned (16)));
    int yarr[4] __attribute__((aligned (16)));

    __m128i Xtemp = _mm_add_epi32(Xs, _mm_set1_epi32(0xF));
    _mm_store_si128((__m128i*)xarr, _mm_srli_epi32(Xtemp, 4));

    __m128i Ytemp = _mm_add_epi32(Ys, _mm_set1_epi32(0xF));
    _mm_store_si128((__m128i*)yarr, _mm_srli_epi32(Ytemp, 4));

    int minx, miny, maxx, maxy;
    get_clamped_min_max(xarr[0], xarr[1], xarr[2], 0, to_fixed(canvas->width),
                        &minx, &maxx);
    get_clamped_min_max(yarr[0], yarr[1], yarr[2], 0, to_fixed(canvas->height),
                        &miny, &maxy);

    // Start in corner of 8x8 block
    minx &= ~(q - 1);
    miny &= ~(q - 1);

    // Deltas
    __m128i dx_shuffled = _mm_shuffle_epi32(Xs, _MM_SHUFFLE(3, 0, 2, 1));
    __m128i DXs = _mm_sub_epi32(Xs, dx_shuffled);

    __m128i dy_shuffled = _mm_shuffle_epi32(Ys, _MM_SHUFFLE(3, 0, 2, 1));
    __m128i DYs = _mm_sub_epi32(Ys, dy_shuffled);

    // Half-edge constants
    __m128i Cs = _mm_sub_epi32(_mm_mullo_epi32(DYs, Xs), _mm_mullo_epi32(DXs, Ys));

    // Correct for fill convention
    // if(DYs < 0 || (DYs == 0 && DXs > 0)) Cs++;
    __m128i zeros = _mm_setzero_si128();
    __m128i eq = _mm_cmpeq_epi32(DYs, zeros); // Right side of OR
    __m128i gt = _mm_cmpgt_epi32(DXs, zeros); // Right side of OR
    __m128i rs = _mm_and_si128(eq, gt);       // Right side of OR
    __m128i lf = _mm_cmplt_epi32(DYs, zeros); // Left side of OR

    __m128i mask = _mm_and_si128(_mm_or_si128(lf, rs), _mm_set1_epi32(1));
    Cs = _mm_add_epi32(Cs, mask);

    // Fixed-point deltas
    __m128i FDXs = _mm_slli_epi32(DXs, 4);
    __m128i FDYs = _mm_slli_epi32(DYs, 4);

    // Loop through blocks
    for(int y = miny; y < maxy; y += q) {
        for(int x = minx; x < maxx; x += q) {
            // Corners of block
            int x0 = x << 4;
            int x1 = (x + q - 1) << 4;
            int y0 = y << 4;
            int y1 = (y + q - 1) << 4;

            // Evaluate half-space functions
            __m128i left_side = _mm_mullo_epi32(DXs, _mm_set1_epi32(y0));
            __m128i right_side = _mm_mullo_epi32(DYs, _mm_set1_epi32(x0));
            __m128i m00 = _mm_sub_epi32(left_side, right_side);
            m00 = _mm_cmpgt_epi32(_mm_add_epi32(m00, Cs), _mm_setzero_si128());
            m00 = _mm_and_si128(m00, _mm_set1_epi32(1));

            right_side = _mm_mullo_epi32(DYs, _mm_set1_epi32(x1));
            __m128i m10 = _mm_sub_epi32(left_side, right_side);
            m10 = _mm_cmpgt_epi32(_mm_add_epi32(m10, Cs), _mm_setzero_si128());
            m10 = _mm_and_si128(m10, _mm_set1_epi32(2));

            left_side = _mm_mullo_epi32(DXs, _mm_set1_epi32(y1));
            __m128i m11 = _mm_sub_epi32(left_side, right_side);
            m11 = _mm_cmpgt_epi32(_mm_add_epi32(m11, Cs), _mm_setzero_si128());
            m11 = _mm_and_si128(m11, _mm_set1_epi32(8));

            right_side = _mm_mullo_epi32(DYs, _mm_set1_epi32(x0));
            __m128i m01 = _mm_sub_epi32(left_side, right_side);
            m01 = _mm_cmpgt_epi32(_mm_add_epi32(m01, Cs), _mm_setzero_si128());
            m01 = _mm_and_si128(m01, _mm_set1_epi32(4));

            __m128i hsmask = _mm_or_si128(m01, m11);
            hsmask = _mm_or_si128(hsmask, m10);
            hsmask = _mm_or_si128(hsmask, m00);

            int mask_arr[4] __attribute__((aligned(16)));
            _mm_store_si128((__m128i*)mask_arr, hsmask);

            // Skip block when outside an edge
            if(mask_arr[0] == 0 || mask_arr[1] == 0 || mask_arr[2] == 0) continue;

            // Accept whole block when totally covered
            if(mask_arr[0] == 0xF && mask_arr[1] == 0xF && mask_arr[2] == 0xF) {
                for(int iy = y; iy < y + q; iy++) {
                    for(int ix = x; ix < x + q; ix++) {
                        set_pixel(canvas, ix, iy);
                    }
                }
            } else { // Partially covered block
                __m128i left = _mm_mullo_epi32(DXs, _mm_set1_epi32(y0));
                __m128i right = _mm_mullo_epi32(DYs, _mm_set1_epi32(x0));
                __m128i CYs = _mm_add_epi32(Cs, _mm_sub_epi32(left, right));

                for(int iy = y; iy < y + q; iy++) {
                    int vals[4] __attribute__((aligned(16)));
                    __m128i CXs = CYs;

                    for(int ix = x; ix < x + q; ix++) {
                        _mm_store_si128((__m128i*)vals, CXs);
                        if(vals[0] > 0 && vals[1] > 0 && vals[2] > 0) {
                            set_pixel(canvas, ix, iy);
                        }

                        CXs = _mm_sub_epi32(CXs, FDYs);
                    }
                    CYs = _mm_add_epi32(CYs, FDXs);
                }
            }
        }
    }
}

#endif
