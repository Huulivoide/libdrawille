#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/Canvas.h"
#include "../src/stamp.h"

int main(int argc, char** argv) {
    Canvas* c = new_canvas(100, 67 * 4);
    Stamp* s = new_circle_stamp(4, 25);


    struct timespec wait, remaining;
    wait.tv_sec = 0;
    wait.tv_nsec = 100000000;

    char** buffer = new_buffer(c);

    for (float angle = 0; angle <= M_PI * 6; angle += 0.1) {
        fill(c, BLACK);
        reset_mat3(s->tr_matrix);
        translate_mat3(s->tr_matrix, 25, 50);
        rotate_mat3(s->tr_matrix, angle);
        translate_mat3(s->tr_matrix, -25, -25);

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
