#ifndef LIBDRAWILLE_X86_H
#define LIBDRAWILLE_X86_H

#include <x86intrin.h>
#include <stdio.h>

#include "../Canvas.h"
#include "../point.h"

typedef union vec4 {
    __m128i v;
    int i[4];
} vec4;

typedef union vec8 {
    __m256i v;
    int i[8];
} vec8;

static inline void printv4(const char* prefix, vec4 v) {
    fprintf(stderr, "%s(%d, %d, %d, %d)\n", prefix, v.i[0], v.i[1], v.i[2], v.i[3]);
}

static inline void printv8(const char* prefix, vec8 v) {
    fprintf(stderr, "%s(%d, %d, %d, %d, %d, %d, %d, %d)\n", prefix,
           v.i[0], v.i[1], v.i[2], v.i[3],
           v.i[4], v.i[5], v.i[6], v.i[7]);
}

static inline void printv8l(const char* prefix, vec8 v) {
    fprintf(stderr, "%s(%d, %d, %d, %d)\n", prefix, v.i[0], v.i[1], v.i[2], v.i[3]);
}

static inline void printv8h(const char* prefix, vec8 v) {
    fprintf(stderr, "%s(%d, %d, %d, %d)\n", prefix, v.i[4], v.i[5], v.i[6], v.i[7]);
}


void fill_triangle_sse4(Canvas* restrict canvas, const Color color,
                        const Point v1, const Point v2, const Point v3);

void fill_triangle_avx2(Canvas* restrict canvas, const Color color,
                        const Point v1, const Point v2, const Point v3);

#endif //LIBDRAWILLE_X86_H
