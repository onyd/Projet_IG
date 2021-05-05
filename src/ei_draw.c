#include "ei_draw.h"

uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color) {

}


void ei_draw_polyline(ei_surface_t surface,
                      const ei_linked_point_t *first_point,
                      ei_color_t color,
                      const ei_rect_t *clipper) {
    prev, current = NULL, first_point;
    uint32_t *s = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    while (current != NULL) {
        /* Bresenham */
        int dx = current.x - prev.x;
        int dy = current.y - prev.y;

        int x, y = 0;
        int E = 0;
        while (x != (current.x && y != current.y) {
            if dx > 0
            {
                x++;
                E += dx;
            } else {
                x--;
                E -= dx;
            }
            if (2 * E > dx) {
                if dy > 0
                {
                    y++;
                    E -= dx;
                } else {
                    y--;
                    E += dx;
                }
            }
            uint32_t c = ei_map_rgba(surface, color);
            s[x + size.width * y] = c;
        }
    }
    prev = current;
    next = first_point++;
}

}

void ei_draw_polygon(ei_surface_t surface,
                     const ei_linked_point_t *first_point,
                     ei_color_t color,
                     const ei_rect_t *clipper) {

}

void ei_draw_text(ei_surface_t surface,
                  const ei_point_t *where,
                  const char *text,
                  ei_font_t font,
                  ei_color_t color,
                  const ei_rect_t *clipper) {

}

void ei_fill(ei_surface_t surface,
             const ei_color_t *color,
             const ei_rect_t *clipper) {

}

int ei_copy_surface(ei_surface_t destination,
                    const ei_rect_t *dst_rect,
                    ei_surface_t source,
                    const ei_rect_t *src_rect,
                    ei_bool_t alpha) {

}

uint32_t		ei_map_rgba		(ei_surface_t surface, ei_color_t color){
    int* ir;
    int* ig;
    int* ib;
    int* ia;
    hw_surface_get_channel_indices(surface, ir, ig, ib, ia);
    uint32_t r, g, b, a = 0;
    if (ir != -1) {
        r = color.red << (8 * ir);
    }
    if (ig != -1) {
        g = color.green << (8 * ig);
    }
    if (ib != -1) {
        b = color.blue << (8 * ib);
    }
    if (ia != -1) {
        a = color.alpha << (8 * ia);
    }
    uint32_t color = r || g || b || a;
    return color;
}