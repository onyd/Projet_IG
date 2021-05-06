#include "utils.h"
#include "stdio.h"

void append_left(struct table_cote *e, struct table_cote_actif *tca) {
    if (tca->head == NULL) {
        tca->head = e;
        tca->tail = e;
        return;
    }
    e->next = tca->head;
}

void delete(struct table_cote *e, struct table_cote_actif *tca) {
    if (e == tca->head) {
        tca->head = NULL;
        return;;
    }

    struct table_cote *previous = tca->head;
    struct table_cote *current = tca->head->next;
    while (current != NULL) {
        if (current == e)
        previous = current;
        current = current->next;
    }
}

void display(struct table_cote_actif *tca) {
    struct table_cote *current = tca->head;
    while (current != NULL) {
        printf("[%i, %i]->", current->xpmax, current->xpmin);
        current = current->next;
    }
    printf("\n");
}


void swap(uint32_t *a, uint32_t *b) {
    uint32_t tmp = *a;
    (*a) = *b;
    (*b) = tmp;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

struct ei_linked_point_t *y_argmax(struct ei_linked_point_t *a, struct ei_linked_point_t *b) {
    if (a->point.y < b->point.y) {
        return a;
    } else {
        return b;
    }
}

struct ei_linked_point_t *y_argmin(struct ei_linked_point_t *a, struct ei_linked_point_t *b) {
    if (a->point.y > b->point.y) {
        return a;
    } else {
        return b;
    }
}

void sorting_insert(struct table_cote *tc, struct table_cote_actif *tca) {
    if (tca->head->xpmin == tc->xpmin) {
        if (tca->head->xpmax < tc->xpmax) {
            tca->head->next = tc;
        } else {
            append_left(tc, tca);
        }
    } else {
        append_left(tc, tca);
    }

    struct table_cote *previous = tca->head;
    struct table_cote *current = tca->head->next;
    while (current->next != NULL && current->next->xpmin <= current->xpmin) {
        if (current->next->xpmin == current->xpmin) {
            if (current->next->xpmax < current->xpmax) {
                previous->next = current->next;
                current->next->next = current;
                current->next = previous->next->next;
            }
        } else {
            previous->next = current->next;
            current->next->next = current;
            current->next = previous->next->next;
        }
        previous = current;
        current = current->next;
    }
}


