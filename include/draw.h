#ifndef PROJETC_IG_DRAW_H
#define PROJETC_IG_DRAW_H

#include "ei_draw.h"

typedef enum trivial_clipping_code {
    trivial_accept = 0,
    trivial_reject,
    non_trivial_reject
} trivial_clipping_code;

typedef enum clipping_code {
    north_west_reject = 0,
    north_reject,
    north_east_reject,
    east_reject,
    south_east_reject,
    south_reject,
    south_west_reject,
    west_reject
} clipping_code;

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

uint8_t cohen_sutherland_code(ei_point_t p, ei_rect_t *clipper);

enum clipping_code get_clipping_code(uint8_t code);

#endif //PROJETC_IG_DRAW_H
