#include "geometry.h"
#include "math.h"
#include "stdlib.h"


ei_linked_point_t *arc(const ei_point_t *c, uint32_t r, float start_angle, float end_angle, uint32_t N) {
    float da = (end_angle - start_angle) / N;

    ei_linked_point_t *points = calloc(N, sizeof(ei_linked_point_t));
    float angle = start_angle;
    points[0].point.x = c->x + r * cos(angle * (pi / 180));
    points[0].point.y = c->y - r * sin(angle * (pi / 180));

    uint32_t i;
    for (i = 1; i < N; i++) {
        angle += da;
        points[i].point.x = c->x + r * cos(angle * (pi / 180));
        points[i].point.y = c->y - r * sin(angle * (pi / 180));

        points[i-1].next = &points[i];
    }
    points[i-1].next = NULL;

    return points;
}


ei_linked_point_t *rounded_frame(ei_rect_t button_rect, uint32_t radius, uint32_t N) {
    int button_width = button_rect.size.width;
    int button_height = button_rect.size.height;
    int top_left_x = button_rect.top_left.x;
    int top_left_y = button_rect.top_left.y;
    ei_point_t point;

    point.x = top_left_x + radius;
    point.y = top_left_y + radius;
    ei_linked_point_t  *current = arc(&point, radius, 90, 180, N);
    ei_linked_point_t *previous = current;
    ei_linked_point_t *button = current;

    // Find last arc point
    while (previous->next != NULL) {
        previous = previous->next;
    }

    point.x = top_left_x + radius;
    point.y = top_left_y + button_height - radius;
    current = arc(&point, radius, 180, 270, N);
    previous->next = current;
    previous = current;
    while (previous->next != NULL) {
        previous = previous->next;
    }

    point.x = top_left_x + button_width - radius;
    point.y = top_left_y + button_height - radius;
    current = arc(&point, radius, 270, 360, N);
    previous->next = current;
    previous = current;
    while (previous->next != NULL) {
        previous = previous->next;
    }

    point.x = top_left_x + button_width - radius;
    point.y = top_left_y + radius;
    current = arc(&point, radius, 0, 90, N);
    previous->next = current;
    previous = current;
    while (previous->next != NULL) {
        previous = previous->next;
    }

    //previous->next = button;
    return button;
}

