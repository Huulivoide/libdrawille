#ifndef LIBDRAWILLE_POLYGON_H
#define LIBDRAWILLE_POLYGON_H

#include <stdbool.h>

#include "mat3.h"
#include "point.h"

struct Polygon {
    size_t size;
    size_t next;
    Point* vertices;
};
typedef struct Polygon Polygon;

Polygon* new_polygon();
void free_polygon(Polygon* p);

bool optimize_polygon_memory(Polygon* polygon);

size_t add_vertex(Polygon* p, const Point vertex);
void close_polygon(Polygon *polygon);

void transform_polygon(Polygon* p, const mat3* transformations);

Point get_polygon_center(const Polygon* polygon);

#endif //LIBDRAWILLE_POLYGON_H
