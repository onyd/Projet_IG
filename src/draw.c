#include "draw.h"

void draw_rectangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper) {
    ei_linked_point_t *first_point = malloc(5*sizeof(ei_linked_point_t));
    ei_linked_point_t *current = first_point;
    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y;
    current = current->next;
    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y + rect.size.height;
    current = current->next;
    current->point.x = rect.top_left.x + rect.size.width;
    current->point.y = rect.top_left.y + rect.size.height;
    current = current->next;
    current->point.x = rect.top_left.x + rect.size.width;
    current->point.y = rect.top_left.y;
    current = current->next;
    current->point.x = rect.top_left.x;
    current->point.y = rect.top_left.y;
    current->next = NULL;
    ei_draw_polygon(surface, first_point, color, clipper);
    free(first_point);
}

