#include <stdint.h>
#include <stdlib.h>

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
        Point* new = realloc(p->vertices, p->size * 2);
        if (new == NULL) {
            return SIZE_MAX;
        }
        p->vertices = new;
    }

    p->vertices[p->last] = vertex;
    return p->last++;
}

void transform_polygon(Polygon* p, mat3 transformations) {
    for (size_t i = 0; i < p->last; i++) {
        p->vertices[i] = transform_point(p->vertices[i], transformations);
    }
}
