#include "utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "ei_utils.h"

void append_left_tca(struct table_cote *e, struct table_cote_actif *tca) {
    if (tca->head == NULL) {
        tca->head = e;
        return;
    }
    e->next = tca->head;
    tca->head = e;
}

void delete_y(int y, struct table_cote_actif *tca) {
    if (tca->head == NULL) {
        return;
    }

    struct table_cote *previous = tca->head;
    while (previous != NULL && previous->ymax == y) {
        struct table_cote *tmp = previous;
        previous = previous->next;
        free(tmp);
    }
    tca->head = previous;
    if (tca->head == NULL) {
        return;
    }

    struct table_cote *current = tca->head->next;
    while (current != NULL) {
        if (current->ymax == y) {
            previous->next = current->next;
            free(current);
            current = previous->next;
            continue;
        }
        previous = current;
        current = current->next;
    }
}

void display(struct table_cote_actif *tca) {
    struct table_cote *current = tca->head;
    while (current != NULL) {
        printf("[%i, %i]->", current->ymax, current->x_ymin);
        current = current->next;
    }
    printf("\n");
}


void swap(uint32_t *a, uint32_t *b) {
    uint32_t tmp = *a;
    (*a) = *b;
    (*b) = tmp;
}


struct ei_linked_point_t *y_argmax(struct ei_linked_point_t *a, struct ei_linked_point_t *b) {
    if (a->point.y > b->point.y) {
        return a;
    } else {
        return b;
    }
}

struct ei_linked_point_t *y_argmin(struct ei_linked_point_t *a, struct ei_linked_point_t *b) {
    if (a->point.y < b->point.y) {
        return a;
    } else {
        return b;
    }
}

void sorting_insert(struct table_cote *tc, struct table_cote_actif *tca) {
    if (tca->head == NULL) {
        tca->head = tc;
        return;
    }
    struct table_cote *previous = tca->head;
    struct table_cote *current = previous;
    while (current != NULL) {
        // à savoir: il ne peut pas y avoir plus de deux côtés qui ont le même x_ymin
        if (current->x_ymin == tc->x_ymin) {
            //si c'est à la tête:
            if (current == previous) {
                if (current->x_ymax > tc->x_ymax) {
                    append_left_tca(tc, tca);
                } else {
                    tc->next = current->next;
                    current->next = tc;
                }
            } else {
                if (current->x_ymax > tc->x_ymax) {
                    previous->next = tc;
                    tc->next = current;
                } else {
                    tc->next = current->next;
                    current->next = tc;
                }
            }
            return;
        }
        if (current->x_ymin > tc->x_ymin) {
            //si c'est la tete
            if (current == previous) {
                append_left_tca(tc, tca);
            } else {
                previous->next = tc;
                tc->next = current;
            }
            return;
        }
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        previous->next = tc;
    }
}

void tc_free(struct table_cote *tc) {
    struct table_cote *next_tc = tc;
    while (tc != NULL) {
        next_tc = tc->next;
        free(tc);
        tc = next_tc;
    }
}

void tca_free(struct table_cote_actif *tca) {
    tc_free(tca->head);
    free(tca);
}

ei_point_t absolute_coords(ei_rect_t relative, float x_rel, float y_rel) {
    return ei_point_add(relative.top_left, ei_point(x_rel * relative.size.width, y_rel * relative.size.height));
}