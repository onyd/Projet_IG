#include "geometry.h"
#include "math.h"
#include "ei_utils.h"
#include "stdlib.h"


ei_point_t *arc(const ei_point_t *c, float r, float start_angle, float end_angle, uint32_t N) {
    float da = (end_angle - start_angle) / N;

    struct ei *points = calloc(N, sizeof(ei_linked_point_t));
    float angle = start_angle;
    for (float i = 0; i < N; i++) {
        points[i] = ei_point(r * cos(angle*(pi/180)), r * sin(angle*(pi/180)));
        angle += da;
    }

    return points;
}



ei_linked_point_t *rounded_frame(ei_rect_t button_rect, int rayon) {
    int button_width = button_rect.size.width;
    int button_height = button_rect.size.height;
    int top_left_x = button_rect.top_left.x;
    int top_left_y = button_rect.top_left.y;
    ei_point_t point;
    point.x = top_left_x + rayon;
    point.y = top_left_y;
    ei_linked_point_t *prec, *button, *current = &arc(point, rayon, pi/2, pi);
    while (prec->next != NULL) {
        prec = prec->next;
    }
    point.x = top_left_x;
    point.y = top_left_y + button_height - rayon;
    current = &arc(point, rayon, pi, 3*pi/2);
    prec->next = current;
    prec = current;
    while (prec->next != NULL) {
        prec = prec->next;
    }
    point.x = top_left_x + button_width - rayon;
    point.y = top_left_y + button_height;
    current = &arc(point, rayon, 3*pi/2, 0);
    prec->next = current;
    prec = current;
    while (prec->next != NULL) {
        prec = prec->next;
    }
    point.x = top_left_x + button_width;
    point.y = top_left_y + rayon;
    current = &arc(point, rayon, 0, pi/2);
    prec->next = current;
    prec = current;
    while (prec->next != NULL) {
        prec = prec->next;
    }
    prec->next = button;
    return button;
}

