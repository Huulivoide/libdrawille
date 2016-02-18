#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "mat3.h"

const mat3 I = {{
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}
}};

mat3 translate_mat3(const mat3 m, const float x_delta, const float y_delta) {
    mat3 translated = m;
    translated.m[0][2] = (m.m[0][0] * x_delta) + (m.m[0][1] * y_delta) + m.m[0][2];
    translated.m[1][2] = (m.m[1][0] * x_delta) + (m.m[1][1] * y_delta) + m.m[1][2];

    return translated;
}

mat3 scale_mat3(const mat3 m, const float x_delta, const float y_delta) {
    mat3 scaled = m;
    scaled.m[0][0] *= x_delta;
    scaled.m[0][1] *= y_delta;
    scaled.m[1][0] *= x_delta;
    scaled.m[1][1] *= y_delta;

    return scaled;
}

mat3 rotate_mat3(const mat3 m, const float angle) {
    float sina = sinf(angle);
    float cosa = cosf(angle);

    mat3 rotated = m;

    rotated.m[0][0] = m.m[0][0] * cosa - m.m[0][1] * sina;
    rotated.m[0][1] = m.m[0][0] * sina + m.m[0][1] * cosa;
    rotated.m[1][0] = m.m[1][0] * cosa - m.m[1][1] * sina;
    rotated.m[1][1] = m.m[1][0] * sina + m.m[1][1] * cosa;

    return rotated;
}

mat3 shear_mat3(const mat3 m, const float x_delta, const float y_delta) {
    mat3 sheared = m;

    sheared.m[0][0] += m.m[0][1] * y_delta;
    sheared.m[0][1] += m.m[0][0] * x_delta;
    sheared.m[1][0] += m.m[1][1] * y_delta;
    sheared.m[1][1] += m.m[1][0] * x_delta;

    return sheared;
}

mat3 combine_mat3(const mat3 a, const mat3 b) {
    mat3 result = I;

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 3; j++) {
            result.m[i][j] = a.m[i][0] * b.m[0][j]
                           + a.m[i][1] * b.m[1][j]
                           + a.m[i][2] * b.m[2][j];
        }
    }

    return result;
}

char** to_string_mat3(const mat3 m) {
    char** strs = malloc(sizeof(char*) * 5);

    /* 4 line drawing chars (3bytes) + (3 numbers ±xx.y) */
    for (size_t i = 0; i < 5; i++) {
        strs[i] = malloc(30);

        if (i == 0) {
            sprintf(strs[i], "┌─                 ─┐");
        } else if (i != 4) {
            sprintf(strs[i], "│ %5.1f %5.1f %5.1f │",
                    m.m[i - 1][0], m.m[i - 1][1], m.m[i - 1][2]);
        } else {
            sprintf(strs[i], "└─                 ─┘");
        }
    }

    return strs;
}

bool is_identity_matrix(const mat3 m) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (I.m[i][j] != m.m[i][j]) {
                return false;
            }
        };
    }

    return true;
}
