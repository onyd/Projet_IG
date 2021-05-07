#include "ei_draw.h"
#include "utils.h"
#include "stdio.h"
#include "math.h"
#include "stdbool.h"
#include "stdlib.h"
#include "ei_utils.h"

uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color) {
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint32_t r, g, b, a = 0;
    r = (uint32_t) color.red << (8 * ir);
    g = (uint32_t) color.green << (8 * ig);
    b = (uint32_t) color.blue << (8 * ib);
    ia = 6 - ir - ig - ib;
    a = (uint32_t) color.alpha << (8 * ia);
    return r | g | b | a;
}

void ei_draw_polyline(ei_surface_t surface,
                      const ei_linked_point_t *first_point,
                      ei_color_t color,
                      const ei_rect_t *clipper) {
    const ei_linked_point_t *first = first_point;
    const ei_linked_point_t *second = first_point->next;

    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    uint32_t c = ei_map_rgba(surface, color);

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

        int sign_x = (dx > 0) ? 1 : -1;
        int sign_y = (dy > 0) ? 1 : -1;

        // Vertical and horizontal case
        if (dx == 0 && (clipper == NULL || x >= top_left_x && x <= top_right_x)) {
            while (y != second->point.y) {
                if (clipper == NULL || y >= top_left_y && y <= bottom_left_y) {
                    pixels[x + size.width * y] = c;
                }
                y += sign_y;
            }
            first = second;
            second = second->next;
            continue;
        }
        if (dy == 0 && (clipper == NULL || y >= top_left_y && y <= bottom_left_y)) {
            while (x != second->point.x) {
                if (clipper == NULL || x >= top_left_x && x <= top_right_x) {
                    pixels[x + size.width * y] = c;
                }
                x += sign_x;
            }
            first = second;
            second = second->next;
            continue;
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
            if (!swapped &&
                (clipper == NULL || (x >= top_left_x && x <= top_right_x && y >= top_left_y && y <= bottom_left_y))) {
                pixels[x + size.width * y] = c;
            } else if (swapped && (clipper == NULL ||
                                   (y >= top_left_x && y <= top_right_x && x >= top_left_y && x <= bottom_left_y))) {
                pixels[y + size.width * x] = c;
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

    int top_left_x, top_right_x, top_left_y, bottom_left_y;
    if (clipper != NULL) {
        top_left_x = clipper->top_left.x;
        top_right_x = clipper->top_left.x + clipper->size.width;
        top_left_y = clipper->top_left.y;
        bottom_left_y = clipper->top_left.y + clipper->size.height;
    }

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

        // Ignore horizontal edges
        if (dy != 0) {
            struct table_cote *edge = malloc(sizeof(struct table_cote));
            edge->ymax = p_max->point.y;
            edge->x_ymin = p_min->point.x;
            edge->x_ymax = p_max->point.x;
            edge->dx = dx;
            edge->dy = dy;
            edge->E = dx / 2;
            edge->inv_p = ceil(dx / dy);

            edge->next = TC[p_min->point.y - ymin];
            TC[p_min->point.y - ymin] = edge;
        }
        first = second;
        second = second->next;
    }


    /* On initialise la table des côté actifs en se
     * placant à la premiere scanline correspondant à
     * y = ymin avec ymin le côté minimum de tout les points du polygone
     * puis on parcourt les lignes jusqu'à ce que la TCA et la TC soient vides*/
    uint32_t y = ymin;
    while (y < ymax) {
        uint32_t i = y - ymin;

        // Delete TCA edges such that y = ymax
        struct table_cote *current_tca = TCA->head;
        while (current_tca != NULL) {
            if (current_tca->ymax == y) {
                delete(current_tca, TCA);
            }
            current_tca = current_tca->next;
        }

        // Add current scanline starting edges while keeping TCA sorted
        while (TC[i] != NULL) {
            struct table_cote *tc = TC[i];
            TC[i] = tc->next;
            tc->next = NULL;
            sorting_insert(tc, TCA);
        }

        // Draw pixels
        struct table_cote *current = TCA->head;
        while (current != NULL && current->next != NULL) {
            int x1 = current->x_ymin;
            current = current->next;
            int x2 = current->x_ymin;
            for (int k = x1; k < x2; k++) {
                if (clipper == NULL || (k >= top_left_x && k <= top_right_x && y >= top_left_y && y <= bottom_left_y)) {
                    pixels[k + size.width * y] = c;
                }
            }
            current = current->next;
        }

        y++;

        // Bresenham iterations for next intersections
        current = TCA->head;
        while (current != NULL) {
            int sign_x = (current->dx > 0) ? 1 : -1;

            current->E += abs(current->dx);
            if (2 * current->E > abs(current->dx)) {
                if (abs(current->dx) > abs(current->dy)) {
                    current->x_ymin += current->inv_p;
                } else {
                    current->x_ymin += sign_x;
                }
                current->E -= abs(current->dy);
            }
            current = current->next;
        }
    }

    // Free resources
    for (uint32_t k = 0; k < ymax - ymin; k++) {
        tc_free(TC[k]);
    }
    tca_free(TCA);
}

void ei_draw_text(ei_surface_t surface,
                  const ei_point_t *where,
                  const char *text,
                  ei_font_t font,
                  ei_color_t color,
                  const ei_rect_t *clipper) {
    ei_surface_t new_surface = hw_text_create_surface(text, font, color);
    ei_size_t size_dst_rect = hw_surface_get_size((const) new_surface);
    const ei_rect_t	dst_rect = ei_rect(*where, size_dst_rect);

    ei_copy_surface(new_surface, clipper, surface, clipper, false);

}

void ei_fill(ei_surface_t surface,
             const ei_color_t *color,
             const ei_rect_t *clipper) {
    int top_left_x, top_right_x, top_left_y, bottom_left_y;
    if (clipper != NULL) {
        top_left_x = clipper->top_left.x;
        top_right_x = clipper->top_left.x + clipper->size.width;
        top_left_y = clipper->top_left.y;
        bottom_left_y = clipper->top_left.y + clipper->size.height;
    }
    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    uint32_t c = ei_map_rgba(surface, *color);
    for (uint32_t y = 0; y < size.height; y++) {
        for (uint32_t x = 0; x< size.width; x++) {
            if (clipper == NULL || (x >= top_left_x && x <= top_right_x && y >= top_left_y && y <= bottom_left_y)) {
                pixels[x + size.width * y] = c;
            }
        }
    }
}

int ei_copy_surface(ei_surface_t destination,
                    const ei_rect_t *dst_rect,
                    ei_surface_t source,
                    const ei_rect_t *src_rect,
                    ei_bool_t alpha) {
    ei_size_t dst_size = hw_surface_get_size(destination);
    ei_size_t src_size = hw_surface_get_size(source);
    uint32_t *dst_pixels = (uint32_t *) hw_surface_get_buffer(destination);
    uint32_t *src_pixels = (uint32_t *) hw_surface_get_buffer(source);
    // If it's not the same size between both surfaces then failure
    if (src_size.width != dst_size.width && src_size.height != dst_size.height) {
        return 1;
    }
    hw_surface_lock(destination);
    hw_surface_lock(source);
    int src_top_left_x, src_top_right_x, src_top_left_y, src_bottom_left_y;
    int dst_top_left_x, dst_top_right_x, dst_top_left_y, dst_bottom_left_y;
    if (src_rect != NULL) {
        src_top_left_x = src_rect->top_left.x;
        src_top_right_x = src_rect->top_left.x + src_rect->size.width;
        src_top_left_y = src_rect->top_left.y;
        src_bottom_left_y = src_rect->top_left.y + src_rect->size.height;
    }
    if (dst_rect != NULL) {
        dst_top_left_x = dst_rect->top_left.x;
        dst_top_right_x = dst_rect->top_left.x + dst_rect->size.width;
        dst_top_left_y = dst_rect->top_left.y;
        dst_bottom_left_y = dst_rect->top_left.y + dst_rect->size.height;
    }
    for (int y=0; y<src_size.height; y++) {
        for (int x=0; x<src_size.width; x++) {
            // Draw pixel in the buffer
            if ((src_rect == NULL ||
            (x >= src_top_left_x && x <= src_top_right_x && y >= src_top_left_y && y <= src_bottom_left_y)) &&
            (dst_rect == NULL ||
             (x >= dst_top_left_x && x <= dst_top_right_x && y >= dst_top_left_y && y <= dst_bottom_left_y))) {
                if (!alpha) {
                    dst_pixels[x + dst_size.width * y] = src_pixels[x + dst_size.width * y];
                }
                else {
                    int ir, ig, ib, ia;
                    hw_surface_get_channel_indices(destination, &ir, &ig, &ib, &ia);
                    ia = 6 - (ir + ib + ig);
                    uint32_t src_r, src_g, src_b, src_a = 0;
                    uint32_t dst_r, dst_g, dst_b = 0;
                    uint32_t dst_a = 255 << 8*ia;
                    //We take the RGB color of the source and the destination
                    src_r = (uint8_t) (src_pixels[x + dst_size.width * y] >> (8 * ir));
                    src_g = (uint8_t) (src_pixels[x + dst_size.width * y] >> (8 * ig));
                    src_b = (uint8_t) (src_pixels[x + dst_size.width * y] >> (8 * ib));
                    src_a = (uint8_t) (src_pixels[x + dst_size.width * y] >> (8 * ia));
                    dst_r = (uint8_t) (dst_pixels[x + dst_size.width * y] >> (8 * ir));
                    dst_g = (uint8_t) (dst_pixels[x + dst_size.width * y] >> (8 * ig));
                    dst_b = (uint8_t) (dst_pixels[x + dst_size.width * y] >> (8 * ib));
                    // apply the transparancy
                    dst_r = ((src_a*src_r + (255 - src_a)*dst_r)/255) << (8*ir);
                    dst_g = ((src_a*src_g + (255 - src_a)*dst_g)/255) << (8*ig);
                    dst_b = ((src_a*src_b + (255 - src_a)*dst_b)/255) << (8*ib);
                    uint32_t color = dst_r | dst_g | dst_b | dst_a;
                    dst_pixels[x + dst_size.width * y] = color;
                }
            }
        }
    }
    hw_surface_unlock(destination);
    hw_surface_unlock(source);
    return 0;
}

