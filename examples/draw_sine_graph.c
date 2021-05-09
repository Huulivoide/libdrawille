#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/Canvas.h"
#include "../src/utils.h"

#define radians(degrees) ((degrees) * M_PI / 180.0)

int main(int argc, char** argv) {
    size_t width, height;

    if (!get_console_size(&width, &height)) {
        return 1;
    }

    Canvas* c = new_canvas(width, height);

    struct timespec wait, remaining;
    wait.tv_sec = 0;
    wait.tv_nsec = 100000000;

    char** buffer = new_buffer(c);

    while (1) {
        fill(c, BLACK);
        for (int x = 0; x <= 1800; x += 10) {
            set_pixel(c, WHITE, x / 10, 10 + sin(radians(x)) * 10);
        }

        draw(c, buffer);
        for (size_t i = 0; buffer[i] != NULL; i++) {
            printf("%s\n", buffer[i]);
        }

        nanosleep(&wait, &remaining);
    }

    free_buffer(buffer);
    free_canvas(c);

    return 0;
}
