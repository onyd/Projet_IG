#ifndef PROJETC_IG_GEOMETRY_H
#define PROJETC_IG_GEOMETRY_H

#include "ei_types.h"
const float pi = 3.1415927;

ei_point_t* arc(const ei_point_t* c, float r, float start_angle, float end_angle, uint32_t N);

ei_linked_point_t *rounded_frame(ei_rect_t button_rect, int rayon);

#endif //PROJETC_IG_GEOMETRY_H
