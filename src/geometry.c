#include "geometry.h"

/*
ei_linked_point_t arc(const ei_point_t* c, uint16_t start_angle, uint16_t end_angle) {

}
 */


ei_linked_point_t *rounded_frame(ei_rect_t button_rect, int rayon) {
    float pi = 3.1415927;
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

