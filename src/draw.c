#include "draw.h"
#include "stdlib.h"

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

void draw_image(ei_surface_t surface, ei_surface_t img, ei_point_t *pos, ei_rect_t *clipper) {
    uint32_t *img_buffer = (uint32_t *) hw_surface_get_buffer(img);
    ei_size_t img_size = hw_surface_get_size(img);

    uint32_t *buffer = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(img);

    // Clipper coordinates
    int top_left_x, top_right_x, top_left_y, bottom_left_y;
    if (clipper != NULL) {
        top_left_x = clipper->top_left.x;
        top_right_x = clipper->top_left.x + clipper->size.width;
        top_left_y = clipper->top_left.y;
        bottom_left_y = clipper->top_left.y + clipper->size.height;
    }

    for (uint32_t x = 0; x < img_size.width; x++) {
        for (uint32_t y = 0; y < img_size.height; y++) {
            if (clipper == NULL || (pos->x + x >= top_left_x &&
                                    pos->x + x <= top_right_x &&
                                    pos->y + y >= top_left_y &&
                                    pos->y + y <= bottom_left_y)) {
                buffer[pos->x + x + (pos->y + y) * size.width] = img_buffer[x + y * img_size.width];
            }
        }
    }
}