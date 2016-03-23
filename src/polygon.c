#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "polygon.h"

Polygon* new_polygon() {
    Polygon* p = malloc(sizeof(Polygon));

    if (p == NULL) {
        return NULL;
    }

    p->size = 20;
    p->last = 0;

    p->vertices = malloc(sizeof(Point) * 20);

    if (p->vertices == NULL) {
        free(p);
        return NULL;
    }

    return p;
}

void free_polygon(Polygon* p) {
    free(p->vertices);
    free(p);
}

size_t add_vertex(Polygon* p, const Point vertex) {
    if (p->last == p->size) {
        p->size *= 2;
        Point* new = realloc(p->vertices, p->size);
        if (new == NULL) {
            p->size /= 2;
            return SIZE_MAX;
        }
        p->vertices = new;
    }

    p->vertices[p->last] = vertex;
    return p->last++;
}

void transform_polygon(Polygon* p, const mat3* transformations) {
    for (size_t i = 0; i < p->last; i++) {
        p->vertices[i] = transform_point(p->vertices[i], transformations);
    }
}

Point get_polygon_center(const Polygon* polygon) {
    float minx = 0;
    float miny = 0;
    float maxx = 0;
    float maxy = 0;

    for (size_t i = 0; i < polygon->last; i++) {
        minx = fminf(minx, polygon->vertices[i].x);
        miny = fminf(miny, polygon->vertices[i].y);
        maxx = fmaxf(maxx, polygon->vertices[i].x);
        maxy = fmaxf(maxy, polygon->vertices[i].y);
    }

    return (Point) {fabsf(minx - maxx) / 2, fabsf(miny - maxy) / 2};
}
