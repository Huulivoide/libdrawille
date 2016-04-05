#ifndef LIBDRAWILLE_UTILS_H
#define LIBDRAWILLE_UTILS_H

#include <math.h>

#include "Canvas.h"
#include "point.h"

extern void(*fill_triangle_implementation)(Canvas*, const Point, const Point,
               const Point, void(*set_pixel)(Canvas*, const int, const int));

void init_library();

bool get_console_size(size_t *width, size_t* height);

void get_clamped_min_max(int a, int b, int c, const int clamp_min,
                         const int clamp_max, int *min, int *max);

static inline int to_fixed(const float f) {
    return (int) roundf(16.0f * f);
}

#endif //LIBDRAWILLE_UTILS_H
