#ifndef LIBDRAWILLE_POLYGON_H
#define LIBDRAWILLE_POLYGON_H

#include "mat3.h"
#include "point.h"

struct Polygon {
    size_t size;
    size_t last;
    Point* vertices;
};
typedef struct Polygon Polygon;

Polygon* new_polygon();
void free_polygon(Polygon* p);

size_t add_vertex(Polygon* p, const Point vertex);

void transform_polygon(Polygon* p, mat3 transformations);

#endif //LIBDRAWILLE_POLYGON_H
