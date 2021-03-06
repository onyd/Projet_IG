#include "ei_draw.h"
#include "utils.h"
#include "stdio.h"
#include <math.h>
#include "stdbool.h"
#include <stdlib.h>
#include "ei_utils.h"
#include "geometry.h"
#include "draw.h"

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


    // Draw all lines between points
    while (first != NULL && second != NULL) {
        int dx = second->point.x - first->point.x;
        int dy = second->point.y - first->point.y;

        // Clipping
        ei_point_t clipped1, clipped2;
        float error;
        if (!line_analytic_clipping(first->point, second->point, &clipped1, &clipped2, &error, clipper)) {
            first = second;
            second = second->next;
            continue;
        }

        int x = clipped1.x;
        int y = clipped1.y;


        /* Bresenham */
        int sign_dx = (dx > 0) ? 1 : -1;
        int sign_dy = (dy > 0) ? 1 : -1;

        // Vertical and horizontal case
        if (dx == 0) {
            while (sign_dy * y <= sign_dy * second->point.y) {
                pixels[x + size.width * y] = c;
                y += sign_dy;
            }
            first = second;
            second = second->next;
            continue;
        }
        if (dy == 0) {
            while (sign_dx * x <= sign_dx * second->point.x) {
                pixels[x + size.width * y] = c;
                x += sign_dx;
            }
            first = second;
            second = second->next;
            continue;
        }

        // Swap variable for y-directed line
        bool swapped = abs(dx) < abs(dy);
        int x2 = clipped2.x;
        int y2 = clipped2.y;

        if (swapped) {
            swap(&x, &y);
            swap(&dx, &dy);
            swap(&sign_dx, &sign_dy);
            swap(&x2, &y2);
        }
        int E = (int) (error * abs(dx));
        while (sign_dx * x < sign_dx * x2) {
            x += sign_dx;
            E += abs(dy);

            if (2 * E > abs(dx)) {
                y += sign_dy;
                E -= abs(dx);
            }

            // Draw pixel in the buffer
            if (!swapped) {
                pixels[x + size.width * y] = c;
            } else {
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

    // Clipping
    ei_point_list_t clipped = {NULL, NULL};
    ei_error_list_t errors = {NULL, NULL};
    if (clipper == NULL) {
        clipped.head = (ei_linked_point_t *) first_point;
    } else {
        polygon_analytic_clipping(first_point, &clipped, &errors, clipper);
        if (clipped.head == NULL) {
            return; // Totally out, we skip it
        }
    }

    // Find ymax and ymin to initialize size
    int ymax = clipped.head->point.y;
    int ymin = clipped.head->point.y;

    ei_linked_point_t *current = clipped.head;
    while (current != NULL) {
        if (ymax < current->point.y) {
            ymax = current->point.y;
        } else if (ymin > current->point.y) {
            ymin = current->point.y;
        }
        current = current->next;
    }

    // TC/TCA initialization
    linked_edges **TC = calloc(ymax - ymin, sizeof(linked_edges *));
    linked_acive_edges *TCA = malloc(sizeof(linked_acive_edges));
    TCA->head = NULL;

    // Build TC
    ei_linked_point_t *first = clipped.head;
    ei_linked_point_t *second = first->next;
    ei_linked_error_t *error = errors.head;

    while (second != NULL) {
        ei_linked_point_t *p_min = y_argmin(first, second);
        ei_linked_point_t *p_max = y_argmax(first, second);

        int dx = p_max->point.x - p_min->point.x;
        int dy = p_max->point.y - p_min->point.y;
        // Ignore horizontal edges
        if (dy != 0) {
            linked_edges *edge = malloc(sizeof(linked_edges));
            edge->ymax = p_max->point.y;
            edge->x_ymin = p_min->point.x;
            edge->x_ymax = p_max->point.x;
            edge->dx = dx;
            edge->dy = dy;
            edge->E = (error != NULL) ? (int) (error->error * abs(dx)) : 0;
            edge->sign_dx = (dx > 0) ? 1 : -1;
            edge->next = TC[p_min->point.y - ymin];
            TC[p_min->point.y - ymin] = edge;
        }
        first = second;
        second = second->next;
        if (error != NULL) {
            error = error->next;
        }
    }

    /* Fill polygon for every scanline */
    int y = ymin;
    while (y < ymax) {
        uint32_t i = y - ymin;

        // Delete TCA edges such that y = ymax
        delete_y(y, TCA);

        // Add current scanline starting edges while keeping TCA sorted
        while (TC[i] != NULL) {
            linked_edges *tc = TC[i];
            TC[i] = tc->next;
            tc->next = NULL;
            sorting_insert(tc, TCA);
        }

        // Draw pixels
        linked_edges *current = TCA->head;
        while (current != NULL && current->next != NULL) {
            int x1 = current->x_ymin;
            current = current->next;
            int x2 = current->x_ymin;
            for (int k = x1; k < x2; k++) {
                if (color.alpha == 255) {
                    pixels[k + size.width * y] = c;
                }
                else {
                    ei_color_t color_transparancy = ei_map_rgba_inverse(surface, pixels[k + size.width * y]);
                    color_transparancy.red = (color.alpha * color.red + (255 - color.alpha) * color_transparancy.red)/255;
                    color_transparancy.green = (color.alpha * color.green + (255 - color.alpha) * color_transparancy.green)/255;
                    color_transparancy.blue = (color.alpha * color.blue + (255 - color.alpha) * color_transparancy.blue)/255;
                    color_transparancy.alpha = color.alpha;
                    pixels[k + size.width * y] = ei_map_rgba(surface, color_transparancy);
                }
            }
            current = current->next;
        }

        y++;

        // Bresenham iterations for next intersection
        current = TCA->head;
        while (current != NULL) {
            // y-directed
            if (abs(current->dx) <= current->dy) {
                current->E += abs(current->dx);
                if (2 * current->E > current->dy) {
                    current->x_ymin += current->sign_dx;
                    current->E -= current->dy;
                }
            } else {                 // x-directed
                while (true) {
                    current->E += current->dy;
                    current->x_ymin += current->sign_dx;
                    if (2 * current->E > abs(current->dx)) {
                        current->E -= abs(current->dx);
                        break; // Stop when we change of scanline
                    }
                }
            }
            current = current->next;
        }
    }
    if (clipper != NULL) {
        free_linked_point(clipped.head);
    }
    free_linked_error(errors.head);
    lae_free(TCA);
    free(TC);
}

void ei_draw_text(ei_surface_t surface,
                  const ei_point_t *where,
                  const char *text,
                  ei_font_t font,
                  ei_color_t color,
                  const ei_rect_t *clipper) {
    font = (font == NULL) ? ei_default_font : font;
    ei_surface_t new_surface = hw_text_create_surface(text, font, color);
    const ei_rect_t src_rect = hw_surface_get_rect(new_surface);

    ei_size_t size_dst_rect = hw_surface_get_size(new_surface);
    const ei_rect_t dst_rect = ei_rect(*where, size_dst_rect);

    if (clipper != NULL) {
        ei_rect_t intersection_rect_rect;
        if (intersection_rect(&dst_rect, clipper, &intersection_rect_rect)) {
            const ei_rect_t dst_rect_clipped = ei_rect(
                    ei_point(intersection_rect_rect.top_left.x, intersection_rect_rect.top_left.y),
                    ei_size(intersection_rect_rect.size.width, intersection_rect_rect.size.height));
            intersection_rect_rect.top_left.x = intersection_rect_rect.top_left.x - (where->x);
            intersection_rect_rect.top_left.y = intersection_rect_rect.top_left.y - (where->y);

            ei_copy_surface(surface, &dst_rect_clipped, new_surface, &intersection_rect_rect, true);
        }
    } else {
        ei_copy_surface(surface, &dst_rect, new_surface, &src_rect, true);
    }
    hw_surface_free(new_surface);
}

void ei_fill(ei_surface_t surface,
             const ei_color_t *color,
             const ei_rect_t *clipper) {

    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    uint32_t c = ei_map_rgba(surface, *color);
    for (int y = 0; y < size.height; y++) {
        for (int x = 0; x < size.width; x++) {
            if (inside(ei_point(x, y), clipper)) {
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

    // If it's not the same size between both surfaces or rect then failure
    if ((dst_rect == NULL || src_rect == NULL) &&
        (src_size.width != dst_size.width && src_size.height != dst_size.height)) {
        return 1;
    }
    if ((src_rect != NULL && dst_rect != NULL) && (src_rect->size.width != dst_rect->size.width &&
                                                   src_rect->size.height != dst_rect->size.height)) {
        return 1;
    }

    uint32_t *dst_pixels = (uint32_t *) hw_surface_get_buffer(destination);
    uint32_t *src_pixels = (uint32_t *) hw_surface_get_buffer(source);
    // Parameters for the index of the source and the destination and for the clipping
    int src_first_x, src_first_y, src_size_x, src_size_y;
    int dst_first_x, dst_first_y;
    if (src_rect == NULL) {
        src_first_x = 0;
        src_first_y = 0;
        src_size_x = src_size.width;
        src_size_y = src_size.height;
    } else {
        src_first_x = src_rect->top_left.x;
        src_first_y = src_rect->top_left.y;
        src_size_x = src_rect->size.width;
        src_size_y = src_rect->size.height;
    }
    if (dst_rect == NULL) {
        dst_first_x = 0;
        dst_first_y = 0;
    } else {
        dst_first_x = dst_rect->top_left.x;
        dst_first_y = dst_rect->top_left.y;
    }
    hw_surface_lock(destination);
    hw_surface_lock(source);
    int y1 = src_first_y;
    int y2 = dst_first_y;
    while (y1 < src_first_y + src_size_y) {
        int x1 = src_first_x;
        int x2 = dst_first_x;
        while (x1 < src_first_x + src_size_x) {
            // Draw pixel in the buffer
            if (!alpha) {
                dst_pixels[x2 + dst_size.width * y2] = src_pixels[x1 + src_size.width * y1];
            } else {
                int ir, ig, ib, ia;
                hw_surface_get_channel_indices(destination, &ir, &ig, &ib, &ia);
                ia = 6 - (ir + ib + ig);
                uint32_t src_r, src_g, src_b, src_a = 0;
                uint32_t dst_r, dst_g, dst_b = 0;
                uint32_t dst_a = 255 << 8 * ia;
                //We take the RGB color of the source and the destination
                src_r = (uint8_t) (src_pixels[x1 + src_size.width * y1] >> (8 * ir));
                src_g = (uint8_t) (src_pixels[x1 + src_size.width * y1] >> (8 * ig));
                src_b = (uint8_t) (src_pixels[x1 + src_size.width * y1] >> (8 * ib));
                src_a = (uint8_t) (src_pixels[x1 + src_size.width * y1] >> (8 * ia));
                dst_r = (uint8_t) (dst_pixels[x2 + dst_size.width * y2] >> (8 * ir));
                dst_g = (uint8_t) (dst_pixels[x2 + dst_size.width * y2] >> (8 * ig));
                dst_b = (uint8_t) (dst_pixels[x2 + dst_size.width * y2] >> (8 * ib));
                // apply the transparancy
                dst_r = ((src_a * src_r + (255 - src_a) * dst_r) / 255) << (8 * ir);
                dst_g = ((src_a * src_g + (255 - src_a) * dst_g) / 255) << (8 * ig);
                dst_b = ((src_a * src_b + (255 - src_a) * dst_b) / 255) << (8 * ib);
                uint32_t color = dst_r | dst_g | dst_b | dst_a;
                dst_pixels[x2 + dst_size.width * y2] = color;
            }
            x1++;
            x2++;
        }
        y1++;
        y2++;
    }
    hw_surface_unlock(destination);
    hw_surface_unlock(source);
    return 0;
}

