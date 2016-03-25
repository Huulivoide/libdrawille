#ifndef LIBDRAWILLE_MAT3_H
#define LIBDRAWILLE_MAT3_H

/**
 * 2D transformation matrix implementation.
 *
 * \file
 */

#include <stdbool.h>

/**
 * 3x3 2D transformation-matrix
 *
 * Represents the position, rotation, scale and shear of an stamp.
 *
 * mat3 is a wrapper around a float[3][3] table of 2D transformation matrix
 * values.
 */
struct mat3 {
    float m[3][3]; /** Row major nested array of the values */
};
typedef struct mat3 mat3;

/**
 * Creates a new matrix.
 *
 * \f[ Matrix form: M =
 * \begin{bmatrix}
 *     1 & 0 & 0 \\
 *     0 & 1 & 0 \\
 *     0 & 0 & 1
 * \end{bmatrix}
 * \f]
 *
 * \return 3x3 identity matrix.
 */
mat3* new_mat3();

/**
 * Frees the matrix.
 *
 * \param m matrix to delete
 */
void free_mat3(mat3* m);

/**
 * Clones the given matrix.
 *
 * Creates a new instance of mat3 and initializes it's values to those of
 * the input matrix.
 *
 * \param m matrix to clone
 * \return new clone of m
 */
mat3* clone_mat3(const mat3* m);

/**
 * Resets to identity matrix.
 *
 * Reinitializes the matrix to be an identity matrix.
 *
 * \param m matrix to reset
 */
void reset_mat3(mat3* m);

/**
 * Translates by given x and y deltas.
 *
 * Applies a translation, i.e. a shift/move to the matrix.
 *
 * \f[ Matrix form: M' = M
 * \begin{bmatrix}
 *     1 & 0 & x\_delta \\
 *     0 & 1 & y\_delta \\
 *     0 & 0 & 1
 * \end{bmatrix}
 * \f]
 *
 * \param m matrix to translate
 * \param x_delta >0: move right; 0: do nothing; <0: move left
 * \param y_delta >0: move down; 0: do nothing; <0: move up
 */
void translate_mat3(mat3* m, const float x_delta, const float y_delta);

/**
 * Scales by given x and y deltas.
 *
 * Applies a scaling to the matrix.
 *
 * Scaling factors:
 *     - 1: do nothing
 *     - ]0, 1[: make smaller
 *     - ]1, ∞]: make bigger
 *     - negative values mirror and scale the object
 *
 * For example translate_mat3(id_matrix, -1, -0.5) would cause the matrix
 * transform the stamp it was applied to be mirrored on both horizontal and
 * vertical axises and also be squashed/scaled to half of it's height.
 *
 * \f[ Matrix form: M' = M
 * \begin{bmatrix}
 *     x\_delta &     0    & 0 \\
 *         0    & y\_delta & 0 \\
 *         0    &     0    & 1
 * \end{bmatrix}
 * \f]
 *
 * \param m matrix to scale
 * \param x_delta horizontal scaling factor
 * \param y_delta vertical scaling factor
 */
void scale_mat3(mat3* m, const float x_delta, const float y_delta);

/**
 * Rotates around origin.
 *
 * Apply a rotation to the matrix.
 *
 * \f[ Matrix form: M' = M
 * \begin{bmatrix}
 *      cos(ɑ) & sin(ɑ) & 0 \\
 *     -sin(ɑ) & cos(ɑ) & 0 \\
 *         0   &    0   & 1
 * \end{bmatrix}
 * \f]
 *
 * \param m matrix to rotate
 * \param angle rotation angle in radians
 */
void rotate_mat3(mat3* m, const float angle);

/**
 * Shears by given x and y deltas.
 *
 * Shears, i.e. applies a transformation proportional to the
 * distance from the given axis.
 *
 * For example shear_mat3(id_matrix, 1, 0) would cause point a=(1, 1) to be
 * translated to (2, 1) and point b=(2, 2) to (4, 2).
 *
 * \f[ Matrix form: M' = M
 * \begin{bmatrix}
 *         1    & y\_delta & 0 \\
 *     x\_delta &     1    & 0 \\
 *         0    &     0    & 1
 * \end{bmatrix}
 * \f]
 *
 * \param m matrix to shear
 * \param x_delta horizontal shear
 * \param y_delta vertical shear
 */
void shear_mat3(mat3* m, const float x_delta, const float y_delta);

/**
 * Performs a matrix multiplication.
 *
 * Multiplies <b>A</b> with <b>B</b> and stores the result in <b>TO</b>.
 *
 * NOTE: Does not perform a full multiplication <b>A</b> and <b>B</b>
 * are both assumed to be transformation matrices and thus
 * \f$ t_{31} \f$, \f$ t_{32} \f$ and \f$ t_{33} \f$ are left untouched.
 *
 * If \a to is same matrix as \a a or \a b, then an additional 3 floats
 * worth of memory is used as temporary storage.
 *
 */
void combine_mat3(const mat3* a, const mat3* b, mat3* to);

char** to_string_mat3(const mat3* m);

/**
 * Checks whether the matrix is an identity matrix or not.
 */
bool is_identity_matrix(const mat3* m);

#endif //LIBDRAWILLE_MAT3_H
