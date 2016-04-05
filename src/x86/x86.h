#ifndef LIBDRAWILLE_X86_H
#define LIBDRAWILLE_X86_H

#include "../Canvas.h"
#include "../point.h"

void fill_triangle_sse4(Canvas* restrict canvas, const Color color,
                        const Point v1, const Point v2, const Point v3);

#endif //LIBDRAWILLE_X86_H
