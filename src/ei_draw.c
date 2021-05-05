#include "ei_draw.h"
#include "stdio.h"

uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color) {
    int ir, ig, ib, ia;

    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint32_t r, g, b, a = 0;

    r = (uint32_t)color.red << (8 * ir);
    g = (uint32_t)color.green << (8 * ig);
    b = (uint32_t)color.blue << (8 * ib);
    if (ia != -1) {
        return r | g | b | a;
    } else {
        return r | g | b;
    }
}

void ei_draw_polyline(ei_surface_t surface,
                      const ei_linked_point_t *first_point,
                      ei_color_t color,
                      const ei_rect_t *clipper) {
    const ei_linked_point_t *prev = first_point;
    const ei_linked_point_t *current = first_point->next;

    hw_surface_lock(surface);
    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);

    // Draw all lines between points
    while (current != NULL) {
        /* Bresenham */
        int dx = current->point.x - prev->point.x;
        int dy = current->point.y - prev->point.y;

        int x = prev->point.x;
        int y = prev->point.y;
        int E = 0;
        while (x != current->point.x && y != current->point.y) {
            if (dx > 0) {
                x++;
                E += dy;
            } else {
                x--;
                E -= dy;
            }
            if (2 * E > dx) {
                if (dy > 0) {
                    y++;
                    E -= dx;
                } else {
                    y--;
                    E += dx;
                }
            }
            // Draw pixel in the buffer
            uint32_t c = ei_map_rgba(surface, color);
            pixels[x + size.width * y] = c;
        }
        prev = current;
        current = current->next;

    }
    hw_surface_unlock(surface);
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
    hw_surface_lock(surface);
    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    for (uint32_t i = 0; i< size.width*size.height;i++){
        pixels[i] = "0xFFFFFFFF";
    }
    hw_surface_unlock(surface);
}

int ei_copy_surface(ei_surface_t destination,
                    const ei_rect_t *dst_rect,
                    ei_surface_t source,
                    const ei_rect_t *src_rect,
                    ei_bool_t alpha) {

}

