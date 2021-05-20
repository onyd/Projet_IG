#ifndef PROJETC_IG_UTILS_H
#define PROJETC_IG_UTILS_H

#include "stdint.h"
#include "ei_types.h"
#include "hw_interface.h"

struct linked_edges {
    int ymax;
    int x_ymin;
    int x_ymax;
    int E;
    int dx;
    int dy;
    int sign_dx;
    struct linked_edges *next;
};

struct linked_acive_edges {
    struct linked_edges *head;
};


void append_left_tca(struct linked_edges *e, struct linked_acive_edges *tca);

void delete_y(int y, struct linked_acive_edges *tca);

void display(struct linked_acive_edges *tac);

void sorting_insert(struct linked_edges *tc, struct linked_acive_edges *tca);

/**
 * \brief	free the structure linked_edges
 *
 * @param	 	struct linked_edges*.
 */
void tc_free(struct linked_edges *);

/**
 * \brief	free the structure linked_acive_edges
 *
 * @param	 	struct linked_acive_edges*.
 */
void tca_free(struct linked_acive_edges *);

/**
 * \brief	Swap the variable content pointed by a and b
 *
 * @param	a 	uint32_t variable.
 * @param	b   uint32_t variable.
 */
void swap(int *a, int *b);

struct ei_linked_point_t *y_argmax(struct ei_linked_point_t *a, struct ei_linked_point_t *b);

/**
 * \brief	Return the point which have the maximum y coordinates
 *
 * @param	a 	struct ei_linked_point_t the first point.
 * @param	b   struct ei_linked_point_t the other point.
 */
struct ei_linked_point_t *y_argmin(struct ei_linked_point_t *a, struct ei_linked_point_t *b);

ei_point_t topleft(ei_point_t anchor_point, ei_size_t size, const ei_anchor_t *anchor);

ei_point_t anchor_point(ei_point_t topleft, ei_size_t size, const ei_anchor_t *anchor);

ei_point_t anchor_target_pos(ei_anchor_t anchor, ei_size_t target_size, ei_rect_t parent_rect);

ei_color_t ei_map_rgba_inverse(ei_surface_t surface, uint32_t color_id);

#endif //PROJETC_IG_UTILS_H
