#include "draw.h"
#include "ei_utils.h"
#include "geometry.h"

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

void draw_rect_triangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int direction) {
    ei_linked_point_t first_point[4];
    ei_linked_point_t *current = first_point;

    if (direction == 0) {
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
    } else {
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
    } else {
        dst_rect = *clipper;
    }

    ei_rect_t src_rect = ei_rect(ei_point_zero(), dst_rect.size);
    ei_copy_surface(surface, &dst_rect, img, &src_rect, EI_TRUE);
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

uint8_t cohen_sutherland_code(ei_point_t p, ei_rect_t *clipper) {
    uint8_t result = 15;

    result &= (p.x < clipper->top_left.x);
    result &= (p.x > clipper->top_left.x + clipper->size.width) << 1;
    result &= (p.y < clipper->top_left.y) << 2;
    result &= (p.y > clipper->top_left.y + clipper->size.height) << 3;

    return result;
}


enum clipping_code get_clipping_code(uint8_t code) {
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

ei_bool_t analytic_clipping(ei_point_t p1, ei_point_t p2, ei_point_t *clipped1, ei_point_t *clipped2, float *E,
                       ei_rect_t *clipper) {
    uint8_t c1 = cohen_sutherland_code(p1, clipper);
    uint8_t c2 = cohen_sutherland_code(p2, clipper);
    *E = 0;

    // Trivial case
    if (c1 | c2 == 0) {
        clipped1 = &p1;
        clipped2 = &p2;
        *E = 0;
        return true;
    } else if (c1 & c2 != 0) {
        return false;
    }

    switch (get_clipping_code(c1)) {
        case center_reject:
            clipped1 = &p1;
            break;
        case north_east_reject:
        case north_west_reject:
        case south_east_reject:
        case south_west_reject:
            // Try horizontal intersection_rects
            *clipped1 = horizontal_line_intersection_rect(p1, p2, clipper->top_left.y, E);
            
            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped1->x < clipper->top_left.x || clipped1->x > clipper->top_left.x + clipper->size.width) {
                *clipped1 = vertical_line_intersection_rect(p1, p2, clipper->top_left.x, E);
            }
            return true; // Corner implies other point is in the clipper
        case north_reject:
        case south_reject:
            *clipped1 = horizontal_line_intersection_rect(p1, p2, clipper->top_left.y, E);
            break;
        case east_reject:
        case west_reject:
            *clipped1 = vertical_line_intersection_rect(p1, p2, clipper->top_left.x, E);
            break;
    }

    switch (get_clipping_code(c2)) {
        case center_reject:
            clipped2 = &p2;
            break;
        case north_east_reject:
        case north_west_reject:
        case south_east_reject:
        case south_west_reject:
            // Try horizontal intersection_rects
            *clipped2 = horizontal_line_intersection_rect(p1, p2, clipper->top_left.y, E);

            // Bad horizontal intersection_rect => vertical intersection_rect
            if (clipped2->x < clipper->top_left.x || clipped2->x > clipper->top_left.x + clipper->size.width) {
                *clipped2 = vertical_line_intersection_rect(p1, p2, clipper->top_left.x, E);
            }
            break;
        case north_reject:
        case south_reject:
            *clipped2 = horizontal_line_intersection_rect(p1, p2, clipper->top_left.y, E);
            break;
        case east_reject:
        case west_reject:
            *clipped2 = vertical_line_intersection_rect(p1, p2, clipper->top_left.x, E);
            break;
    }
    return true;
}