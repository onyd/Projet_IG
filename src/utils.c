#include "utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "ei_utils.h"
#include "hw_interface.h"

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


ei_point_t topleft(ei_point_t anchor_point, ei_size_t size, const ei_anchor_t *anchor) {
    switch (*anchor) {
        case ei_anc_none:
            break;
        case ei_anc_center:
            anchor_point = ei_point_sub(anchor_point, ei_point(0.5 * size.width, 0.5 * size.height));
            break;
        case ei_anc_north:
            anchor_point = ei_point_sub(anchor_point, ei_point(0.5 * size.width, 0));
            break;
        case ei_anc_northeast:
            anchor_point = ei_point_sub(anchor_point, ei_point(size.width, 0));
            break;
        case ei_anc_east:
            anchor_point = ei_point_sub(anchor_point, ei_point(size.width, 0.5 * size.height));
            break;
        case ei_anc_southeast:
            anchor_point = ei_point_sub(anchor_point, ei_point(size.width, size.height));
            break;
        case ei_anc_south:
            anchor_point = ei_point_sub(anchor_point, ei_point(0.5 * size.width, size.height));
            break;
        case ei_anc_southwest:
            anchor_point = ei_point_sub(anchor_point, ei_point(0, size.height));
            break;
        case ei_anc_west:
            anchor_point = ei_point_sub(anchor_point, ei_point(0, 0.5 * size.height));
            break;
        case ei_anc_northwest:
            break;
    }
    return anchor_point;
}

ei_point_t anchor_point(ei_point_t topleft, ei_size_t size, const ei_anchor_t *anchor) {
    switch (*anchor) {
        case ei_anc_none:
            break;
        case ei_anc_center:
            topleft = ei_point_add(topleft, ei_point(0.5 * size.width, 0.5 * size.height));
            break;
        case ei_anc_north:
            topleft = ei_point_add(topleft, ei_point(0.5 * size.width, 0));
            break;
        case ei_anc_northeast:
            topleft = ei_point_add(topleft, ei_point(size.width, 0));
            break;
        case ei_anc_east:
            topleft = ei_point_add(topleft, ei_point(size.width, 0.5 * size.height));
            break;
        case ei_anc_southeast:
            topleft = ei_point_add(topleft, ei_point(size.width, size.height));
            break;
        case ei_anc_south:
            topleft = ei_point_add(topleft, ei_point(0.5 * size.width, size.height));
            break;
        case ei_anc_southwest:
            topleft = ei_point_add(topleft, ei_point(0, size.height));
            break;
        case ei_anc_west:
            topleft = ei_point_add(topleft, ei_point(0, 0.5 * size.height));
            break;
        case ei_anc_northwest:
            break;
    }
    return topleft;
}

ei_point_t *topleft_text(ei_anchor_t anchor, ei_font_t font, char *text, ei_rect_t parent) {
    int width;
    int height;
    ei_point_t *topleft = malloc(sizeof(ei_point_t));
    hw_text_compute_size(text, font, &width, &height);
    switch (anchor) {
        case ei_anc_none:
            topleft->y = parent.top_left.y;
            topleft->x = parent.top_left.x;
            break;
        case ei_anc_center:
            topleft->y = parent.top_left.y + parent.size.height/2 - height/2;
            topleft->x = parent.top_left.x + parent.size.width/2 - width/2;
            break;
        case ei_anc_north:
            topleft->y = parent.top_left.y;
            topleft->x = parent.top_left.x + parent.size.width/2 - width/2;
            break;
        case ei_anc_northeast:
            topleft->y = parent.top_left.y;
            topleft->x = parent.top_left.x + parent.size.width - width;
            break;
        case ei_anc_east:
            topleft->y = parent.top_left.y + parent.size.height/2 - height/2;
            topleft->x = parent.top_left.x + parent.size.width - width;
            break;
        case ei_anc_southeast:
            topleft->y = parent.top_left.y + parent.size.height - height;
            topleft->x = parent.top_left.x + parent.size.width - width;
            break;
        case ei_anc_south:
            topleft->y = parent.top_left.y + parent.size.height - height;
            topleft->x = parent.top_left.x + parent.size.width/2 - width/2;
            break;
        case ei_anc_southwest:
            topleft->y = parent.top_left.y + parent.size.height - height;
            topleft->x = parent.top_left.x;
            break;
        case ei_anc_west:
            topleft->y = parent.top_left.y + parent.size.height/2 - height/2;
            topleft->x = parent.top_left.x;
            break;
        case ei_anc_northwest:
            topleft->y = parent.top_left.y;
            topleft->x = parent.top_left.x;
            break;
    }
    return topleft;
}
