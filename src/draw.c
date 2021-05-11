#include "draw.h"
#include "stdlib.h"

void draw_rectangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper) {
    ei_linked_point_t *first_point = calloc(5, sizeof(ei_linked_point_t));
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
    free(first_point);
}

void rect_to_triangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int direction) {
    if (direction == 0) {
        ei_linked_point_t *first_point = calloc(4, sizeof(ei_linked_point_t));
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
        current->point.y = rect.top_left.y;
        current->next = &first_point[3];
        current = current->next;
        current->point.x = rect.top_left.x;
        current->point.y = rect.top_left.y;
        current->next = NULL;
        ei_draw_polygon(surface, first_point, color, clipper);
        free(first_point);
    } else {
        ei_linked_point_t *first_point = calloc(4, sizeof(ei_linked_point_t));
        ei_linked_point_t *current = first_point;
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
        current->next = NULL;
        ei_draw_polygon(surface, first_point, color, clipper);
        free(first_point);
    }
}

