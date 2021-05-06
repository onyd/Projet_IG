#include "ei_draw.h"
#include "utils.h"
#include "stdio.h"
#include "math.h"
#include "stdbool.h"

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
    int top_left_x, top_right_x, top_left_y, bottom_left_y;
    if (clipper != NULL) {
        top_left_x = clipper->top_left.x;
        top_right_x = clipper->top_left.x + clipper->size.width;
        top_left_y = clipper->top_left.y;
        bottom_left_y = clipper->top_left.y + clipper->size.height;
    }

    // Draw all lines between points
    while (first != NULL && second != NULL) {
        /* Bresenham */
        int dx = second->point.x - first->point.x;
        int dy = second->point.y - first->point.y;
        int x = first->point.x;
        int y = first->point.y;

        uint32_t c = ei_map_rgba(surface, color);
        int sign_x = (dx > 0) ? 1 : -1;
        int sign_y = (dy > 0) ? 1 : -1;

        // Vertical and horizontal case
        if (dx == 0 && (clipper == NULL || x >= top_left_x && x <= top_right_x)) {
            while (y != second->point.y) {
                if (y >= top_left_y && y <= bottom_left_y) {
                    pixels[x + size.width * y] = c;
                }
                y += sign_y;
            }
            break;
        }
        if (dy == 0 && (clipper == NULL || y >= top_left_y && y <= bottom_left_y)) {
            while (x != second->point.x) {
                if (x >= top_left_x && x <= top_right_x) {
                    pixels[x + size.width * y] = c;
                }
                x += sign_x;
            }
            break;
        }

        // Swap variable for y-directed line
        bool swapped = abs(dx) < abs(dy);
        int x2 = second->point.x;
        int y2 = second->point.y;

        if (swapped) {
            swap(&x, &y);
            swap(&dx, &dy);
            swap(&sign_x, &sign_y);
            swap(&x2, &y2);
        }
        int E = dy / 2;

        while (sign_x * x < sign_x * x2) {
            x += sign_x;
            E += abs(dy);

            if (2 * E > abs(dx)) {
                y += sign_y;
                E -= abs(dx);
            }
            // Draw pixel in the buffer
            if (!swapped && (clipper == NULL || (x >= top_left_x && x <= top_right_x && y >= top_left_y && y <= bottom_left_y))) {
                    pixels[x + size.width * y] = c;
            } else if (swapped && (clipper == NULL || (y >= top_left_x && y <= top_right_x && x >= top_left_y && x <= bottom_left_y))){
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
    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    uint32_t c = ei_map_rgba(surface, color);

    // Find ymax and ymin to initialize size
    int ymax = first_point->point.y;
    int ymin = first_point->point.y;

    struct ei_linked_point_t *current = first_point;
    while (current != NULL) {
        if (ymax < current->point.y) {
            ymax = current->point.y;
        } else if (ymin > current->point.y) {
            ymin = current->point.y;
        }
        current = current->next;
    }

    // TC/TCA initialization
    struct table_cote **TC = calloc(ymax - ymin, sizeof(struct table_cote *));
    struct table_cote_actif *TCA = malloc(sizeof(struct table_cote_actif));
    TCA->head = NULL;

    // Build TC
    struct ei_linked_point_t *first = first_point;
    struct ei_linked_point_t *second = first_point->next;

    while (second != NULL) {
        struct ei_linked_point_t *p_min = y_argmin(first, second);
        struct ei_linked_point_t *p_max = y_argmax(first, second);

        int dx = p_max->point.x - p_min->point.x;
        int dy = p_max->point.y - p_min->point.y;

        // Ignore horizontal and vertical edges
        if (dx != 0 || dy != 0) {
            struct table_cote *edge = malloc(sizeof(struct table_cote));
            edge->ymax = p_max->point.y;
            edge->x_ymin = p_min->point.x;
            edge->dx = dx;
            edge->dy = dy;
            edge->E = dx / 2;

            append_left(edge, TC[p_min->point.y]);
        }
        first = second;
        second = second->next;
    }

    /* On initialise la table des côté actifs en se
     * placant à la premiere scanline correspondant à
     * y = ymin avec ymin le côté minimum de tout les points du polygone
     * puis on parcourt les lignes jusqu'à ce que la TCA et la TC soient vides*/
    for (uint32_t i = 0; i < ymax - ymin; i++) {
        uint32_t y = i + ymin;

        // Delete TCA edges such that y = ymax
        struct table_cote *current_tca = TCA->head;
        while (current_tca != NULL) {
            if (current_tca->ymax == y) {
                delete(current_tca, TCA);
            }
            current = current->next;
        }

        // Add current scanline starting edges
        struct table_cote *previous = TC[y];
        while (TC[y] != NULL && TCA->head != NULL) {
            struct table_cote *current_tc = TC[y];
            while (current_tc != NULL) {
                struct table_cote *tc = TC[y];
                TC[y] = tc->next;
                tc->next = NULL;
                sorting_insert(tc, TCA);
                current_tc = current_tc->next;
            }
        }

        // Draw pixels
        struct table_cote *current = TCA->head;
        while (current != NULL) {
            int x1 = current->x_ymin;
            current = current->next;
            int x2 = current->x_ymin;
            for (int i = x1; i < x2; i++) {
                pixels[i + size.width * y];
            }

            // Bresenham iterations for next intersections
            int dx = current->dx;
            int dy = current->dy;
            int sign_x = (dx > 0) ? 1 : -1;
            int sign_y = (dy > 0) ? 1 : -1;
            current->E += abs(dx);
            if (2 * current->E > abs(dy)) {
                current->x_ymin += sign_x;
                current->E -= abs(dy);
            }
        }
    }

    // Free resources
    for (uint32_t i = 0; i < ymax - ymin; i++) {
        tc_free(TC);
    }
    tca_free(TCA);
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
    uint32_t c = ei_map_rgba(surface, *color);
    for (uint32_t i = 0; i < size.width * size.height; i++) {
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

