#include "point.h"

Point transform_point(Point p, mat3 m) {
    return (Point) {
            m.m[0][0] * p.x + m.m[0][1] * p.y + m.m[0][2],
            m.m[1][0] * p.x + m.m[1][1] * p.y + m.m[1][2]
    };
}
