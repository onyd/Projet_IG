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

/**
 * @brief	Identifies which part of the rectangle to draw.
 */
typedef enum direction {
    both = 0,                 ///< Draw all the rectangle.
    up,                       ///< Draw the top part of the rectangle.
    down                      ///< Draw the down part of the rectangle.
} direction;

// Draw helper
/**
 * @brief	Compute a linked points to construct an arc.
 */
ei_linked_point_t *arc(ei_point_t c, float r, float start_angle, float end_angle, uint32_t N);

/**
 * @brief	Compute a linked points to construct a rectangle with rounded corner.
 */
ei_linked_point_t *rounded_frame(ei_rect_t button_rect, int radius, uint32_t N, enum direction dir);


// Geometry helper
/**
 * @brief	Compute the intersection rectangle between tw0 rectangles r1 and r2.
 */
ei_bool_t intersection_rect(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result);

/**
 * @brief	Compute the union rectangle between tw0 rectangles r1 and r2.
 */
void union_rect(const ei_rect_t *r1, const ei_rect_t *r2, ei_rect_t *result);

/**
 * @brief	Compute the intersection point between the vertical line x and the line that goes through first and second.
 */
float vertical_line_intersection_rect(ei_point_t first, ei_point_t second, float x, ei_point_t *clipped);

/**
 * @brief	Compute the intersection point between the horizontal line y and the line that goes through first and second.
 */
float horizontal_line_intersection_rect(ei_point_t first, ei_point_t second, float y, ei_point_t *clipped);

/**
 * @brief	return the cross product between two points v1 and v2.
 */
int cross_product(ei_point_t v1, ei_point_t v2);

/**
 * @brief	Check if a point p is to the left to the line that goes through p1 and p2 or not.
 */
ei_bool_t is_left(ei_point_t p, ei_point_t p1, ei_point_t p2);

/**
 * @brief	Check if a point p is inside a rectangle r or not.
 */
ei_bool_t inside(ei_point_t p, const ei_rect_t *r);


#endif //PROJETC_IG_GEOMETRY_H
