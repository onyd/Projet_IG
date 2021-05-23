#ifndef PROJETC_IG_DRAW_H
#define PROJETC_IG_DRAW_H

#include "ei_draw.h"
#include "vector.h"
#include "geometry.h"
#include "utils.h"

/** @brief Says where the point is clipped according to clipper
 */
typedef enum clipping_type {
    north_west_reject = 0,
    north_reject,
    north_east_reject,
    east_reject,
    south_east_reject,
    south_reject,
    south_west_reject,
    west_reject,
    center_reject
} clipping_type;


/**
 * @brief Allows to draw the whole window from the root widget
 */
void draw_window();

/** @brief Allows to draw a rectangle
 * @param surface       the surface on which we want to draw
 * @param rect          the rectangle
 * @param color         the color
 * @param clipper       the rect that clip the drawing
 */
void draw_rectangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper);


/** @brief Allows to draw a triangle from a rectangle
 * @param surface       the surface on which we want to draw
 * @param rect          the rectangle
 * @param clor          the color
 * @param clipper       the rect that clip the drawing
 * @param dir           dir = up for top triangle | dir = down for bottom triangle
 */
void draw_rect_triangle(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, direction dir);


/** @brief Allows to draw an image on a surface
 * @param surface       the surface on which we want to draw
 * @param img           the surface of the image
 * @param pos           the position of the topleft of the image
 * @param img_rect      the img_rect of the widget
 * @param clipper       the rect that clip the drawing
 */
void draw_image(ei_surface_t surface, ei_surface_t img, ei_point_t *pos, ei_rect_t *img_rect, ei_rect_t *clipper);

/** @brief Allows to draw a cross on a surface
 * @param surface       the surface on which we want to draw
 * @param rect          the rectangle in where we want to draw
 * @param color         the color of the cross
 * @param clipper       the rect that clip the drawing
 * @param size          the thickness of the cross
 */
void draw_cross(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int32_t size);

/** @brief Allows to draw a lined rectangle with blank on a surface
 * @param surface       the surface on which we want to draw
 * @param rect          the rectangle in where we want to draw
 * @param color         the color of the line
 * @param clipper       the rect that clip the drawing
 * @param w             the width of the blank
 * @param d             the margin from where the blank start
 */
void draw_blank_rect(ei_surface_t surface, ei_rect_t rect, ei_color_t color, ei_rect_t *clipper, int32_t w, int32_t d);

/** @brief Compute the Sutherland-Cohen code on the 4 LSB of the uint_8
 * @param p             the point which we want the code
 * @param clipper       the rect that clip
 */
uint8_t cohen_sutherland_code(ei_point_t p, ei_rect_t *clipper);

/** @brief Give what is the type of clipping of the point (see \ref clipping_type)
 * @param code          the Sutherland-Cohen code
 */
enum clipping_type get_clipping_type(uint8_t code);

/** @brief Compute the clipped segments, the errors due to int cast and return false if it completely clipped else true
 * @param p1            the first point of the edge
 * @param p2            the second point of the edge
 * @param clipped1      where to store the first clipped point of the edge
 * @param clipped2      where to store the second clipped point of the edge
 * @param error         where to store the error
 * @param clipper       the clipper
 */
ei_bool_t line_analytic_clipping(ei_point_t p1, ei_point_t p2, ei_point_t *clipped1, ei_point_t *clipped2, float *error,
                                 ei_rect_t *clipper);

/** @brief Compute the clipped polygons, the errors due to int cast
 * @param points        the points of the polygon
 * @param clipped       where to store the clipped polygons
 * @param errors        where to store the error
 * @param clipper       the clipper
 */
void polygon_analytic_clipping(ei_linked_point_t *points, ei_point_list_t *clipped, ei_error_list_t *errors, ei_rect_t *clipper);

#endif //PROJETC_IG_DRAW_H
