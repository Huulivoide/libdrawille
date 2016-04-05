#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Canvas.h"
#include "point.h"

typedef unsigned char uchar;

const uchar pixmap[4][2] = {
    {0x01, 0x08},
    {0x02, 0x10},
    {0x04, 0x20},
    {0x40, 0x80}
};

char** new_buffer(const Canvas* c) {
    char** canvas = malloc(sizeof(char*) * (c->cheight + 1));

    if (canvas == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < c->cheight; i++) {
        canvas[i] = malloc((c->cwidth * 3)+ 1);

        if (canvas[i] == NULL) {
            free_buffer(canvas);
            return NULL;
        }

        canvas[i][c->cwidth * 3] = '\0';
    }

    canvas[c->cheight] = NULL;
    return canvas;
}

void free_buffer(char** buffer) {
    size_t pos = 0;
    while (buffer[pos] != NULL) {
        free(buffer[pos]);
        pos++;
    }
    free(buffer);
}

Canvas* new_canvas(const size_t width, const size_t height) {
    Canvas* c = malloc(sizeof(Canvas));

    if (c == NULL) {
        return NULL;
    }

    c->width = width;
    c->height = height;

    c->cwidth = width / 2;
    c->cheight = height / 4;

    c->canvas = calloc(c->height, c->width);

    if (c->canvas == NULL) {
        free(c);
        return NULL;
    }

    return c;
}

void free_canvas(Canvas* c) {
    free(c->canvas);
    free(c);
}

inline void set_pixel_unsafe(Canvas* c, const Color color,
                             const int x, const int y) {
    c->canvas[y*c->width + x] = color;
}

void set_pixel(Canvas* c, const Color color, const int x, const int y) {
    if(x >= 0 && x < c->width && y >= 0 && y < c->height) {
        set_pixel_unsafe(c, color, x, y);
    }
}

Color get_pixel(const Canvas* c, const int x, const int y) {
    if(x < 0 || x >= c->width || y < 0 || y >= c->height) {
        return BLACK;
    }

    return (c->canvas[y*x + x] == WHITE) ? WHITE : BLACK;
}

static void set_bytes(char* row, const uchar c, const size_t bpos) {
    row[bpos] = (char)0xE2;

    if (c & pixmap[3][0] && c & pixmap[3][1]) {
        row[bpos + 1] = (char)0xA3;
    } else if (c & pixmap[3][1]) {
        row[bpos + 1] = (char)0xA2;
    } else if (c & pixmap[3][0]) {
        row[bpos + 1] = (char)0xA1;
    } else {
        row[bpos + 1] = (char)0xA0;
    }

    row[bpos + 2] = (char)((0xBF & c) | 0x80);
}

void draw(const Canvas* c, char** buffer) {
    for (size_t y = 0; y < c->height; y += 4) {
        size_t buffer_row = y / 4;
        size_t bpos = 0;
        for(size_t x = 0; x < c->width; x +=2) {
            uchar* pos = c->canvas+(y*c->width + x);
            uchar byte = (pos[0]          << 0) | (pos[1]            << 3) |
                         (pos[1*c->width] << 1) | (pos[1*c->width+1] << 4) |
                         (pos[2*c->width] << 2) | (pos[2*c->width+1] << 5) |
                         (pos[3*c->width] << 6) | (pos[3*c->width+1] << 7);
            set_bytes(buffer[buffer_row], byte, bpos);
            bpos += 3;
        }
        buffer[buffer_row][bpos] = '\0';
    }
}

void clear(Canvas* c) {
    fill(c, BLACK);
}

void fill(Canvas* c, const Color color) {
    memset(c->canvas, color, c->width * c->height);
}

static inline Point extract_scale(const mat3* m) {
    return (Point) {
        sqrtf(powf(m->m[0][0], 2) + powf(m->m[1][0], 2)),
        sqrtf(powf(m->m[0][1], 2) + powf(m->m[1][1], 2))
    };
}

static Point calculate_new_size(const Canvas* c, const mat3* m) {
    Point ll = transform_point((Point) {0, 0}, m);
    Point lr = transform_point((Point) {c->width, 0}, m);
    Point ur = transform_point((Point) {c->width, c->height}, m);
    Point ul = transform_point((Point) {0, c->height}, m);

    float xmin = fminf(0, fminf(ll.x, fminf(lr.x, fminf(ur.x, ul.x))));
    float xmax = fmaxf(0, fmaxf(ll.x, fmaxf(lr.x, fmaxf(ur.x, ul.x))));
    float ymin = fminf(0, fminf(ll.y, fminf(lr.y, fminf(ur.y, ul.y))));
    float ymax = fmaxf(0, fmaxf(ll.y, fmaxf(lr.y, fmaxf(ur.y, ul.y))));

    return (Point) {
            ceilf(fabsf(xmax - xmin)),
            ceilf(fabsf(ymax - ymin))
    };
}

static void copy_canvas(Canvas* source, Canvas* target, const mat3* m) {
    for (int x = 0; x < source->width; x++) {
        for (int y = 0; y < source->height; y++) {
            if (get_pixel(source, x, y) == WHITE) {
                Point p = transform_point((Point) {x, y}, m);
                set_pixel(target, WHITE, (int) roundf(p.x), (int) roundf(p.y));
            }
        }
    }
}

Canvas* transform_canvas(Canvas* c, const mat3* transformations,
                         const ScalingMethod sm, const bool crop)
{
    if (is_identity_matrix(transformations)) {
        return NULL;
    }

    Canvas* target = NULL;

    Point scale = extract_scale(transformations);
    mat3* unscaled = clone_mat3(transformations);
    scale_mat3(unscaled, 1/scale.x, 1/scale.y);

    if (!crop) {
        Point new_size = calculate_new_size(c, transformations);
        target = new_canvas((size_t) new_size.x, (size_t) new_size.y);
    } else {
        target = new_canvas(c->width, c->height);
    }

    copy_canvas(c, target, unscaled);
    return target;
}
