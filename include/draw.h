#ifndef PROJETC_IG_DRAW_H
#define PROJETC_IG_DRAW_H

#include "ei_draw.h"


/** @brief Allows to draw a rectangle
 * @param the surface on which we want to draw
 * @param the rectangle
 * @param the color
 * @param the clipper
 */
void draw_rectangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper);


/** @brief Allows to draw a triangle from a rectangle
 * @param the surface on which we want to draw
 * @param the rectangle
 * @param the color
 * @param the clipper
 * @param direction = 0: the triangle is at the top of the rectangle, else at the bottom
 */
void draw_rect_triangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int direction);

void draw_image(ei_surface_t surface, ei_surface_t img, ei_point_t *pos, ei_rect_t *img_rect, ei_rect_t *clipper);

void draw_cross(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int32_t size);

#endif //PROJETC_IG_DRAW_H
