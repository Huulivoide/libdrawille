#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/mat3.h"
#include "util.h"

static void test_one(void** state) {
    assert_matrix(I, scale_mat3(I, 1, 1));
}

static void test_x(void** state) {
    mat3 expected = I;
    expected.m[0][0] = 5;

    assert_matrix(expected, scale_mat3(I, 5, 1));
}

static void test_y(void** state) {
    mat3 expected = I;
    expected.m[1][1] = 5;

    assert_matrix(expected, scale_mat3(I, 1, 5));
}

static void test_xy(void** state) {
    mat3 expected = I;
    expected.m[0][0] = 7;
    expected.m[1][1] = 3;

    assert_matrix(expected, scale_mat3(I, 7, 3));
}

static void test_negative_x(void** state) {
    mat3 expected = I;
    expected.m[0][0] = -5;

    mat3 m = scale_mat3(I, 5, 1);
    m = scale_mat3(m, -1, 1);

    assert_matrix(expected, m);
}

static void test_negative_y(void** state) {
    mat3 expected = I;
    expected.m[1][1] = -5;

    mat3 m = scale_mat3(I, 1, 5);
    m = scale_mat3(m, 1, -1);

    assert_matrix(expected, m);
}

static void test_several(void** state) {
    mat3 expected = I;
    expected.m[0][0] = 16;
    expected.m[1][1] = -8;

    mat3 m = scale_mat3(I, 4, 2);
    m = scale_mat3(m, -1, -2);
    m = scale_mat3(m, -2, 1);
    m = scale_mat3(m, 2, 2);

    assert_matrix(expected, m);
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_one),
            cmocka_unit_test(test_x),
            cmocka_unit_test(test_y),
            cmocka_unit_test(test_xy),
            cmocka_unit_test(test_negative_x),
            cmocka_unit_test(test_negative_y),
            cmocka_unit_test(test_several),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
