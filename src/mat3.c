#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mat3.h"

mat3* new_mat3() {
    mat3* m = calloc(1, sizeof(mat3));

    if (m == NULL) {
        return NULL;
    }

    m->m[0][0] = 1.0;
    m->m[1][1] = 1.0;
    m->m[2][2] = 1.0;

    return m;
}

void free_mat3(mat3* m) {
    free(m);
}

mat3* clone_mat3(const mat3* m) {
    mat3* clone = malloc(sizeof(mat3));

    if (clone == NULL) {
        return NULL;
    }

    memcpy(clone, m, sizeof(mat3));
    return clone;
}

void reset_mat3(mat3* m) {
    memset(m->m, 0, sizeof(m->m));

    m->m[0][0] = 1.0;
    m->m[1][1] = 1.0;
    m->m[2][2] = 1.0;
}

void translate_mat3(mat3* m, const float x_delta, const float y_delta) {
    m->m[0][2] = (m->m[0][0] * x_delta) + (m->m[0][1] * y_delta) + m->m[0][2];
    m->m[1][2] = (m->m[1][0] * x_delta) + (m->m[1][1] * y_delta) + m->m[1][2];
}

void scale_mat3(mat3* m, const float x_delta, const float y_delta) {
    m->m[0][0] *= x_delta;
    m->m[0][1] *= y_delta;
    m->m[1][0] *= x_delta;
    m->m[1][1] *= y_delta;
}

void rotate_mat3(mat3* m, const float angle) {
    float sina = sinf(angle);
    float cosa = cosf(angle);

    float old = m->m[0][0];
    m->m[0][0] = old * cosa - m->m[0][1] * sina;
    m->m[0][1] = old * sina + m->m[0][1] * cosa;
    
    old = m->m[1][0];
    m->m[1][0] = old * cosa - m->m[1][1] * sina;
    m->m[1][1] = old * sina + m->m[1][1] * cosa;
}

void shear_mat3(mat3* m, const float x_delta, const float y_delta) {
    float old = m->m[0][0];
    m->m[0][0] += m->m[0][1] * y_delta;
    m->m[0][1] += old * x_delta;
    
    old = m->m[1][0];
    m->m[1][0] += m->m[1][1] * y_delta;
    m->m[1][1] += old * x_delta;
}

void combine_a_is_to(mat3* a, const mat3* b) {
    for (size_t i = 0; i < 2; i++) {
        float row[3] = {a->m[i][0], a->m[i][1], a->m[i][2]};
        for (size_t j = 0; j < 3; j++) {
            a->m[i][j] = row[0] * b->m[0][j]
                       + row[1] * b->m[1][j]
                       + row[2] * b->m[2][j];
        }
    }
}

void combine_b_is_to(const mat3* a, mat3* b) {
    for (size_t j = 0; j < 3; j++) {
        float column[3] = {b->m[0][j], b->m[1][j], b->m[2][j]};
        for (size_t i = 0; i < 2; i++) {
            b->m[i][j] = a->m[i][0] * column[0]
                       + a->m[i][1] * column[1]
                       + a->m[i][2] * column[2];
        }
    }
}

void combine_mat3(const mat3* a, const mat3* b, mat3* to) {
    if (a == to) {
        combine_a_is_to(to, b);
    } else if (b == to) {
        combine_b_is_to(a, to);
    } else {
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 3; j++) {
                to->m[i][j] = a->m[i][0] * b->m[0][j]
                            + a->m[i][1] * b->m[1][j]
                            + a->m[i][2] * b->m[2][j];
            }
        }
    }
}

char** to_string_mat3(const mat3* m) {
    char** strs = malloc(sizeof(char*) * 5);

    /* 4 line drawing chars (3bytes) + (3 numbers ±xx.y) */
    for (size_t i = 0; i < 5; i++) {
        strs[i] = malloc(30);

        if (i == 0) {
            sprintf(strs[i], "┌─                 ─┐");
        } else if (i != 4) {
            sprintf(strs[i], "│ %5.1f %5.1f %5.1f │",
                    m->m[i - 1][0], m->m[i - 1][1], m->m[i - 1][2]);
        } else {
            sprintf(strs[i], "└─                 ─┘");
        }
    }

    return strs;
}

bool is_identity_matrix(const mat3* m) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if ((i == j && m->m[i][j] != 1.0) ||
                (i != j && m->m[i][j] != 0.0)) {
                return false;
            }
        };
    }

    return true;
}
