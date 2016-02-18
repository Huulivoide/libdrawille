#ifndef LIBDRAWILLE_POINT_H
#define LIBDRAWILLE_POINT_H

#include "mat3.h"

struct Point {
    float x;
    float y;
};

typedef struct Point Point;

Point transform_point(Point p, mat3 m);

#endif //LIBDRAWILLE_POINT_H
