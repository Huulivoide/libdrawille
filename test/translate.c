#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/mat3.h"
#include "util.h"

static void test_zero(void** state) {
    assert_matrix(I, translate_mat3(I, 0, 0));
}

static void test_x(void** state) {
    mat3 expected = I;
    expected.m[0][2] = 5;

    assert_matrix(expected, translate_mat3(I, 5, 0));
}

static void test_y(void** state) {
    mat3 expected = I;
    expected.m[1][2] = 5;

    assert_matrix(expected, translate_mat3(I, 0, 5));
}

static void test_xy(void** state) {
    mat3 expected = I;
    expected.m[0][2] = 7;
    expected.m[1][2] = 3;

    assert_matrix(expected, translate_mat3(I, 7, 3));
}

static void test_negative_x(void** state) {
    mat3 expected = I;
    expected.m[0][2] = -5;

    mat3 m = translate_mat3(I, 5, 0);
    m = translate_mat3(m, -10, 0);

    assert_matrix(expected, m);
}

static void test_negative_y(void** state) {
    mat3 expected = I;
    expected.m[1][2] = -5;

    mat3 m = translate_mat3(I, 0, 5);
    m = translate_mat3(m, 0, -10);

    assert_matrix(expected, m);
}

static void test_several(void** state) {
    mat3 expected = I;
    expected.m[0][2] = 15;
    expected.m[1][2] = 23;

    mat3 m = translate_mat3(I, 1, 1);
    m = translate_mat3(m, 10, 15);
    m = translate_mat3(m, 7, 10);
    m = translate_mat3(m, -3, -3);

    assert_matrix(expected, m);
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_zero),
        cmocka_unit_test(test_x),
        cmocka_unit_test(test_y),
        cmocka_unit_test(test_xy),
        cmocka_unit_test(test_negative_x),
        cmocka_unit_test(test_negative_y),
        cmocka_unit_test(test_several),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
