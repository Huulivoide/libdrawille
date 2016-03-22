#ifndef LIBDRAWILLE_MAT3_H
#define LIBDRAWILLE_MAT3_H

#include <stdbool.h>

struct mat3 {
    float m[3][3];
};

typedef struct mat3 mat3;

mat3* new_mat3();
void free_mat3(mat3* m);
mat3* clone_mat3(const mat3* m);

void reset_mat3(mat3* m);

void translate_mat3(mat3* m, const float x_delta, const float y_delta);
void scale_mat3(mat3* m, const float x_delta, const float y_delta);
void rotate_mat3(mat3* m, const float angle);
void shear_mat3(mat3* m, const float x_delta, const float y_delta);

void combine_mat3(const mat3* a, const mat3* b, mat3* to);

char** to_string_mat3(const mat3* m);

bool is_identity_matrix(const mat3* m);

#endif //LIBDRAWILLE_MAT3_H
