#include "geometry.h"


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


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
    // if param = 0, generate all the boutton, if param = 1 generate the top else the bottom
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
    ei_linked_point_t *end = malloc(sizeof(ei_linked_point_t));
    end->next = NULL;

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
            button2->next = current;
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
            previous->next = end;
            end->point = button1->point;
            return button1;
        } else {
            button1 = previous;
        }
    }
    //Construction of the cut
    // point at the bot left
    point1.x = top_left_x + h;
    point1.y = top_left_y + button_height - h;

    //point at the top right
    point2.x = top_left_x + button_width - h;
    point2.y = top_left_y + h;
    ei_linked_point_t *cut_bot_left = malloc(sizeof(ei_linked_point_t));
    cut_bot_left->point = point1;
    ei_linked_point_t *cut_top_right = malloc(sizeof(ei_linked_point_t));
    cut_top_right->point = point2;
    if (param == 1) {
        button2->next = cut_bot_left;
        cut_bot_left->next = cut_top_right;
        cut_top_right->next = end;
        end->point = button1->point;
        return button1;
    } else {
        button1->next = cut_top_right;
        cut_top_right->next = cut_bot_left;
        cut_bot_left->next = end;
        end->point = button2->point;
        return button2;
    }
}


void free_rounded_frame(ei_linked_point_t *points) {
    free(points);
}

ei_bool_t intersection(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result) {
    if (r1 == NULL) {
        *result = ei_rect(ei_point(r2->top_left.x, r2->top_left.y), ei_size(r2->size.width, r2->size.height));
        return true;
    }
    if (r2 == NULL) {
        *result = ei_rect(ei_point(r1->top_left.x, r1->top_left.y), ei_size(r1->size.width, r1->size.height));
        return true;
    }

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

ei_bool_t inside(ei_point_t p, const ei_rect_t *r) {
    if (r == NULL) {
        return true;
    }

    if (p.x >= r->top_left.x &&
        p.x <= r->top_left.x + r->size.width &&
        p.y >= r->top_left.y &&
        p.y <= r->top_left.y + r->size.height) {
        return true;
    }

    return false;
}

ei_bool_t union_rect(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result){
    int top_left_x = min(r1->top_left.x, r2->top_left.x);
    int top_left_y = min(r1->top_left.y, r2->top_left.y);
    int width = max(r1->top_left.x + r1->size.width, r2->top_left.x + r2->size.width) - min(r1->top_left.x, r2->top_left.x);
    int height = max(r1->top_left.y + r1->size.height, r2->top_left.y + r2->size.height) - min(r1->top_left.y, r2->top_left.y);
    if (result->top_left.x >= 2){
        result->top_left.x = top_left_x - 2;
    } else {
        result->top_left.x = top_left_x;
    }
    if (result->top_left.y >= 2){
        result->top_left.y = top_left_y - 2;
    } else {
        result->top_left.y = top_left_y;
    }
    result->size.width = width + 4;
    result->size.height = height + 4;
}
