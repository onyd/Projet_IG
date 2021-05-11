#ifndef PROJETC_IG_DRAW_H
#define PROJETC_IG_DRAW_H

#include "ei_draw.h"


/** fonction that create a rectangle
 * @param the surface
 * @param the rectangle
 * @param the color
 * @param the clipper
 */
void draw_rectangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper);

#endif //PROJETC_IG_DRAW_H
