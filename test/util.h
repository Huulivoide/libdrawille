#ifndef LIBDRAWILLE_TESTS_UTIL_H
#define LIBDRAWILLE_TESTS_UTIL_H

#include "../src/mat3.h"

extern const double delta;

void assert_matrix(mat3 expected, mat3 m);

#endif //LIBDRAWILLE_TESTS_UTIL_H
