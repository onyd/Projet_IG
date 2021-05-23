#include "draw.h"
#include "ei_utils.h"
#include "geometry.h"
#include "defaults.h"
#include "ei_application.h"

void draw_window() {
    ei_widget_t *root = ei_app_root_widget();
    root->wclass->drawfunc(root, get_main_window(), get_pick_surface(), get_clipper_window());
}

void draw_rectangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper) {
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

    ei_draw_polygon(surface, first_point, color, clipper);
}

void draw_rect_triangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, direction dir) {
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

    ei_draw_polygon(surface, first_point, color, clipper);
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
        ei_rect_t src_rect = ei_rect(ei_point_zero(), dst_rect.size);
        ei_copy_surface(surface, clipper, img, &src_rect, EI_TRUE);
    }
}

void draw_cross(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int32_t size) {
    int d = (sqrt(2) / 2) * size;

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

void draw_blank_rect(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int32_t w, int32_t d) {
    ei_linked_point_t first_point[7];
    ei_linked_point_t *current = first_point;
    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y;
    current->next = &first_point[1];
    current->point.x = rect.top_left.x + d;
    current->point.y = rect.top_left.y;
    current->next = &first_point[2];
    current->point.x = rect.top_left.x + d + w;
    current->point.y = rect.top_left.y;
    current->next = &first_point[3];
    current->point.x = rect.top_left.x + rect.size.width;
    current->point.y = rect.top_left.y;
    current->next = &first_point[4];
    current->point.x = rect.top_left.x + rect.size.width;
    current->point.y = rect.top_left.y + rect.size.height;
    current->next = &first_point[5];
    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y + rect.size.height;
    current->next = &first_point[6];
    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y;
    current->next = NULL;

    ei_draw_polyline(surface, first_point, color, clipper);
}

uint8_t cohen_sutherland_code(ei_point_t p, ei_rect_t *clipper) {
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
    }
}

ei_bool_t line_analytic_clipping(ei_point_t p1, ei_point_t p2, ei_point_t *clipped1, ei_point_t *clipped2, float *error,
                                 ei_rect_t *clipper) {
    *error += 0;
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

    int x_min = clipper->top_left.x;
    int x_max = clipper->top_left.x + clipper->size.width;
    int y_min = clipper->top_left.y;
    int y_max = clipper->top_left.y + clipper->size.height;

    float e = 0;
    switch (get_clipping_type(c1)) {
        case center_reject:
            *clipped1 = p1;
            break;
        case north_east_reject:
            // Try horizontal intersection_rects
            e = horizontal_line_intersection_rect(p1, p2, y_min, clipped1);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1->x < x_min || clipped1->x > x_max) {
                e = vertical_line_intersection_rect(p1, p2, x_max, clipped1);
            }
            *error += e;
            return true; // Corner implies other point is in the clipper
        case north_west_reject:
            // Try horizontal intersection_rects
            e = horizontal_line_intersection_rect(p1, p2, y_min, clipped1);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1->x < x_min || clipped1->x > x_max) {
                e = vertical_line_intersection_rect(p1, p2, x_min, clipped1);
            }
            return true; // Corner implies other point is in the clipper
        case south_east_reject:
            // Try horizontal intersection_rects
            e = horizontal_line_intersection_rect(p1, p2, y_max, clipped1);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1->x < x_min || clipped1->x > x_max) {
                e = vertical_line_intersection_rect(p1, p2, x_max, clipped1);
            }
            return true; // Corner implies other point is in the clipper
        case south_west_reject:
            // Try horizontal intersection_rects
            e = horizontal_line_intersection_rect(p1, p2, y_max, clipped1);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1->x < x_min || clipped1->x > x_max) {
                e = vertical_line_intersection_rect(p1, p2, x_min, clipped1);
            }
            return true; // Corner implies other point is in the clipper
        case north_reject:
            *error += horizontal_line_intersection_rect(p1, p2, y_min, clipped1);
        case south_reject:
            *error += horizontal_line_intersection_rect(p1, p2, y_max, clipped1);
            break;
        case east_reject:
            *error += vertical_line_intersection_rect(p1, p2, x_max, clipped1);
            break;
        case west_reject:
            *error += vertical_line_intersection_rect(p1, p2, x_min, clipped1);
            break;
    }
    *error += e;

    switch (get_clipping_type(c2)) {
        case center_reject:
            *clipped2 = p2;
            break;
        case north_east_reject:
            // Try horizontal intersection_rects
            e = horizontal_line_intersection_rect(p1, p2, y_min, clipped2);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2->x < x_min || clipped2->x > x_max) {
                e = vertical_line_intersection_rect(p1, p2, x_max, clipped2);
            }
            break;
        case north_west_reject:
            // Try horizontal intersection_rects
            e = horizontal_line_intersection_rect(p1, p2, y_min, clipped2);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2->x < x_min || clipped2->x > x_max) {
                e = vertical_line_intersection_rect(p1, p2, x_min, clipped2);
            }
            break;
        case south_east_reject:
            // Try horizontal intersection_rects
            e = horizontal_line_intersection_rect(p1, p2, y_max, clipped2);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2->x < x_min || clipped2->x > x_max) {
                e = vertical_line_intersection_rect(p1, p2, x_min, clipped2);
            }
            break;
        case south_west_reject:
            // Try horizontal intersection_rects
            e = horizontal_line_intersection_rect(p1, p2, y_max, clipped2);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2->x < x_min || clipped2->x > x_max) {
                e = vertical_line_intersection_rect(p1, p2, x_max, clipped2);
            }
            break;
        case north_reject:
            *error += horizontal_line_intersection_rect(p1, p2, y_min, clipped2);
        case south_reject:
            *error += horizontal_line_intersection_rect(p1, p2, y_max, clipped2);
            break;
        case east_reject:
            *error += vertical_line_intersection_rect(p1, p2, x_max, clipped2);
            break;
        case west_reject:
            *error += vertical_line_intersection_rect(p1, p2, x_min, clipped2);
            break;
    }

    *error += e;
    return true;
}

void polygon_analytic_clipping(ei_linked_point_t *points, ei_point_list_t *clipped,
                               ei_error_list_t *errors, ei_rect_t *clipper) {
    // Initialization
    ei_point_list_t input = {NULL};
    ei_error_list_t input_errors = {NULL};
    clipped->head = points;

    ei_point_t topleft = clipper->top_left;
    ei_point_t topright = ei_point(clipper->top_left.x + clipper->size.width, clipper->top_left.y);
    ei_point_t botleft = ei_point(clipper->top_left.x, clipper->top_left.y + clipper->size.height);
    ei_point_t botright = ei_point(clipper->top_left.x + clipper->size.width,
                                   clipper->top_left.y + clipper->size.height);
    // West
    input.head = clipped;
    clipped->head = NULL;
    errors->head = NULL;

    // Go through all polygon's edges
    ei_linked_point_t *previous = input.head;
    ei_linked_point_t *current = previous->next;
    while (current != NULL) {
        ei_point_t intersection;
        float e = vertical_line_intersection_rect(previous->point, current->point, topleft.x, &intersection);
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

    // South
    free_linked_point(input.head);
    input.head = clipped->head;
    input_errors.head = errors->head;
    clipped->head = NULL;
    errors->head = NULL;

    // Go through all polygon's edges
    previous = input.head;
    current = previous->next;
    ei_linked_error_t *last_error = input_errors.head;
    while (current != NULL) {
        ei_point_t intersection;
        float e = horizontal_line_intersection_rect(previous->point, current->point, botleft.y,
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

    // East
    free_linked_point(input.head);
    input.head = clipped->head;
    input_errors.head = errors->head;
    clipped->head = NULL;
    errors->head = NULL;

    // Go through all polygon's edges
    previous = input.head;
    current = previous->next;
    last_error = input_errors.head;
    while (current != NULL) {
        ei_point_t intersection;
        float e = vertical_line_intersection_rect(previous->point, current->point, topright.x,
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

    // North
    free_linked_point(input.head);
    input.head = clipped->head;
    input_errors.head = errors->head;
    clipped->head = NULL;
    errors->head = NULL;

    // Go through all polygon's edges
    previous = input.head;
    current = previous->next;
    last_error = input_errors.head;
    while (current != NULL) {
        ei_point_t intersection;
        float e = horizontal_line_intersection_rect(previous->point, current->point, topleft.y,
                                          &intersection);
        if (!is_left(current->point, botright, topleft)) {
            if (is_left(previous->point, botright, topleft)) {
                append_linked_point(intersection, clipped);
                append_linked_error(e + last_error->error, errors);
            }
            append_linked_point(current->point, clipped);
            append_linked_error(last_error->error, errors);
        } else if (!is_left(previous->point, botright, topleft)) {
            append_linked_point(intersection, clipped);
            append_linked_error(e + last_error->error, errors);
        }

        last_error = last_error->next;
        previous = current;
        current = current->next;
    }
}
