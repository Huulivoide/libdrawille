#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <malloc.h>

#include "../src/mat3.h"
#include "util.h"


struct state {
    mat3* expected;
    mat3* a;
    mat3* b;
    mat3* c;
};

int reset_state(void** state_bundle) {
    struct state* st = *state_bundle;
    reset_mat3(st->expected);
    reset_mat3(st->a);
    reset_mat3(st->b);
    reset_mat3(st->c);

    return 0;
}

// Assume we can allocate the memory and skip null pointer checks
static int init(void **state_bundle) {
    struct state* st = malloc(sizeof(struct state));

    st->expected = new_mat3();
    st->a = new_mat3();
    st->b = new_mat3();
    st->c = new_mat3();

    *state_bundle = st;

    return 0;
}

static int end(void **state_bundle) {
    struct state* st = *state_bundle;

    free_mat3(st->expected);
    free_mat3(st->a);
    free_mat3(st->b);
    free_mat3(st->c);

    free(st);

    return 0;
}

static void test_to_a(void** state_bundle) {
    struct state* st = ((struct state*)(*state_bundle));
    combine_mat3(st->a, st->b, st->a);
    assert_matrix(st->expected, st->a);
}

static void test_to_b(void** state_bundle) {
    struct state* st = ((struct state*)(*state_bundle));
    combine_mat3(st->a, st->b, st->b);
    assert_matrix(st->expected, st->b);
}

static void test_to_c(void** state_bundle) {
    struct state* st = ((struct state*)(*state_bundle));
    combine_mat3(st->a, st->b, st->c);
    assert_matrix(st->expected, st->c);
}

static int setup_simple_translations(void** state_bundle) {
    reset_state(state_bundle);

    struct state* st = *state_bundle;

    st->expected->m[0][2] = 5;
    st->expected->m[1][2] = 12;

    st->a->m[0][2] = 2;
    st->a->m[1][2] = 5;

    st->b->m[0][2] = 3;
    st->b->m[1][2] = 7;

    return 0;
}

static int setup_full_matrices(void** state_bundle) {
    reset_state(state_bundle);

    struct state* st = *state_bundle;

    st->expected->m[0][0] = 129;
    st->expected->m[0][1] = 191;
    st->expected->m[0][2] = 258;
    st->expected->m[1][0] = 184;
    st->expected->m[1][1] = 272;
    st->expected->m[1][2] = 365;

    st->a->m[0][0] = 2;
    st->a->m[0][1] = 5;
    st->a->m[0][2] = 11;
    st->a->m[1][0] = 3;
    st->a->m[1][1] = 7;
    st->a->m[1][2] = 13;

    st->b->m[0][0] = 17;
    st->b->m[0][1] = 23;
    st->b->m[0][2] = 31;
    st->b->m[1][0] = 19;
    st->b->m[1][1] = 29;
    st->b->m[1][2] = 37;

    return 0;
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_to_a, reset_state), // I*I=I
        cmocka_unit_test_setup(test_to_b, reset_state),
        cmocka_unit_test_setup(test_to_c, reset_state),
        cmocka_unit_test_setup(test_to_a, setup_simple_translations),
        cmocka_unit_test_setup(test_to_b, setup_simple_translations),
        cmocka_unit_test_setup(test_to_c, setup_simple_translations),
        cmocka_unit_test_setup(test_to_a, setup_full_matrices),
        cmocka_unit_test_setup(test_to_b, setup_full_matrices),
        cmocka_unit_test_setup(test_to_c, setup_full_matrices),
    };

    return cmocka_run_group_tests(tests, init, end);
}
