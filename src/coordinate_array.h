#ifndef LIBDRAWILLE_COORDINATE_ARRAY_H
#define LIBDRAWILLE_COORDINATE_ARRAY_H

#include <stdbool.h>

struct COArray {
    size_t pairs;
    size_t size;
    int* xs;
    int* ys;
};
typedef struct COArray COArray;

COArray* new_COArray();
void free_COArray(COArray* arr);

bool add_coordinate(COArray* arr, const int x, const int y);

#endif // LIBDRAWILLE_COORDINATE_ARRAY_H
