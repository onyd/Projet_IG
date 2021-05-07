#ifndef PROJETC_IG_GEOMETRY_H
#define PROJETC_IG_GEOMETRY_H

#include "ei_types.h"

ei_linked_point_t arc(const ei_point_t* c, uint16_t start_angle, uint16_t end_angle);

ei_linked_point_t *rounded_frame(ei_rect_t button_rect, int rayon);

#endif //PROJETC_IG_GEOMETRY_H
