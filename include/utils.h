//
// Created by antho on 06/05/2021.
//

#ifndef PROJETC_IG_UTILS_H
#define PROJETC_IG_UTILS_H

#include "stdint.h"

struct table_cote {
    int ymax;
    int ymin;
    int xpmin;
    int xpmax;
    int E;
    int dx;
    int dy;
    struct table_cote *next;
};

struct table_cote_actif {
    struct  table_cote *head;
};

void append_left(struct table_cote* e, struct table_cote_actif* tac);

void display(struct table_cote_actif* tac);
/**
 * \brief	Swap the variable content pointed by a and b
 *
 * @param	a 	uint32_t variable.
 * @param	b   uint32_t variable.
 */
void swap(uint32_t *a, uint32_t *b);

/**
 * \brief	Return max of a and b
 *
 * @param	a 	uint32_t variable.
 * @param	b   uint32_t variable.
 */
int max(int a, int b);

void sorting_insert(struct table_cote* tc, struct table_cote_actif* tac);

#endif //PROJETC_IG_UTILS_H
