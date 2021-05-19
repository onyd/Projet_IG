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
        intersection(&dst_rect, clipper, &dst_rect);
    } else {
        dst_rect = *clipper;
    }

    ei_rect_t src_rect = ei_rect(ei_point_zero(), dst_rect.size);
    ei_copy_surface(surface, &dst_rect, img, &src_rect, EI_TRUE);
}

void draw_cross(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int32_t size){
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