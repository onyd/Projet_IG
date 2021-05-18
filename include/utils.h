#ifndef PROJETC_IG_UTILS_H
#define PROJETC_IG_UTILS_H

#include "stdint.h"
#include "ei_types.h"
#include "hw_interface.h"

struct table_cote {
    int ymax;
    int x_ymin;
    int x_ymax;
    int E;
    int dx;
    int dy;
    int sign_dx;
    struct table_cote *next;
};

struct table_cote_actif {
    struct table_cote *head;
};


void append_left_tca(struct table_cote *e, struct table_cote_actif *tca);

void delete_y(int y, struct table_cote_actif *tca);

void display(struct table_cote_actif *tac);

void sorting_insert(struct table_cote *tc, struct table_cote_actif *tca);

/**
 * \brief	free the structure table_cote
 *
 * @param	 	struct table_cote*.
 */
void tc_free(struct table_cote *);

/**
 * \brief	free the structure table_cote_actif
 *
 * @param	 	struct table_cote_actif*.
 */
void tca_free(struct table_cote_actif *);

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
