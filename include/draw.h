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


/** fonction that create a triangle from a rectangle
 * @param the surface
 * @param the rectangle
 * @param the color
 * @param the clipper
 * @param direction = 0: the triangle is at the top of the rectangle, else at the bottom
 */
void rect_to_triangle(ei_surface_t, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int direction);

#endif //PROJETC_IG_DRAW_H
