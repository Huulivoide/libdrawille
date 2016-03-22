#ifndef LIBDRAWILLE_TESTS_UTIL_H
#define LIBDRAWILLE_TESTS_UTIL_H

#include "../src/mat3.h"

extern const double delta;

void assert_matrix(const mat3* expected, const mat3* m);

#endif //LIBDRAWILLE_TESTS_UTIL_H
