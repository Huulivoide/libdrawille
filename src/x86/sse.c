#if defined(__i386__) || defined(__x86_64__)

#include <x86intrin.h>
#include <string.h>

#include "x86.h"
#include "../utils.h"

typedef union vec4 {
    __m128i v;
    int i[4];
} vec4;

static inline __m128i to_fixed_sse(const float a, const float b, const float c) {
    const __m128 m = _mm_mul_ps(_mm_set_ps(0, c, b, a), _mm_set_ps1(16.0f));
    return _mm_cvtps_epi32(m);
}

void fill_triangle_sse4(Canvas* restrict canvas, const Color color,
                        const Point v1, const Point v2, const Point v3) {
    // Block size, standard 8x8 (must be power of two)
    const int q = 8;

    // 28.4 fixed-point coordinates
    const __m128i Xs = to_fixed_sse(v1.x, v2.x, v3.x);
    const __m128i Ys = to_fixed_sse(v1.y, v2.y, v3.y);

    // Bounding rectangle. Also clip to canvas size
    const vec4 Xtemp = {.v = _mm_srli_epi32(_mm_add_epi32(Xs, _mm_set1_epi32(0xF)), 4)};
    const vec4 Ytemp = {.v = _mm_srli_epi32(_mm_add_epi32(Ys, _mm_set1_epi32(0xF)), 4)};

    int minx, miny, maxx, maxy;
    get_clamped_min_max(Xtemp.i[0], Xtemp.i[1], Xtemp.i[2], 0, to_fixed(canvas->width),
                        &minx, &maxx);
    get_clamped_min_max(Ytemp.i[0], Ytemp.i[1], Ytemp.i[2], 0, to_fixed(canvas->height),
                        &miny, &maxy);

    // Start in corner of 8x8 block
    minx &= ~(q - 1);
    miny &= ~(q - 1);

    // Deltas
    const __m128i dx_shuffled = _mm_shuffle_epi32(Xs, _MM_SHUFFLE(3, 0, 2, 1));
    const __m128i DXs = _mm_sub_epi32(Xs, dx_shuffled);

    const __m128i dy_shuffled = _mm_shuffle_epi32(Ys, _MM_SHUFFLE(3, 0, 2, 1));
    const __m128i DYs = _mm_sub_epi32(Ys, dy_shuffled);

    // Half-edge constants
    __m128i Cs = _mm_sub_epi32(_mm_mullo_epi32(DYs, Xs), _mm_mullo_epi32(DXs, Ys));

    // Correct for fill convention
    // if(DYs < 0 || (DYs == 0 && DXs > 0)) Cs++;
    const __m128i zeros = _mm_setzero_si128();
    const __m128i eq = _mm_cmpeq_epi32(DYs, zeros); // Right side of OR
    const __m128i gt = _mm_cmpgt_epi32(DXs, zeros); // Right side of OR
    const __m128i rs = _mm_and_si128(eq, gt);       // Right side of OR
    const __m128i lf = _mm_cmplt_epi32(DYs, zeros); // Left side of OR

    Cs = _mm_add_epi32(Cs, _mm_and_si128(_mm_or_si128(lf, rs), _mm_set1_epi32(1)));

    // Fixed-point deltas
    const __m128i FDXs = _mm_slli_epi32(DXs, 4);
    const __m128i FDYs = _mm_slli_epi32(DYs, 4);

    __m128i EOs = _mm_sub_epi32(zeros, _mm_and_si128(FDYs, _mm_cmplt_epi32(FDYs, zeros)));
    EOs = _mm_add_epi32(EOs, _mm_and_si128(FDXs, _mm_cmpgt_epi32(FDXs, zeros)));
    EOs = _mm_slli_epi32(EOs, 3);

    __m128i EIs = _mm_slli_epi32(_mm_sub_epi32(DXs, DYs), 7);
    EIs = _mm_sub_epi32(EIs, EOs);

    // Loop through blocks
    for(int y = miny; y < maxy; y += q) {
        // Have we already filled an block?
        unsigned char filled = 0;

        for(int x = minx; x < maxx; x += q) {
            unsigned char* canvas_pos = canvas->canvas+(y*canvas->width + x);
            // Corners of block
            int x0 = x << 4;
            int y0 = y << 4;

            const __m128i left = _mm_mullo_epi32(DXs, _mm_set1_epi32(y0));
            const __m128i right = _mm_mullo_epi32(DYs, _mm_set1_epi32(x0));
            vec4 CXs = {.v = _mm_add_epi32(Cs, _mm_sub_epi32(left, right))};

            const vec4 CXs_add_EOs = {.v = _mm_add_epi32(CXs.v, EOs)};
            // Skip block when outside an edge
            if((CXs_add_EOs.i[0] | CXs_add_EOs.i[1] | CXs_add_EOs.i[2]) < 0) {
                if (filled & 0x80) {
                    // We have hit a filled block before, but now we've hit an
                    // empty one. we can skip to the next line.
                    break;
                } else {
                    // We have NOT hit a filled block before,
                    // so we just skip to the next one.
                    continue;
                }
            }

            const vec4 CXs_add_EIs = {.v = _mm_add_epi32(CXs.v, EIs)};
            // Accept whole block when totally covered
            if(CXs_add_EIs.i[0] > 0 && CXs_add_EIs.i[1] > 0 && CXs_add_EIs.i[2] > 0) {
                for(int iy = 0; iy < q; iy++) {
                    memset(canvas_pos, color, q);
                    canvas_pos += canvas->width;
                }
            } else { // Partially covered block
                __m128i CYs = CXs.v;

                for(int iy = 0; iy < q; iy++) {
                    CXs.v = CYs;
                    
                    bool done = false;
                    for(int ix = 0; ix < q; ix++) {
                        if(CXs.i[0] > 0 && CXs.i[1] > 0 && CXs.i[2] > 0) {
                            canvas_pos[ix] = color;
                            done = true;
                        } else if (done) {
                            break;
                        }

                        CXs.v = _mm_sub_epi32(CXs.v, FDYs);
                    }
                    CYs = _mm_add_epi32(CYs, FDXs);
                    canvas_pos += canvas->width;
                }
            }
            filled = 0x80;
        }
    }
}

#endif
