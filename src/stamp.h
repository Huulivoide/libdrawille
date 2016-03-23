#ifndef LIBDRAWILLE_STAMP_H
#define LIBDRAWILLE_STAMP_H

#include <stddef.h>

#include "mat3.h"
#include "point.h"
#include "polygon.h"
#include "Canvas.h"

enum StampType {
    POLYGON = 0,
    BITMAP
};
typedef enum StampType StampType;

struct Stamp {
    const StampType type;
    mat3* tr_matrix;
    Polygon* polygon;
    Canvas* bitmap;
};
typedef struct Stamp Stamp;

Stamp* new_polygon_stamp(Polygon* p);
Stamp* new_rectangle_stamp(const size_t width, const size_t height);
Stamp* new_circle_stamp(const size_t steps, const size_t radius);

void free_stamp(Stamp* s);

Point get_stamp_center(const Stamp* stamp);

void apply_matrix(Stamp* s);

int draw_stamp_outline(Canvas* c, Color color, const Stamp* s);
int fill_shape(Canvas* c, Color color, const Stamp* s);

#endif //LIBDRAWILLE_STAMP_H
