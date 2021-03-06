#include "geometry.h"


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


ei_linked_point_t *arc(ei_point_t c, float r, float start_angle, float end_angle, uint32_t N) {
    float da = (end_angle - start_angle) / (float) N;

    ei_linked_point_t *points = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *current = points;
    ei_linked_point_t *previous = points;
    float angle = start_angle;
    current->point.x = c.x + (int) (r * cosf(angle * (pi / 180.0f)));
    current->point.y = c.y - (int) (r * sinf(angle * (pi / 180.0f)));

    uint32_t i;
    for (i = 1; i <= N; i++) {
        current = calloc(1, sizeof(ei_linked_point_t));
        angle += da;
        current->point.x = c.x + (int) (r * cosf(angle * (pi / 180.0f)));
        current->point.y = c.y - (int) (r * sinf(angle * (pi / 180.0f)));

        previous->next = current;
        previous = current;
    }
    previous->next = NULL;

    return points;
}


ei_linked_point_t *rounded_frame(ei_rect_t button_rect, int radius, uint32_t N, direction dir) {
    // if dir = both, generate all the boutton, if dir = up generate the top, and if dir = down the bottom
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
    float radius_f = (float) radius;
    if (dir <= 1) {
        // Top right first part
        point1.x = top_left_x + (int) (button_width - radius);
        point1.y = top_left_y + radius;
        current = arc(point1, radius_f, 45, 90, N);
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
        current = arc(point1, radius_f, 90, 180, N);
        previous->next = current;

        // Find last arc point
        while (previous->next != NULL) {
            previous = previous->next;
        }

        // Bot left first part
        point1.x = top_left_x + radius;
        point1.y = top_left_y + button_height - radius;
        current = arc(point1, radius_f, 180, 225, N);
        previous->next = current;
        while (previous->next != NULL) {
            previous = previous->next;
        }
        //button2 is at bot left
        button2 = previous;
    }

    if (dir != 1) {
        // Bot left second part
        point1.x = top_left_x + radius;
        point1.y = top_left_y + button_height - radius;
        current = arc(point1, radius_f, 225, 270, N);
        // If we have to build the whole button, we use previous values
        if (dir == 0) {
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
        current = arc(point1, radius_f, 270, 360, N);
        previous->next = current;
        while (previous->next != NULL) {
            previous = previous->next;
        }

        // Top right
        point1.x = top_left_x + button_width - radius;
        point1.y = top_left_y + radius;
        current = arc(point1, radius_f, 0, 45, N);
        previous->next = current;
        while (previous->next != NULL) {
            previous = previous->next;
        }
        if (dir == 0) {
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
    if (dir == 1) {
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


ei_bool_t intersection_rect(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result) {
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

void union_rect(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result) {
    if (r1 == NULL) {
        *result = ei_rect(ei_point(r2->top_left.x, r2->top_left.y), ei_size(r2->size.width, r2->size.height));
    }
    if (r2 == NULL) {
        *result = ei_rect(ei_point(r1->top_left.x, r1->top_left.y), ei_size(r1->size.width, r1->size.height));
    }

    int top_left_x = min(r1->top_left.x, r2->top_left.x);
    int top_left_y = min(r1->top_left.y, r2->top_left.y);
    int width = max(r1->top_left.x + r1->size.width, r2->top_left.x + r2->size.width) -
                min(r1->top_left.x, r2->top_left.x);
    int height = max(r1->top_left.y + r1->size.height, r2->top_left.y + r2->size.height) -
                 min(r1->top_left.y, r2->top_left.y);

    result->top_left.x = top_left_x;
    result->top_left.y = top_left_y;
    result->size.width = width;
    result->size.height = height;
}

float vertical_line_intersection_rect(ei_point_t first, ei_point_t second, float x, ei_point_t *clipped) {
    float x1 = (float) first.x;
    float x2 = (float) second.x;
    float y1 = (float) first.y;
    float y2 = (float) second.y;

    // Line parmeters
    float a = (y2 - y1) / (x2 - x1);
    float b = y1 - a * x1;

    float y = a * x + b;
    int int_y = (int) y;
    *clipped = ei_point((int) x, int_y);

    return y - (float) int_y;
}

float horizontal_line_intersection_rect(ei_point_t first, ei_point_t second, float y, ei_point_t *clipped) {
    float x1 = (float) first.x;
    float x2 = (float) second.x;
    float y1 = (float) first.y;
    float y2 = (float) second.y;

    // Vertical
    if (x1 == x2) {
        *clipped = ei_point((int) x1, (int) y);
        return 0;
    }

    // Line parmeters
    float a = (y2 - y1) / (x2 - x1);
    float b = y1 - a * x1;

    float x = (y - b) / a;
    int int_x = (int) x;
    *clipped = ei_point(int_x, (int) y);

    return x - (float) int_x;
}

int cross_product(ei_point_t v1, ei_point_t v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

ei_bool_t is_left(ei_point_t p, ei_point_t p1, ei_point_t p2) {
    ei_point_t v1 = ei_point_sub(p, p1);
    ei_point_t v2 = ei_point_sub(p2, p1);
    return cross_product(v1, v2) < 0;
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


