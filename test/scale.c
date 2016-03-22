#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/mat3.h"
#include "util.h"

static void test_one(void** state) {
    mat3* expected = new_mat3();
    mat3* scaled = new_mat3();
    scale_mat3(scaled, 1, 1);

    assert_matrix(expected, scaled);
    free_mat3(expected);
    free_mat3(scaled);
}

static void test_x(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = 5;

    mat3* scaled = new_mat3();
    scale_mat3(scaled, 5, 1);

    assert_matrix(expected, scaled);
    free_mat3(expected);
    free_mat3(scaled);
}

static void test_y(void** state) {
    mat3* expected = new_mat3();
    expected->m[1][1] = 5;

    mat3* scaled = new_mat3();
    scale_mat3(scaled, 1, 5);

    assert_matrix(expected, scaled);
    free_mat3(expected);
    free_mat3(scaled);
}

static void test_xy(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = 7;
    expected->m[1][1] = 3;

    mat3* scaled = new_mat3();
    scale_mat3(scaled, 7, 3);

    assert_matrix(expected, scaled);
    free_mat3(expected);
    free_mat3(scaled);
}

static void test_negative_x(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = -5;

    mat3* scaled = new_mat3();
    scale_mat3(scaled, 5, 1);
    scale_mat3(scaled, -1, 1);

    assert_matrix(expected, scaled);
    free_mat3(expected);
    free_mat3(scaled);
}

static void test_negative_y(void** state) {
    mat3* expected = new_mat3();
    expected->m[1][1] = -5;

    mat3* scaled = new_mat3();
    scale_mat3(scaled, 1, 5);
    scale_mat3(scaled, 1, -1);

    assert_matrix(expected, scaled);
    free_mat3(expected);
    free_mat3(scaled);
}

static void test_several(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][0] = 16;
    expected->m[1][1] = -8;

    mat3* scaled = new_mat3();
    scale_mat3(scaled, 4, 2);
    scale_mat3(scaled, -1, -2);
    scale_mat3(scaled, -2, 1);
    scale_mat3(scaled, 2, 2);

    assert_matrix(expected, scaled);
    free_mat3(expected);
    free_mat3(scaled);
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
