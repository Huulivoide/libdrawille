#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/mat3.h"
#include "util.h"

static void test_zero(void** state) {
    mat3* rotated = new_mat3();
    rotate_mat3(rotated, 0);

    mat3* expected = new_mat3();

    assert_matrix(expected, rotated);
    free_mat3(expected);
    free_mat3(rotated);
}

static void test_90(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = 0;
    expected->m[0][1] = 1;
    expected->m[1][0] = -1;
    expected->m[1][1] = 0;

    mat3* rotated = new_mat3();
    rotate_mat3(rotated, M_PI_2);

    assert_matrix(expected, rotated);
    free_mat3(expected);
    free_mat3(rotated);
}

static void test_180(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = -1;
    expected->m[1][1] = -1;

    mat3* rotated = new_mat3();
    rotate_mat3(rotated, M_PI);

    assert_matrix(expected, rotated);
    free_mat3(expected);
    free_mat3(rotated);
}

static void test_270(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = 0;
    expected->m[0][1] = -1;
    expected->m[1][0] = 1;
    expected->m[1][1] = 0;

    mat3* rotated = new_mat3();
    rotate_mat3(rotated, M_PI + M_PI_2);

    assert_matrix(expected, rotated);
    free_mat3(expected);
    free_mat3(rotated);
}

static void test_360(void** state) {
    mat3* rotated = new_mat3();
    rotate_mat3(rotated, 2 * M_PI);

    mat3* expected = new_mat3();

    assert_matrix(expected, rotated);
    free_mat3(expected);
    free_mat3(rotated);
}

static void test_negative(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = 0;
    expected->m[0][1] = -1;
    expected->m[1][0] = 1;
    expected->m[1][1] = 0;

    mat3* rotated = new_mat3();
    rotate_mat3(rotated, 0 - M_PI_2);

    assert_matrix(expected, rotated);
    free_mat3(expected);
    free_mat3(rotated);
}

static void test_multiple(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = -0.7071067812f;
    expected->m[0][1] = 0.7071067812f;
    expected->m[1][0] = -0.7071067812f;
    expected->m[1][1] = -0.7071067812f;

    mat3* m = new_mat3();
    rotate_mat3(m, M_PI_4);
    rotate_mat3(m, M_PI_4);
    rotate_mat3(m, M_PI_4);

    assert_matrix(expected, m);
    free_mat3(expected);
    free_mat3(m);
}

int main(int argc, char** argv) {
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
