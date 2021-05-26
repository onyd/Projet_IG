#include "draw.h"
#include "ei_utils.h"
#include "geometry.h"
#include "defaults.h"
#include "ei_application.h"

void draw_window() {
    ei_widget_t *root = ei_app_root_widget();
    root->wclass->drawfunc(root, get_main_window(), get_pick_surface(), get_clipper_window());
}

void draw_rectangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, ei_bool_t for_screen) {
    ei_linked_point_t first_point[5];
    ei_linked_point_t *current = first_point;

    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y;
    current->next = &first_point[1];
    current = current->next;
    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y + rect.size.height;
    current->next = &first_point[2];
    current = current->next;
    current->point.x = rect.top_left.x + rect.size.width;
    current->point.y = rect.top_left.y + rect.size.height;
    current->next = &first_point[3];
    current = current->next;
    current->point.x = rect.top_left.x + rect.size.width;
    current->point.y = rect.top_left.y;
    current->next = &first_point[4];
    current = current->next;
    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y;
    current->next = NULL;

    if (for_screen) {
        ei_draw_polygon(surface, first_point, color, clipper);
    }
    else {
        draw_picking_polygon(surface, first_point, color, clipper);
    }
}

void draw_full_circle(ei_surface_t surface, ei_point_t center, float radius, ei_color_t color, ei_rect_t *clipper) {
    ei_linked_point_t *pts = arc(center, radius, 0, 360, 20);
    ei_draw_polygon(surface, pts, color, clipper);
    free_linked_point(pts);
}

void draw_rect_triangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, direction dir, ei_bool_t for_screen) {
    ei_linked_point_t first_point[4];
    ei_linked_point_t *current = first_point;

    if (dir == up) {
        current->point.x = rect.top_left.x;
        current->point.y = rect.top_left.y;
        current->next = &first_point[1];
        current = current->next;
        current->point.x = rect.top_left.x;
        current->point.y = rect.top_left.y + rect.size.height;
        current->next = &first_point[2];
        current = current->next;
        current->point.x = rect.top_left.x + rect.size.width;
        current->point.y = rect.top_left.y;
        current->next = &first_point[3];
        current = current->next;
        current->point.x = rect.top_left.x;
        current->point.y = rect.top_left.y;
    } else if (dir == down) {
        current->point.x = rect.top_left.x + rect.size.width;
        current->point.y = rect.top_left.y;
        current->next = &first_point[1];
        current = current->next;
        current->point.x = rect.top_left.x;
        current->point.y = rect.top_left.y + rect.size.height;
        current->next = &first_point[2];
        current = current->next;
        current->point.x = rect.top_left.x + rect.size.width;
        current->point.y = rect.top_left.y + rect.size.height;
        current->next = &first_point[3];
        current = current->next;
        current->point.x = rect.top_left.x + rect.size.width;
        current->point.y = rect.top_left.y;
    }
    current->next = NULL;

    if (for_screen) {
        ei_draw_polygon(surface, first_point, color, clipper);
    }
    else {
        draw_picking_polygon(surface, first_point, color, clipper);
    }
}

void draw_image(ei_surface_t surface, ei_surface_t img, ei_point_t *pos, ei_rect_t *img_rect, ei_rect_t *clipper) {

    ei_rect_t dst_rect;
    if (img_rect != NULL) {
        dst_rect = ei_rect(*pos, img_rect->size);
        // Crop the image in the clipper according to img_rect
        intersection_rect(&dst_rect, clipper, &dst_rect);
        ei_rect_t src_rect = ei_rect(img_rect->top_left, dst_rect.size);
        ei_copy_surface(surface, &dst_rect, img, &src_rect, EI_TRUE);
    } else {
        ei_rect_t src_rect = ei_rect(ei_point_zero(), clipper->size);
        ei_copy_surface(surface, clipper, img, &src_rect, EI_TRUE);
    }
}

void draw_cross(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int32_t size) {
    int d = (int) ((sqrt(2) / 2) * size);

    // First cross
    ei_linked_point_t first_point_1[5];
    ei_linked_point_t *current_1 = first_point_1;
    current_1->point.x = rect.top_left.x;
    current_1->point.y = rect.top_left.y + d;
    current_1->next = &first_point_1[1];
    current_1 = current_1->next;
    current_1->point.x = rect.top_left.x + d;
    current_1->point.y = rect.top_left.y;
    current_1->next = &first_point_1[2];
    current_1 = current_1->next;
    current_1->point.x = rect.top_left.x + rect.size.width;
    current_1->point.y = rect.top_left.y + rect.size.height - d;
    current_1->next = &first_point_1[3];
    current_1 = current_1->next;
    current_1->point.x = rect.top_left.x + rect.size.width - d;
    current_1->point.y = rect.top_left.y + rect.size.height;
    current_1->next = &first_point_1[4];
    current_1 = current_1->next;
    current_1->point.x = rect.top_left.x;
    current_1->point.y = rect.top_left.y + d;
    current_1->next = NULL;

    // Second cross
    ei_linked_point_t first_point_2[5];
    ei_linked_point_t *current_2 = first_point_2;
    current_2->point.x = rect.top_left.x + rect.size.width - d;
    current_2->point.y = rect.top_left.y;
    current_2->next = &first_point_2[1];
    current_2 = current_2->next;
    current_2->point.x = rect.top_left.x + rect.size.width;
    current_2->point.y = rect.top_left.y + d;
    current_2->next = &first_point_2[2];
    current_2 = current_2->next;
    current_2->point.x = rect.top_left.x + d;
    current_2->point.y = rect.top_left.y + rect.size.height;
    current_2->next = &first_point_2[3];
    current_2 = current_2->next;
    current_2->point.x = rect.top_left.x;
    current_2->point.y = rect.top_left.y + rect.size.height - d;
    current_2->next = &first_point_2[4];
    current_2 = current_2->next;
    current_2->point.x = rect.top_left.x + rect.size.width - d;
    current_2->point.y = rect.top_left.y;
    current_2->next = NULL;

    ei_draw_polygon(surface, first_point_1, color, clipper);
    ei_draw_polygon(surface, first_point_2, color, clipper);
}

void draw_topbar(ei_surface_t surface, ei_widget_t *widget, ei_color_t color, ei_rect_t *clipper) {
    ei_rect_t topbar = ei_rect(widget->screen_location.top_left, ei_size(widget->screen_location.size.width,
                                                                widget->screen_location.size.height - widget->content_rect->size.height));
    draw_rectangle(surface, topbar, color, clipper, EI_TRUE);
}

void draw_border_toplevel(ei_surface_t surface, ei_widget_t *widget, ei_color_t color, ei_rect_t *clipper) {
    ei_linked_point_t *points = calloc(9, sizeof(ei_linked_point_t));
    points[0].point = ei_point(widget->screen_location.top_left.x, widget->content_rect->top_left.y);
    points[0].next = &points[1];
    points[1].point = ei_point(widget->screen_location.top_left.x,
                               widget->screen_location.top_left.y + widget->screen_location.size.height);
    points[1].next = &points[2];
    points[2].point = ei_point(widget->screen_location.top_left.x + widget->screen_location.size.width,
                               widget->screen_location.top_left.y + widget->screen_location.size.height);
    points[2].next = &points[3];
    points[3].point = ei_point(widget->screen_location.top_left.x + widget->screen_location.size.width,
                               widget->content_rect->top_left.y);
    points[3].next = &points[4];
    points[4].point = ei_point(widget->content_rect->top_left.x + widget->content_rect->size.width,
                               widget->content_rect->top_left.y);
    points[4].next = &points[5];
    points[5].point = ei_point(widget->content_rect->top_left.x + widget->content_rect->size.width,
                               widget->content_rect->top_left.y + widget->content_rect->size.height);
    points[5].next = &points[6];
    points[6].point = ei_point(widget->content_rect->top_left.x,
                               widget->content_rect->top_left.y + widget->content_rect->size.height);
    points[6].next = &points[7];
    points[7].point = ei_point(widget->content_rect->top_left.x,widget->content_rect->top_left.y);
    points[7].next = &points[8];
    points[8].point = ei_point(widget->screen_location.top_left.x, widget->content_rect->top_left.y);
    points[8].next = NULL;
    ei_draw_polygon(surface, points, color, clipper);
    free(points);
}

void draw_blank_rect(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int32_t w, int32_t d) {
    ei_linked_point_t first_point[6];
    ei_linked_point_t *current = first_point;
    current->point.x = rect.top_left.x + d + w;
    current->point.y = rect.top_left.y;
    current->next = &first_point[1];
    current = current->next;

    current->point.x = rect.top_left.x + rect.size.width;
    current->point.y = rect.top_left.y;
    current->next = &first_point[2];
    current = current->next;

    current->point.x = rect.top_left.x + rect.size.width;
    current->point.y = rect.top_left.y + rect.size.height;
    current->next = &first_point[3];
    current = current->next;

    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y + rect.size.height;
    current->next = &first_point[4];
    current = current->next;

    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y;
    current->next = &first_point[5];
    current = current->next;

    current->point.x = rect.top_left.x + d;
    current->point.y = rect.top_left.y;
    current->next = NULL;

    ei_draw_polyline(surface, first_point, color, clipper);
}

uint8_t cohen_sutherland_code(ei_point_t p, const ei_rect_t *clipper) {
    uint8_t result = 0;

    result |= (p.x < clipper->top_left.x);
    result |= (p.x > clipper->top_left.x + clipper->size.width) << 1;
    result |= (p.y < clipper->top_left.y) << 2;
    result |= (p.y > clipper->top_left.y + clipper->size.height) << 3;

    return result;
}

enum clipping_type get_clipping_type(uint8_t code) {
    switch (code) {
        case 0:
            return center_reject;
        case 1:
            return west_reject;
        case 2:
            return east_reject;
        case 4:
            return north_reject;
        case 5:
            return north_west_reject;
        case 6:
            return north_east_reject;
        case 8:
            return south_reject;
        case 9:
            return south_west_reject;
        case 10:
            return south_east_reject;
        default:
            return -1;
    }
}

ei_bool_t line_analytic_clipping(ei_point_t p1, ei_point_t p2, ei_point_t *clipped1, ei_point_t *clipped2, float *error,
                                 const ei_rect_t *clipper) {
    *error = 0;
    if (clipper == NULL) {
        *clipped1 = p1;
        *clipped2 = p2;
        return true;
    }

    uint8_t c1 = cohen_sutherland_code(p1, clipper);
    uint8_t c2 = cohen_sutherland_code(p2, clipper);

    // Trivial case
    if ((c1 | c2) == 0) {
        *clipped1 = p1;
        *clipped2 = p2;
        return true;
    } else if ((c1 & c2) != 0) {
        return false;
    }

    float x_min = (float) clipper->top_left.x;
    float x_max = (float) clipper->top_left.x + (float) clipper->size.width;
    float y_min = (float) clipper->top_left.y;
    float y_max = (float) clipper->top_left.y + (float) clipper->size.height;
    float clipped1_x_f = (float) clipped1->x;
    float clipped2_x_f = (float) clipped2->x;

    switch (get_clipping_type(c1)) {
        case center_reject:
            *clipped1 = p1;
            break;
        case north_east_reject:
            // Try horizontal intersection_rects
            *error = horizontal_line_intersection_rect(p1, p2, y_min, clipped1);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1_x_f < x_min || clipped1_x_f > x_max) {
                *error = vertical_line_intersection_rect(p1, p2, x_max, clipped1);
            }
            return true; // Corner implies other point is in the clipper
        case north_west_reject:
            // Try horizontal intersection_rects
            *error = horizontal_line_intersection_rect(p1, p2, y_min, clipped1);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1_x_f < x_min || clipped1_x_f > x_max) {
                *error = vertical_line_intersection_rect(p1, p2, x_min, clipped1);
            }
            return true; // Corner implies other point is in the clipper
        case south_east_reject:
            // Try horizontal intersection_rects
            *error = horizontal_line_intersection_rect(p1, p2, y_max, clipped1);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1_x_f < x_min || clipped1_x_f > x_max) {
                *error = vertical_line_intersection_rect(p1, p2, x_max, clipped1);
            }
            return true; // Corner implies other point is in the clipper
        case south_west_reject:
            // Try horizontal intersection_rects
            *error = horizontal_line_intersection_rect(p1, p2, y_max, clipped1);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1_x_f < x_min || clipped1_x_f > x_max) {
                *error = vertical_line_intersection_rect(p1, p2, x_min, clipped1);
            }
            return true; // Corner implies other point is in the clipper
        case north_reject:
            *error = horizontal_line_intersection_rect(p1, p2, y_min, clipped1);
        case south_reject:
            *error = horizontal_line_intersection_rect(p1, p2, y_max, clipped1);
            break;
        case east_reject:
            *error = vertical_line_intersection_rect(p1, p2, x_max, clipped1);
            break;
        case west_reject:
            *error = vertical_line_intersection_rect(p1, p2, x_min, clipped1);
            break;
    }

    switch (get_clipping_type(c2)) {
        case center_reject:
            *clipped2 = p2;
            break;
        case north_east_reject:
            // Try horizontal intersection_rects
            horizontal_line_intersection_rect(p1, p2, y_min, clipped2);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2_x_f < x_min || clipped2_x_f > x_max) {
                vertical_line_intersection_rect(p1, p2, x_max, clipped2);
            }
            break;
        case north_west_reject:
            // Try horizontal intersection_rects
            horizontal_line_intersection_rect(p1, p2, y_min, clipped2);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2_x_f < x_min || clipped2_x_f > x_max) {
                vertical_line_intersection_rect(p1, p2, x_min, clipped2);
            }
            break;
        case south_east_reject:
            // Try horizontal intersection_rects
            horizontal_line_intersection_rect(p1, p2, y_max, clipped2);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2_x_f < x_min || clipped2_x_f > x_max) {
                vertical_line_intersection_rect(p1, p2, x_min, clipped2);
            }
            break;
        case south_west_reject:
            // Try horizontal intersection_rects
            horizontal_line_intersection_rect(p1, p2, y_max, clipped2);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2_x_f < x_min || clipped2_x_f > x_max) {
                vertical_line_intersection_rect(p1, p2, x_max, clipped2);
            }
            break;
        case north_reject:
            horizontal_line_intersection_rect(p1, p2, y_min, clipped2);
        case south_reject:
            horizontal_line_intersection_rect(p1, p2, y_max, clipped2);
            break;
        case east_reject:
            vertical_line_intersection_rect(p1, p2, x_max, clipped2);
            break;
        case west_reject:
            vertical_line_intersection_rect(p1, p2, x_min, clipped2);
            break;
    }

    return true;
}

void polygon_analytic_clipping(const ei_linked_point_t *points, ei_point_list_t *clipped,
                               ei_error_list_t *errors, const ei_rect_t *clipper) {
    // Initialization
    ei_point_list_t input = {NULL};
    ei_error_list_t input_errors = {NULL};
    clipped->head = (ei_linked_point_t *) points;

    ei_point_t topleft = clipper->top_left;
    ei_point_t topright = ei_point(clipper->top_left.x + clipper->size.width, clipper->top_left.y);
    ei_point_t botleft = ei_point(clipper->top_left.x, clipper->top_left.y + clipper->size.height);
    ei_point_t botright = ei_point(clipper->top_left.x + clipper->size.width,
                                   clipper->top_left.y + clipper->size.height);
    // West
    input.head = clipped->head;
    clipped->head = NULL;
    errors->head = NULL;

    // Go through all polygon's edges
    ei_linked_point_t *previous = input.head;
    ei_linked_point_t *current = (previous != NULL) ? previous->next : NULL;
    while (current != NULL) {
        ei_point_t intersection;
        float e = vertical_line_intersection_rect(previous->point, current->point, (float) topleft.x, &intersection);
        if (!is_left(current->point, topleft, botleft)) {
            if (is_left(previous->point, topleft, botleft)) {
                append_linked_point(intersection, clipped);
                append_linked_error(e, errors);
            }
            append_linked_point(current->point, clipped);
            append_linked_error(0.0f, errors);
        } else if (!is_left(previous->point, topleft, botleft)) {
            append_linked_point(intersection, clipped);
            append_linked_error(e, errors);
        }

        previous = current;
        current = current->next;
    }

    if (clipped->head != NULL) {
        append_linked_point(clipped->head->point, clipped);
    }

    // South
    //free_linked_point(input.head); don't free args points
    input.head = clipped->head;
    input_errors.head = errors->head;
    clipped->head = NULL;
    errors->head = NULL;

    // Go through all polygon's edges
    previous = input.head;
    current = (previous != NULL) ? previous->next : NULL;
    ei_linked_error_t *last_error = input_errors.head;
    while (current != NULL) {
        ei_point_t intersection;
        float e = horizontal_line_intersection_rect(previous->point, current->point, (float) botleft.y,
                                                    &intersection);
        if (!is_left(current->point, botleft, botright)) {
            if (is_left(previous->point, botleft, botright)) {
                append_linked_point(intersection, clipped);
                append_linked_error(e + last_error->error, errors);
            }
            append_linked_point(current->point, clipped);
            append_linked_error(last_error->error, errors);
        } else if (!is_left(previous->point, botleft, botright)) {
            append_linked_point(intersection, clipped);
            append_linked_error(e + last_error->error, errors);
        }

        last_error = last_error->next;
        previous = current;
        current = current->next;
    }

    if (clipped->head != NULL) {
        append_linked_point(clipped->head->point, clipped);
    }

    // East
    free_linked_point(input.head);
    free_linked_error(input_errors.head);
    input.head = clipped->head;
    input_errors.head = errors->head;
    clipped->head = NULL;
    errors->head = NULL;

    // Go through all polygon's edges
    previous = input.head;
    current = (previous != NULL) ? previous->next : NULL;
    last_error = input_errors.head;
    while (current != NULL) {
        ei_point_t intersection;
        float e = vertical_line_intersection_rect(previous->point, current->point, (float) topright.x,
                                                  &intersection);
        if (!is_left(current->point, botright, topright)) {
            if (is_left(previous->point, botright, topright)) {
                append_linked_point(intersection, clipped);
                append_linked_error(e + last_error->error, errors);
            }
            append_linked_point(current->point, clipped);
            append_linked_error(last_error->error, errors);
        } else if (!is_left(previous->point, botright, topright)) {
            append_linked_point(intersection, clipped);
            append_linked_error(e + last_error->error, errors);
        }

        last_error = last_error->next;
        previous = current;
        current = current->next;
    }

    if (clipped->head != NULL) {
        append_linked_point(clipped->head->point, clipped);
    }

    // North
    free_linked_point(input.head);
    free_linked_error(input_errors.head);
    input.head = clipped->head;
    input_errors.head = errors->head;
    clipped->head = NULL;
    errors->head = NULL;

    // Go through all polygon's edges
    previous = input.head;
    current = (previous != NULL) ? previous->next : NULL;
    last_error = input_errors.head;
    while (current != NULL) {
        ei_point_t intersection;
        float e = horizontal_line_intersection_rect(previous->point, current->point, (float) topleft.y,
                                                    &intersection);
        if (!is_left(current->point, topright, topleft)) {
            if (is_left(previous->point, topright, topleft)) {
                append_linked_point(intersection, clipped);
                append_linked_error(e + last_error->error, errors);
            }
            append_linked_point(current->point, clipped);
            append_linked_error(last_error->error, errors);
        } else if (!is_left(previous->point, topright, topleft)) {
            append_linked_point(intersection, clipped);
            append_linked_error(e + last_error->error, errors);
        }

        last_error = last_error->next;
        previous = current;
        current = current->next;
    }

    if (clipped->head != NULL) {
        append_linked_point(clipped->head->point, clipped);
    }
    free_linked_point(input.head);
    free_linked_error(input_errors.head);
}

void draw_picking_polygon(ei_surface_t surface,
                     const ei_linked_point_t *first_point,
                     ei_color_t color,
                     const ei_rect_t *clipper) {
    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    uint32_t c = ei_map_rgba(surface, color);

    // Clipping
    ei_point_list_t clipped;
    ei_error_list_t errors;
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
            edge->E = (int) (error->error * abs(dx));
            edge->sign_dx = (dx > 0) ? 1 : -1;
            edge->next = TC[p_min->point.y - ymin];
            TC[p_min->point.y - ymin] = edge;
        }
        first = second;
        second = second->next;
        error = error->next;
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
                pixels[k + size.width * y] = c;
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
    free_linked_point(clipped.head);
    free_linked_error(errors.head);
    lae_free(TCA);
    free(TC);
}

