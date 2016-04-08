#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stamp.h"
#include "utils.h"

Stamp* new_polygon_stamp(Polygon* p) {
    Stamp* s = malloc(sizeof(Stamp));
    if (s == NULL) {
        return NULL;
    }

    s->polygon = p;
    s->tr_matrix = new_mat3();

    if (s->tr_matrix == NULL) {
        free(s);
        return NULL;
    }

    return s;
}

Stamp* new_rectangle_stamp(const size_t width, const size_t height) {
    Polygon* p = new_polygon();
    if (p == NULL) {
        return NULL;
    }

    add_vertex(p, (Point) {0, 0});
    add_vertex(p, (Point) {width, 0});
    add_vertex(p, (Point) {width, height});
    add_vertex(p, (Point) {0, height});
    close_polygon(p);

    Stamp* s = new_polygon_stamp(p);
    if (s == NULL) {
        free_polygon(p);
        return NULL;
    }

    return s;
}

Stamp* new_circle_stamp(const size_t steps, const size_t radius) {
    Polygon* p = new_polygon();
    if (p == NULL) {
        return NULL;
    }

    float stepa = (float) M_PI / (steps / 2.0f);
    for (size_t i = 0; i < steps; i++) {
        add_vertex(p, (Point) {
            cosf(stepa * i) * radius + radius,
            sinf(stepa * i) * radius + radius
        }); // +radius to move it to the positive side
    }
    close_polygon(p);

    Stamp* s = new_polygon_stamp(p);
    if (s == NULL) {
        free_polygon(p);
        return NULL;
    }

    return s;
}

void free_stamp(Stamp* s) {
    if (s->polygon) {
        free_polygon(s->polygon);
    }
    if (s->bitmap) {
        free_canvas(s->bitmap);
    }

    free_mat3(s->tr_matrix);
    free(s);
}

Point get_stamp_center(const Stamp* stamp) {
    if (stamp->type == POLYGON) {
        return get_polygon_center(stamp->polygon);
    }

    return (Point) {stamp->bitmap->width / 2, stamp->bitmap->height / 2};
}

void apply_matrix(Stamp* s) {
    if (s->polygon) {
        transform_polygon(s->polygon, s->tr_matrix);
    }

    reset_mat3(s->tr_matrix);
}

void bresenham(Canvas* c, Color color, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int incx = (x1 < x2) ? 1 : -1;
    int incy = (y1 < y2) ? 1 : -1;

    int err = ((dx > dy) ? dx : -dy) / 2;
    int e2 = 0;

    for(;;){
        set_pixel(c, color, x1, y1);
        if (x1 == x2 && y1 == y2) {
            break;
        }

        e2 = err;
        if (e2 >-dx) {
            err -= dy;
            x1 += incx;
        }
        if (e2 < dy) {
            err += dx;
            y1 += incy;
        }
    }
}

int draw_stamp_outline(Canvas* c, Color color, const Stamp* s) {
    if (s->type == BITMAP) {
        return -1;
    }

    for (size_t i = 1; i < s->polygon->next; i++) {
        Point from = transform_point(s->polygon->vertices[i - 1], s->tr_matrix);
        Point to = transform_point(s->polygon->vertices[i], s->tr_matrix);
        bresenham(c, color, (int) from.x, (int) from.y, (int) to.x, (int) to.y);
    }

    return 1;
}

void fill_triangle(Canvas* restrict canvas, const Color color,
                   const Point v1, const Point v2, const Point v3) {
    // 28.4 fixed-point coordinates
    const int Y1 = to_fixed(v1.y);
    const int Y2 = to_fixed(v2.y);
    const int Y3 = to_fixed(v3.y);

    const int X1 = to_fixed(v1.x);
    const int X2 = to_fixed(v2.x);
    const int X3 = to_fixed(v3.x);

    // Deltas
    const int DX12 = X1 - X2;
    const int DX23 = X2 - X3;
    const int DX31 = X3 - X1;

    const int DY12 = Y1 - Y2;
    const int DY23 = Y2 - Y3;
    const int DY31 = Y3 - Y1;

    // Fixed-point deltas
    const int FDX12 = DX12 << 4;
    const int FDX23 = DX23 << 4;
    const int FDX31 = DX31 << 4;

    const int FDY12 = DY12 << 4;
    const int FDY23 = DY23 << 4;
    const int FDY31 = DY31 << 4;

    // Bounding rectangle. Also clip to canvas size
    int minx, miny, maxx, maxy;
    get_clamped_min_max(X1, X2, X3, 0, to_fixed(canvas->width), &minx, &maxx);
    get_clamped_min_max(Y1, Y2, Y3, 0, to_fixed(canvas->height), &miny, &maxy);
    minx = (minx + 0xF) >> 4;
    miny = (miny + 0xF) >> 4;
    maxx = (maxx + 0xF) >> 4;
    maxy = (maxy + 0xF) >> 4;

    // Block size, standard 8x8 (must be power of two)
    const int q = 8;

    // Start in corner of 8x8 block
    minx &= ~(q - 1);
    miny &= ~(q - 1);

    // Half-edge constants
    int C1 = DY12 * X1 - DX12 * Y1;
    int C2 = DY23 * X2 - DX23 * Y2;
    int C3 = DY31 * X3 - DX31 * Y3;

    // Correct for fill convention
    C1 += (DY12 < 0 || (DY12 == 0 && DX12 > 0));
    C2 += (DY23 < 0 || (DY23 == 0 && DX23 > 0));
    C3 += (DY31 < 0 || (DY31 == 0 && DX31 > 0));

    // find box test offsets for each edge
    int eo1 = 0;
    if (FDY12 < 0) eo1 -= FDY12 << 3; //meaning y1 is above y2, block size is 8 ==> shl 3
    if (FDX12 > 0) eo1 += FDX12 << 3; //meaning x1 is right of x2

    int eo2 = 0;
    if (FDY23 < 0) eo2 -= FDY23 << 3; //meaning y2 is above y3
    if (FDX23 > 0) eo2 += FDX23 << 3; //meaning x2 is right of x3

    int eo3 = 0;
    if (FDY31 < 0) eo3 -= FDY31 << 3; //meaning y3 is above y1
    if (FDX31 > 0) eo3 += FDX31 << 3; //meaning x3 is right of x1

    //these are the offsets fo the bottom-right block corner
    const int ei1 = ((DX12 - DY12) << 7) - eo1; //block size is 8 ==> shl 3 + 4
    const int ei2 = ((DX23 - DY23) << 7) - eo2;
    const int ei3 = ((DX31 - DY31) << 7) - eo3;

    // Loop through blocks
    for(int y = miny; y < maxy; y += q) {
        // Have we already filled an block?
        unsigned char filled = 0;

        for(int x = minx; x < maxx; x += q) {
            unsigned char* canvas_pos = canvas->canvas+(y*canvas->width + x);
            // Corners of block
            int x0 = x << 4;
            int y0 = y << 4;

            int CX1 = C1 + DX12 * y0 - DY12 * x0;
            int CX2 = C2 + DX23 * y0 - DY23 * x0;
            int CX3 = C3 + DX31 * y0 - DY31 * x0;

            // Skip block when outside an edge
            if((CX1+eo1) < 0 || (CX2+eo2) < 0 || (CX3+eo3) < 0) {
                if (filled & 0x80) {
                    // We have hit a filled block before, but now we've hit an
                    // empty one. we can skip to the next line.
                    break;
                } else {
                    // We have NOT hit a filled block before,
                    // so we just skip to the next one.
                    continue;
                }
            }

            // Accept whole block when totally covered
            if((CX1+ei1) > 0 && (CX2+ei2) > 0 && (CX3+ei3) > 0) {
                for(int iy = 0; iy < q; iy++) {
                    memset(canvas_pos, color, q);
                    canvas_pos += canvas->width;
                }
            } else { // Partially covered block
                int CY1 = CX1;
                int CY2 = CX2;
                int CY3 = CX3;

                for (int iy = 0; iy < q; iy++) {
                    CX1 = CY1;
                    CX2 = CY2;
                    CX3 = CY3;

                    for (int ix = 0; ix < q; ix++) {
                        if (CX1 > 0 && CX2 > 0 && CX3 > 0) {
                            canvas_pos[ix] = color;
                        }

                        CX1 -= FDY12;
                        CX2 -= FDY23;
                        CX3 -= FDY31;
                    }
                    CY1 += FDX12;
                    CY2 += FDX23;
                    CY3 += FDX31;

                    canvas_pos += canvas->width;
                }
            }
            filled = 0x80;
        }
    }
}

int fill_shape(Canvas* c, Color color, const Stamp* s) {
    Point p1 = transform_point(s->polygon->vertices[0], s->tr_matrix);
    Point p2 = transform_point(s->polygon->vertices[1], s->tr_matrix);

    for (int i = 2; i <= s->polygon->next; i++) {
        Point p3 = transform_point(s->polygon->vertices[i], s->tr_matrix);
        fill_triangle_implementation(c, color, p3, p2, p1);
        p2 = p3;
    }

   return 1;
}

