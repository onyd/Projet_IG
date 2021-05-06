#include "utils.h"
#include "stdio.h"

void append_left(struct table_cote *e, struct table_cote_actif *tac) {
    if (tac->head == NULL) {
        tac->head = e;
        return;
    }
    e->next = tac->head;
    tac->head = e;
}

void display(struct table_cote_actif *tac) {
    struct table_cote *current = tac->head;
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

void sorting_insert(struct table_cote *tc, struct table_cote_actif *tac) {
    if (tac->head->xpmin == tc->xpmin) {
        if (tac->head->xpmax < tc->xpmax) {
            tac->head->next = tc;
        } else {
            append_left(tc, tac);
        }
    } else {
        append_left(tc, tac);
    }

    struct table_cote *previous = tac->head;
    struct table_cote *current = tac->head->next;
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


