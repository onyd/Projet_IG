#ifndef PROJETC_IG_GEOMETRY_H
#define PROJETC_IG_GEOMETRY_H

#include "ei_types.h"
#include "math.h"
#include "stdlib.h"
#include "ei_utils.h"
#include <stdbool.h>
#include "stdio.h"
#include "ei_draw.h"
#include "ei_widget.h"


static const float pi = 3.1415927;

ei_linked_point_t *arc(const ei_point_t *c, uint32_t r, float start_angle, float end_angle, uint32_t N);

ei_linked_point_t *rounded_frame(ei_rect_t button_rect, uint32_t rayon, uint32_t N, int param);

void free_rounded_frame(ei_linked_point_t *points);

ei_bool_t intersection(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result);

#endif //PROJETC_IG_GEOMETRY_H
