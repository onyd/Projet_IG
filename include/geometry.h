#ifndef PROJETC_IG_GEOMETRY_H
#define PROJETC_IG_GEOMETRY_H

#include "ei_types.h"
#include "math.h"
#include "stdlib.h"
#include "ei_utils.h"
#include <stdbool.h>
#include "stdio.h"
#include "ei_draw.h"


static const float pi = 3.14159265359f;

typedef enum direction {
    both = 0,
    up,
    down
} direction;

// Draw helper
ei_linked_point_t *arc(ei_point_t c, float r, float start_angle, float end_angle, uint32_t N);

ei_linked_point_t *rounded_frame(ei_rect_t button_rect, uint32_t radius, uint32_t N, enum direction dir);


// Geometry helper
ei_bool_t intersection_rect(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result);

void union_rect(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result);

float vertical_line_intersection_rect(ei_point_t first, ei_point_t second, float x, ei_point_t *clipped);

float horizontal_line_intersection_rect(ei_point_t first, ei_point_t second, float y, ei_point_t *clipped);

int cross_product(ei_point_t v1, ei_point_t v2);

ei_bool_t is_left(ei_point_t p, ei_point_t p1, ei_point_t p2);

ei_bool_t inside(ei_point_t p, const ei_rect_t *r);


#endif //PROJETC_IG_GEOMETRY_H
