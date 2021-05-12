#ifndef PROJETC_IG_UTILS_H
#define PROJETC_IG_UTILS_H

#include "stdint.h"
#include "ei_types.h"

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
void swap(uint32_t *a, uint32_t *b);

struct ei_linked_point_t *y_argmax(struct ei_linked_point_t *a, struct ei_linked_point_t *b);

/**
 * \brief	Return the point which have the maximum y coordinates
 *
 * @param	a 	struct ei_linked_point_t the first point.
 * @param	b   struct ei_linked_point_t the other point.
 */
struct ei_linked_point_t *y_argmin(struct ei_linked_point_t *a, struct ei_linked_point_t *b);

ei_point_t absolute_coords(ei_rect_t relative, float x_rel, float y_rel);

#endif //PROJETC_IG_UTILS_H
