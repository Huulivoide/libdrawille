#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/mat3.h"
#include "util.h"

static void test_zero(void** state) {
    assert_matrix(I, rotate_mat3(I, 0));
}

static void test_90(void** state) {
    mat3 expected = I;
    expected.m[0][0] = 0;
    expected.m[0][1] = 1;
    expected.m[1][0] = -1;
    expected.m[1][1] = 0;

    assert_matrix(expected, rotate_mat3(I, M_PI_2));
}

static void test_180(void** state) {
    mat3 expected = I;
    expected.m[0][0] = -1;
    expected.m[1][1] = -1;

    assert_matrix(expected, rotate_mat3(I, M_PI));
}

static void test_270(void** state) {
    mat3 expected = I;
    expected.m[0][0] = 0;
    expected.m[0][1] = -1;
    expected.m[1][0] = 1;
    expected.m[1][1] = 0;

    assert_matrix(expected, rotate_mat3(I, M_PI + M_PI_2));
}

static void test_360(void** state) {
    assert_matrix(I, rotate_mat3(I, 2 * M_PI));
}

static void test_negative(void** state) {
    mat3 expected = I;
    expected.m[0][0] = 0;
    expected.m[0][1] = -1;
    expected.m[1][0] = 1;
    expected.m[1][1] = 0;

    assert_matrix(expected, rotate_mat3(I, 0 - M_PI_2));
}

static void test_multiple(void** state) {
    mat3 expected = I;
    expected.m[0][0] = -0.7071067812;
    expected.m[0][1] = 0.7071067812;
    expected.m[1][0] = -0.7071067812;
    expected.m[1][1] = -0.7071067812;

    mat3 m = rotate_mat3(I, M_PI_4);
    m = rotate_mat3(m, M_PI_4);
    m = rotate_mat3(m, M_PI_4);

    assert_matrix(expected, m);
}

int main(int argx, char** argv) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_zero),
            cmocka_unit_test(test_90),
            cmocka_unit_test(test_180),
            cmocka_unit_test(test_270),
            cmocka_unit_test(test_360),
            cmocka_unit_test(test_negative),
            cmocka_unit_test(test_multiple)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
