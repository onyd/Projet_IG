#include "geometry.h"
#include "math.h"
#include "stdlib.h"
#include "ei_utils.h"
#include <stdbool.h>
#include "stdio.h"
#include "ei_draw.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) < (b)) ? (a) : (b))


ei_linked_point_t *arc(const ei_point_t *c, uint32_t r, float start_angle, float end_angle, uint32_t N) {
    float da = (end_angle - start_angle) / (N - 1);

    ei_linked_point_t *points = calloc(N, sizeof(ei_linked_point_t));
    float angle = start_angle;
    points[0].point.x = c->x + r * cos(angle * (pi / 180.0f));
    points[0].point.y = c->y - r * sin(angle * (pi / 180.0f));

    uint32_t i;
    for (i = 1; i < N; i++) {
        angle += da;
        points[i].point.x = c->x + r * cos(angle * (pi / 180.0f));
        points[i].point.y = c->y - r * sin(angle * (pi / 180.0f));

        points[i - 1].next = &points[i];
    }
    points[i - 1].next = NULL;

    return points;
}


ei_linked_point_t *rounded_frame(ei_rect_t button_rect, uint32_t radius, uint32_t N, int param) {
    // si param = 0, on génère tout le boutton, s'il est égale à 1 on génère la parie haute et sinon la partie basse
    int button_width = button_rect.size.width;
    int button_height = button_rect.size.height;
    int top_left_x = button_rect.top_left.x;
    int top_left_y = button_rect.top_left.y;
    int h = min(button_width / 2, button_height / 2);
    ei_point_t point1, point2;
    ei_linked_point_t *current;
    ei_linked_point_t *previous;
    ei_linked_point_t *button1;
    ei_linked_point_t *button2;

    if (param <= 1) {
        // Top right first part
        point1.x = top_left_x + button_width - radius;
        point1.y = top_left_y + radius;
        current = arc(&point1, radius, 45, 90, N);
        //Button1 is at top right
        button1 = current;
        previous = current;

        // Find last arc point
        while (previous->next != NULL) {
            previous = previous->next;
        }

        // Top left
        point1.x = top_left_x + radius;
        point1.y = top_left_y + radius;
        current = arc(&point1, radius, 90, 180, N);
        previous->next = current;

        // Find last arc point
        while (previous->next != NULL) {
            previous = previous->next;
        }

        // Bot left first part
        point1.x = top_left_x + radius;
        point1.y = top_left_y + button_height - radius;
        current = arc(&point1, radius, 180, 225, N);
        previous->next = current;
        while (previous->next != NULL) {
            previous = previous->next;
        }
        //button2 is at bot left
        button2 = previous;
    }

    if (param != 1) {
        // Bot left second part
        point1.x = top_left_x + radius;
        point1.y = top_left_y + button_height - radius;
        current = arc(&point1, radius, 225, 270, N);
        // si on doit construire tout le boutton on récupère les paramètres précédent
        if (param == 0) {
            previous = current;
            current = current->next;
            button2->next = current;
            free(previous);
            previous = current;
        } else {
            button2 = current;
            previous = current;
        }
        while (previous->next != NULL) {
            previous = previous->next;
        }

        // Bot right
        point1.x = top_left_x + button_width - radius;
        point1.y = top_left_y + button_height - radius;
        current = arc(&point1, radius, 270, 360, N);
        previous->next = current;
        while (previous->next != NULL) {
            previous = previous->next;
        }

        // Top right
        point1.x = top_left_x + button_width - radius;
        point1.y = top_left_y + radius;
        current = arc(&point1, radius, 0, 45, N);
        previous->next = current;
        while (previous->next != NULL) {
            previous = previous->next;
        }
        if (param == 0) {
            previous->next = NULL;
            return button1;
        } else {
            button1 = previous;
        }
    }
    //Construction of the cut
    // point at the bot left
    point1.x = top_left_x + h;
    point2.y = top_left_y + button_height - h;

    //point at the top right
    point2.x = top_left_x + button_width - h;
    point2.y = top_left_y + h;
    ei_linked_point_t *cut_bot_left = malloc(sizeof(ei_linked_point_t));
    cut_bot_left->point = point1;
    ei_linked_point_t *cut_top_right = malloc(sizeof(ei_linked_point_t));
    cut_top_right->point = point2;
    ei_linked_point_t *end = malloc(sizeof(ei_linked_point_t));
    if (param == 1) {
        button2->next = cut_bot_left;
        cut_bot_left->next = cut_top_right;
        cut_top_right->next = end;
        end->point = button1->point;
        end->next = NULL;
    } else {
        button1->next = cut_top_right;
        cut_top_right->next = cut_bot_left;
        cut_bot_left->next = end;
        end->point = button2->point;
        end->next = NULL;
    }
    return end;
}

int draw_button(ei_surface_t surface, ei_rect_t button_rect, ei_color_t color, int radius, bool etat) {
    int button_width = button_rect.size.width;
    int button_height = button_rect.size.height;

    // Si etat est à true le boutton est relevé, sinon il est enfoncé
    ei_color_t darker;
    ei_color_t lighter;

    // The two part of the button
    ei_linked_point_t *top = rounded_frame(button_rect, radius, 10, 1);
    ei_linked_point_t *bot = rounded_frame(button_rect, radius, 10, 2);
    // The button
    ei_rect_t inside_button;
    int border_size = min(button_rect.size.width / 20, button_rect.size.height / 20);
    inside_button.top_left.x = button_rect.top_left.x + border_size;
    inside_button.top_left.y = button_rect.top_left.y + border_size;
    inside_button.size.width = button_rect.size.width - 2 * border_size;
    inside_button.size.height = button_rect.size.height - 2 * border_size;
    radius = radius - border_size;
    ei_linked_point_t *button = rounded_frame(inside_button, radius, 10, 0);

    if (etat) {
        ei_draw_polygon(surface, top, lighter, NULL);
        ei_draw_polygon(surface, bot, darker, NULL);
    } else {
        ei_draw_polygon(surface, top, darker, NULL);
        ei_draw_polygon(surface, bot, lighter, NULL);
    }
    ei_draw_polygon(surface, button, color, NULL);
}

ei_bool_t intersection(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result) {

    int leftX = max(r1->top_left.x, r2->top_left.x);
    int rightX = min(r1->top_left.x + r1->size.width, r2->top_left.x + r2->size.width);
    int topY = max(r1->top_left.y, r2->top_left.y);
    int bottomY = min(r1->top_left.y + r1->size.height, r2->top_left.y + r2->size.height);

    if (leftX < rightX && topY < bottomY) {
        result->top_left = ei_point(leftX, topY);
        result->size = ei_size(rightX - leftX, bottomY - topY);
    } else {
        return false;
    }
    return true;
}
