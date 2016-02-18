#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>

#include <cmocka.h>

#include "util.h"
#include "../src/mat3.h"

const double delta = 0.000001;

static size_t print_matrix_header(char* str, mat3 expected, mat3 m) {
    char** expected_str = to_string_mat3(expected);
    char** m_str = to_string_mat3(m);

    size_t bytes = 0;

    bytes += (size_t) sprintf(str, "\n       Expected       │         Value\n");
    for (size_t i = 0; i < 5; i++) {
        bytes += (size_t) sprintf(str + bytes, "%s │ %s\n", expected_str[i], m_str[i]);
        free(expected_str[i]);
        free(m_str[i]);
    }

    free(expected_str);
    free(m_str);

    return bytes;
}

void assert_matrix(mat3 expected, mat3 m) {
    char* str = NULL;
    size_t bytes = 0;
    bool header_not_printed = true;

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (fabs(expected.m[i][j] - m.m[i][j]) > delta) {
                if (header_not_printed) {
                    str = malloc(4096);
                    bytes += print_matrix_header(str, expected, m);
                    header_not_printed = false;
                }
                bytes += (size_t)sprintf(str+bytes, "(%zu, %zu): %10.6f    │ (%zu, %zu): %10.6f\n",
                                         i, j, expected.m[i][j], i, j, m.m[i][j]);
            }
        }
    }

    if (str != NULL) {
        fail_msg("%s", str);
    }
}
