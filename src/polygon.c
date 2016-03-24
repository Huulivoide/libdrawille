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
    p->next = 0;

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

bool optimize_polygon_memory(Polygon* polygon) {
    if (polygon->next < polygon->size) {
        long smaller = polygon->size - polygon->next;
        polygon->size -= smaller;
        Point* new = realloc(polygon->vertices, sizeof(Point) * polygon->size);
        if (new == NULL) {
            polygon->size += smaller;
            return false;
        }
        polygon->vertices = new;
    }

    return true;
}

size_t add_vertex(Polygon* p, const Point vertex) {
    if (p->next == p->size) {
        p->size *= 2;
        Point* new = realloc(p->vertices, sizeof(Point) * p->size);
        if (new == NULL) {
            p->size /= 2;
            return SIZE_MAX;
        }
        p->vertices = new;
    }

    p->vertices[p->next] = vertex;
    return p->next++;
}

void close_polygon(Polygon *polygon) {
    polygon->vertices[polygon->next] = polygon->vertices[0];
    polygon->next++;
    optimize_polygon_memory(polygon);
}

void transform_polygon(Polygon* p, const mat3* transformations) {
    for (size_t i = 0; i < p->next; i++) {
        p->vertices[i] = transform_point(p->vertices[i], transformations);
    }
}

Point get_polygon_center(const Polygon* polygon) {
    float minx = 0;
    float miny = 0;
    float maxx = 0;
    float maxy = 0;

    for (size_t i = 0; i < polygon->next; i++) {
        minx = fminf(minx, polygon->vertices[i].x);
        miny = fminf(miny, polygon->vertices[i].y);
        maxx = fmaxf(maxx, polygon->vertices[i].x);
        maxy = fmaxf(maxy, polygon->vertices[i].y);
    }

    return (Point) {fabsf(minx - maxx) / 2, fabsf(miny - maxy) / 2};
}
