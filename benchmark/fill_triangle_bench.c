#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/stamp.h"
#include "../src/x86/x86.h"
#include "../src/utils.h"

const char* opt_sse = "sse";
const char* opt_c = "c";

const Point triangles[3][3] = {
    {{0, 0}, {100, 100}, {100, 0}},
    {{50, 0}, {0, 100}, {100, 100}},
    {{5.123, 5.0}, {13.8, 20.5}, {35, 23.3}}
};

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
}

int main(int argc, char** argv) {
    Canvas* canvas = new_canvas(200, 200);

    char* endc;
    size_t round_multiplier = strtoull(argv[1], &endc, 10);
    size_t rounds = round_multiplier * 1000;


    size_t total_time = 0;
    struct timespec start;
    struct timespec end;
    struct timespec diff;

    for (size_t round = 0; round < rounds; round++) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        fill_triangle(canvas, WHITE, triangles[0][0], triangles[0][1], triangles[0][2]);
        fill_triangle(canvas, WHITE, triangles[1][0], triangles[1][1], triangles[1][2]);
        fill_triangle(canvas, WHITE, triangles[2][0], triangles[2][1], triangles[2][2]);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        timespec_diff(&start, &end, &diff);
        total_time += diff.tv_sec * 1000000000 + diff.tv_nsec;
    }

    printf("Pure C\n");
    printf("      total nanoseconds: %lu\n", total_time);
    printf("          total seconds: %.2fs\n", total_time / 1000000000.0);
    printf("average run time per 1M: %.2fs\n", total_time / round_multiplier / 1000000000.0);

    total_time = 0;
    for (size_t round = 0; round < rounds; round++) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        fill_triangle_sse4(canvas, WHITE, triangles[0][0], triangles[0][1], triangles[0][2]);
        fill_triangle_sse4(canvas, WHITE, triangles[1][0], triangles[1][1], triangles[1][2]);
        fill_triangle_sse4(canvas, WHITE, triangles[2][0], triangles[2][1], triangles[2][2]);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        timespec_diff(&start, &end, &diff);
        total_time += diff.tv_sec * 1000000000 + diff.tv_nsec;
    }

    printf("\n\nSSE 4.1\n");
    printf("      total nanoseconds: %lu\n", total_time);
    printf("          total seconds: %.2fs\n", total_time / 1000000000.0);
    printf("average run time per 1M: %.2fs\n", total_time / round_multiplier / 1000000000.0);

    total_time = 0;
    for (size_t round = 0; round < rounds; round++) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        fill_triangle_avx2(canvas, WHITE, triangles[0][0], triangles[0][1], triangles[0][2]);
        fill_triangle_avx2(canvas, WHITE, triangles[1][0], triangles[1][1], triangles[1][2]);
        fill_triangle_avx2(canvas, WHITE, triangles[2][0], triangles[2][1], triangles[2][2]);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        timespec_diff(&start, &end, &diff);
        total_time += diff.tv_sec * 1000000000 + diff.tv_nsec;
    }

    printf("\n\nAVX2\n");
    printf("      total nanoseconds: %lu\n", total_time);
    printf("          total seconds: %.2fs\n", total_time / 1000000000.0);
    printf("average run time per 1M: %.2fs\n", total_time / round_multiplier / 1000000000.0);
    
    free_canvas(canvas);

    return 0;
}
