#if defined(__i386__) || defined(__x86_64__)

#include <string.h>

#include "x86.h"
#include "../utils.h"

static inline __m256i _mm256_cmplt_epi32(__m256i __a, __m256i __b)
{
  return (__m256i)((__v8si)__a < (__v8si)__b);
}

static inline __m256i to_fixed_avx2(const float a, const float b, const float c,
                                    const float d, const float e, const float f) {
    const __m256 m = _mm256_mul_ps(_mm256_set_ps(0, f, e, d, 0, c, b, a),
                                   _mm256_set1_ps(16.0f));
    return _mm256_cvtps_epi32(m);
}

void fill_triangle_avx2(Canvas* restrict canvas, const Color color,
                        const Point v1, const Point v2, const Point v3) {
    // Block size, standard 8x8 (must be power of two)
    const int q = 8;

    // 28.4 fixed-point coordinates
    const __m256i FPCs = to_fixed_avx2(v1.x, v2.x, v3.x, v1.y, v2.y, v3.y);

    // Bounding rectangle. Also clip to canvas size
    const vec8 integers =
            {.v = _mm256_srli_epi32(_mm256_add_epi32(FPCs, _mm256_set1_epi32(0xF)), 4)};

    int minx, miny, maxx, maxy;
    get_clamped_min_max(integers.i[0], integers.i[1], integers.i[2], 0, to_fixed(canvas->width),
                        &minx, &maxx);
    get_clamped_min_max(integers.i[4], integers.i[5], integers.i[6], 0, to_fixed(canvas->height),
                        &miny, &maxy);

    // Start in corner of 8x8 block
    minx &= ~(q - 1);
    miny &= ~(q - 1);

    // Deltas
    const __m256i d_shuffled = _mm256_shuffle_epi32(FPCs, _MM_SHUFFLE(3, 0, 2, 1));
    const __m256i Ds = _mm256_sub_epi32(FPCs, d_shuffled);

    // Half-edge constants
    const __m256i switched = _mm256_permute4x64_epi64(Ds, _MM_SHUFFLE(1, 0, 3, 2));
    const __m256i multiplied = _mm256_mullo_epi32(FPCs, switched);
    const __m256i DXs_mullo_Ys = _mm256_permute4x64_epi64(multiplied, _MM_SHUFFLE(1, 0, 3, 2));
    __m256i Cs = _mm256_sub_epi32(multiplied, DXs_mullo_Ys);

    // Correct for fill convention
    // if(DYs < 0 || (DYs == 0 && DXs > 0)) Cs++;
    const __m256i zeros = _mm256_setzero_si256();
    const __m256i eq = _mm256_cmpeq_epi32(switched, zeros); // Right side of OR
    const __m256i gt = _mm256_cmpgt_epi32(Ds, zeros); // Right side of OR
    const __m256i rs = _mm256_and_si256(eq, gt);       // Right side of OR
    const __m256i lf = _mm256_cmplt_epi32(switched, zeros); // Left side of OR
    Cs = _mm256_add_epi32(Cs, _mm256_and_si256(_mm256_or_si256(lf, rs), _mm256_set1_epi32(1)));

    // Fixed-point deltas
    const __m256i FDXs = _mm256_slli_epi32(Ds, 4);
    const __m256i FDYs = _mm256_permute4x64_epi64(FDXs, _MM_SHUFFLE(1, 0, 3, 2));

    __m256i EOs = _mm256_sub_epi32(zeros, _mm256_and_si256(FDYs, _mm256_cmplt_epi32(FDYs, zeros)));
    EOs = _mm256_add_epi32(EOs, _mm256_and_si256(FDXs, _mm256_cmpgt_epi32(FDXs, zeros)));
    EOs = _mm256_slli_epi32(EOs, 3);

    __m256i EIs = _mm256_slli_epi32(_mm256_sub_epi32(Ds, switched), 7);
    EIs = _mm256_sub_epi32(EIs, EOs);
    __m256i Es = _mm256_permute2f128_si256(EOs, EIs, _MM_SHUFFLE(0, 2, 0, 0));

    // Loop through blocks
    for(int y = miny; y < maxy; y += q) {
        // Have we already filled an block?
        bool filled = false;

        for(int x = minx; x < maxx; x += q) {
            unsigned char* canvas_pos = canvas->canvas+(y*canvas->width + x);
            // Corners of block
            int x0 = x << 4;
            int y0 = y << 4;

            const __m256i low = _mm256_mullo_epi32(Ds, _mm256_set_epi32(x0, x0, x0, x0,
                                                                        y0, y0, y0, y0));
            const __m256i high = _mm256_permute4x64_epi64(low, _MM_SHUFFLE(1, 0, 3, 2));
            const __m256i __CXs = _mm256_permute4x64_epi64(
                    _mm256_add_epi32(Cs, _mm256_sub_epi32(low, high)),
                    _MM_SHUFFLE(1, 0, 1, 0));

            const vec8 CXs_plus_Es = {.v = _mm256_add_epi32(__CXs, Es)};
            // Skip block when outside an edge
            if((CXs_plus_Es.i[0] | CXs_plus_Es.i[1] | CXs_plus_Es.i[2]) < 0) {
                if (filled) {
                    // We have hit a filled block before, but now we've hit an
                    // empty one. we can skip to the next line.
                    break;
                } else {
                    // We have NOT hit a filled block before,
                    // so we just skip to the next one.
                    continue;
                }
            }

            // Accept whole block when totally covered
            if(CXs_plus_Es.i[4] > 0 && CXs_plus_Es.i[5] > 0 && CXs_plus_Es.i[6] > 0) {
                for(int iy = 0; iy < q; iy++) {
                    memset(canvas_pos, color, q);
                    canvas_pos += canvas->width;
                }
            } else { // Partially covered block
                __m256i CYs = __CXs;

                for(int iy = 0; iy < q; iy++) {
                    vec8 CXs = {.v = CYs};

                    for(int ix = 0; ix < q; ix++) {
                        if(CXs.i[0] > 0 && CXs.i[1] > 0 && CXs.i[2] > 0) {
                            canvas_pos[ix] = color;
                        }

                        CXs.v = _mm256_sub_epi32(CXs.v, FDYs);
                    }
                    CYs = _mm256_add_epi32(CYs, FDXs);
                    canvas_pos += canvas->width;
                }
            }
            filled = true;
        }
    }
}

#endif
