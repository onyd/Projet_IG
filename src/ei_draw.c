#include "ei_draw.h"


void ei_draw_polyline(ei_surface_t surface,
                      const ei_linked_point_t *first_point,
                      ei_color_t color,
                      const ei_rect_t *clipper) {

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