#include <stdlib.h>

#include "coordinate_array.h"


COArray* new_COArray() {
    COArray* arr = malloc(sizeof(COArray));
    if (arr == NULL) {
        return NULL;
    }

    arr->size = 10;
    arr->pairs = 0;
    arr->xs = malloc(sizeof(int) * arr->size);

    if (arr->xs == NULL) {
        free(arr);
        return NULL;
    }

    arr->ys = malloc(sizeof(int) * arr->size);

    if (arr->ys == NULL) {
        free(arr->xs);
        free(arr);
        return NULL;
    }

    return arr;
}

void free_COArray(COArray* arr) {
    free(arr->xs);
    free(arr->ys);
    free(arr);
}

bool add_coordinate(COArray* arr, const int x, const int y) {
    if (arr->pairs == arr->size) {
        size_t new_size = arr->size * 2;
        int* allocatedx = realloc(arr->xs, sizeof(int) * new_size);
        if (allocatedx == NULL) {
            return false;
        }

        int* allocatedy = realloc(arr->xs, sizeof(int) * new_size);
        if (allocatedy == NULL) {
            return false;
        }
        arr->size = new_size;
        arr->xs = allocatedx;
        arr->ys = allocatedy;
    }

    arr->xs[arr->pairs] = x;
    arr->ys[arr->pairs] = y;
    arr->pairs++;

    return true;
}
