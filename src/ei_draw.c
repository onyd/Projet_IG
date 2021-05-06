#include "ei_draw.h"
#include "stdio.h"
#include "utils.h"


uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color) {
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint32_t r, g, b, a = 0;
    r = (uint32_t) color.red << (8 * ir);
    g = (uint32_t) color.green << (8 * ig);
    b = (uint32_t) color.blue << (8 * ib);
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
    const ei_linked_point_t *first = first_point;
    const ei_linked_point_t *second = first_point->next;

    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);

    // Clipper coordinates
    int top_left_x;
    int top_right_x;
    int top_left_y;
    int bottom_left_y;
    if (clipper != NULL){
        top_left_x = clipper -> top_left.x;
        top_right_x = clipper -> top_left.x + clipper -> size.width;
        top_left_y = clipper -> top_left.y;
        bottom_left_y = clipper -> top_left.y + clipper -> size.height;
    }

    // Draw all lines between points
    while (first != NULL && second != NULL) {
        /* Bresenham */
        int dx = second->point.x - first->point.x;
        int dy = second->point.y - first->point.y;
        int x = first->point.x;
        int y = first->point.y;
        int E = 0;

        uint32_t c = ei_map_rgba(surface, color);
        uint32_t sign_x = (dx > 0) ? 1 : -1;
        uint32_t sign_y = (dy > 0) ? 1 : -1;

        // Swap variable for y-directed line
        if (dx < dy) {
            swap(&x, &y);
            swap(&dx, &dy);
            swap(&sign_x, &sign_y);
        }

        while (x != second->point.x && y != second->point.y) {
            x += sign_x;
            E += sign_x * dy;

            if (2 * E > dx) {
                y += sign_y;
                E -= sign_y * dx;
            }
            // Draw pixel in the buffer
            if (x >= top_left_x && x <= top_right_x && y >= top_left_y && y <= bottom_left_y){
                if (dx > dy) {
                    pixels[x + size.width * y] = c;
                } else {
                    pixels[y + size.width * x] = c;
                }
            }
        }
        first = second;
        second = second->next;
    }
}


void ei_draw_polygon(ei_surface_t surface,
                     const ei_linked_point_t *first_point,
                     ei_color_t color,
                     const ei_rect_t *clipper) {
    // on crée la table des côtés
    const ei_linked_point_t *debut = first_point;
    const ei_linked_point_t *prec = first_point;
    const ei_linked_point_t *current = first_point->next;
    struct table_cote *tab_cote;
    struct table_cote *parcourt = tab_cote;
    do {
        //On ignore les côtés horizontaux qui sont inutiles
        if (prec->point.y == current->point.y) {
            int ymax = max(prec->point.y, current->point.y);
            parcourt->ymax = ymax;
            if (prec->point.y == ymax) {
                parcourt->xymin = current->point.x;
            }
            else {
                parcourt->xymin = prec->point.x;
            }

            //Bresenham selon y
            int x = prec->point.x;
            int y = prec->point.y;
            int dx = current->point.x - prec->point.x;
            int dy = current->point.y - prec->point.y;
            int sign_x = (dx > 0) ? 1 : -1;
            int sign_y = (dy > 0) ? 1 : -1;
            int E = 0;
            while (x != current->point.x && y != current->point.y) {
                y += sign_y;
                E += sign_y * dx;

                if (2 * E > dx) {
                    x += sign_x;
                    E -= sign_x * dy;
                }
            }
            dx = (dx > 0) ? dx : -dx;
            dy = (dy > 0) ? dy : -dy;
            tab_cote->E = E;
            tab_cote->dx = dx;
            tab_cote->dy = dy;
        }
        parcourt = parcourt->next;
        prec = prec->next;
        current = current->next;
    } while(prec != debut);

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
    for (uint32_t i = 0; i < size.width * size.height; i++) {
        uint32_t c = ei_map_rgba(surface, *color);
        pixels[i] = c;
    }
    hw_surface_unlock(surface);
}

int ei_copy_surface(ei_surface_t destination,
                    const ei_rect_t *dst_rect,
                    ei_surface_t source,
                    const ei_rect_t *src_rect,
                    ei_bool_t alpha) {

}

