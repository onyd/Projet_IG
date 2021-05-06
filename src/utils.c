#include "utils.h"
#include "stdio.h"

void append_left(struct table_cote *e, struct table_cote_actif *tca) {
    if (tca->head == NULL) {
        tca->head = e;
        return;
    }
    e->next = tca->head;
    tca->head = e;
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

void sorting_insert(struct table_cote *tc, struct table_cote_actif *tca) {
    if (tca->head == NULL) {
        tca->head = tc;
        return;
    }
    struct table_cote *prec = tca->head;
    struct table_cote *current = prec;
    while (current != NULL) {
        // à savoir: il ne peut pas y avoir plus de deux côtés qui ont le même xpmin
        if (current->xpmin == tc->xpmin) {
            //si c'est à la tête:
            if (current == prec) {
                if (current->xpmax > tc->xpmax) {
                    append_left(tc, tca);
                }
                else {
                    tc->next = current->next;
                    current->next = tc;
                }
            }
            else {
                if (current->xpmax > tc->xpmax) {
                    prec->next = tc;
                    tc->next = current;
                }
                else {
                    tc->next = current->next;
                    current->next = tc;
                }
            }
            return;
        }
        if (current->xpmin > tc->xpmin) {
            //si c'est la tete
            if (current == prec) {
                append_left(tc, tca);
            }
            else {
                prec->next = tc;
                tc->next = current;
            }
            return;
        }
        prec = current;
        current = current->next;
    }
    if (current == NULL) {
        prec->next = tc;
    }
}


