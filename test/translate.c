#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/mat3.h"
#include "util.h"

static void test_zero(void** state) {
    mat3* expected = new_mat3();
    mat3* translated = new_mat3();
    translate_mat3(translated, 0, 0);
    
    assert_matrix(expected, translated);
    free_mat3(expected);
    free_mat3(translated);
}

static void test_x(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][2] = 5;

    mat3* translated = new_mat3();
    translate_mat3(translated, 5, 0);
    
    assert_matrix(expected, translated);
    free_mat3(expected);
    free_mat3(translated);
}

static void test_y(void** state) {
    mat3* expected = new_mat3();
    expected->m[1][2] = 5;

    mat3* translated = new_mat3();
    translate_mat3(translated, 0, 5);
    
    assert_matrix(expected, translated);
    free_mat3(expected);
    free_mat3(translated);
}

static void test_xy(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][2] = 7;
    expected->m[1][2] = 3;

    mat3* translated = new_mat3();
    translate_mat3(translated, 7, 3);
    
    assert_matrix(expected, translated);
    free_mat3(expected);
    free_mat3(translated);
}

static void test_negative_x(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][2] = -5;

    mat3* translated = new_mat3();
    translate_mat3(translated, 5, 0);
    translate_mat3(translated, -10, 0);

    assert_matrix(expected, translated);
    free_mat3(expected);
    free_mat3(translated);
}

static void test_negative_y(void** state) {
    mat3* expected = new_mat3();
    expected->m[1][2] = -5;

    mat3* translated = new_mat3();
    translate_mat3(translated, 0, 5);
    translate_mat3(translated, 0, -10);

    assert_matrix(expected, translated);
    free_mat3(expected);
    free_mat3(translated);
}

static void test_several(void** state) {
    mat3* expected = new_mat3();
    expected->m[0][2] = 15;
    expected->m[1][2] = 23;

    mat3* translated = new_mat3();
    translate_mat3(translated, 1, 1);
    translate_mat3(translated, 10, 15);
    translate_mat3(translated, 7, 10);
    translate_mat3(translated, -3, -3);

    assert_matrix(expected, translated);
    free_mat3(expected);
    free_mat3(translated);
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
