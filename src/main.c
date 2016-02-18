#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Canvas.h"
#include "mat3.h"
#include "stamp.h"

void stupid_square(Canvas* c, int size) {
    int max_x_y = size -1;

    for (int i = 0; i < size; i++) {
        set_pixel(c, WHITE, 0, i);
        set_pixel(c, WHITE, max_x_y, i);
        set_pixel(c, WHITE, i, 0);
        set_pixel(c, WHITE, i, max_x_y);
    }
}

int main(int argc, char** argv) {
    Canvas* c = new_canvas(100, 67 * 4);
    Stamp* s = new_circle_stamp(3, 25);
    s->tr_matrix = I;

    //stupid_square(c, 20);

    struct timespec wait, remaining;
    wait.tv_sec = 0;
    wait.tv_nsec = 100000000;

    char** buffer = new_buffer(c);

    for (float angle = 0; angle <= M_PI * 6; angle += 0.1) {
        fill(c, BLACK);
        s->tr_matrix = translate_mat3(I, 30, 50);
        s->tr_matrix = rotate_mat3(s->tr_matrix, -angle);
        s->tr_matrix = translate_mat3(s->tr_matrix, -25, -25);

        draw_stamp_outline(c, WHITE, s);

        draw(c, buffer);
        for (size_t i = 0; buffer[i] != NULL; i++) {
            printf("%s\n", buffer[i]);
        }

        nanosleep(&wait, &remaining);
    }

    free_stamp(s);
    free_buffer(buffer);
    free_canvas(c);

    return 0;
}
