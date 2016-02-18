#ifndef LIBDRAWILLE_MAT3_H
#define LIBDRAWILLE_MAT3_H

#include <stdbool.h>

struct mat3 {
    float m[3][3];
};

typedef struct mat3 mat3;

extern const mat3 I;

mat3 translate_mat3(const mat3 m, const float x_delta, const float y_delta);
mat3 scale_mat3(const mat3 m, const float x_delta, const float y_delta);
mat3 rotate_mat3(const mat3 m, const float angle);
mat3 shear_mat3(const mat3 m, const float x_delta, const float y_delta);

mat3 combine_mat3(const mat3 a, const mat3 b);

char** to_string_mat3(const mat3 m);

bool is_identity_matrix(const mat3 m);

#endif //LIBDRAWILLE_MAT3_H
