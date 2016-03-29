#ifndef LIBDRAWILLE_CANVAS_H
#define LIBDRAWILLE_CANVAS_H

#include <stddef.h>

#include "mat3.h"

extern const unsigned char pixmap[4][2];

enum Color {
    BLACK = 0,
    WHITE
};
typedef enum Color Color;

struct Canvas
{
    size_t width;
    size_t height;
    size_t cwidth;
    size_t cheight;

    unsigned char** canvas;
};
typedef struct Canvas Canvas;

enum ScalingMethod {
    NEAREST_NEIGHBOR = 0
};
typedef enum ScalingMethod ScalingMethod;

Canvas* new_canvas(const size_t width, const size_t height);
void free_canvas(Canvas* c);

char** new_buffer(const Canvas* c);
void free_buffer(char** buffer);

void set_white_pixel_unsafe(Canvas* c, const int x, const int y);
void set_black_pixel_unsafe(Canvas* c, const int x, const int y);
void set_white_pixel(Canvas* c, const int x, const int y);
void set_black_pixel(Canvas* c, const int x, const int y);

void set_pixel(Canvas* c, Color color, const int x, const int y);
Color get_pixel(const Canvas* c, const int x, const int y);

void clear(Canvas* c);
void fill(Canvas* c, const Color color);
void draw(const Canvas* c, char** buffer);

Canvas* transform_canvas(Canvas* c, const mat3* transformations,
                      const ScalingMethod sm, const bool crop);

#endif // LIBDRAWILLE_CANVAS_H
