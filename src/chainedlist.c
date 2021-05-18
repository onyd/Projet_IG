#include <ei_widget.h>
#include "chainedlist.h"
#include "stdlib.h"

chained_list *create_chained_list() {
    chained_list *l = calloc(1, sizeof(chained_list));
    return l;
}

void append_left(chained_cell *cell, chained_list *l) {
    chained_cell *chained_cell = malloc(sizeof(chained_cell));
    chained_cell->val = cell;
    chained_cell->next = NULL;

    // Empty
    if (l->head == NULL) {
        l->head = chained_cell;
        l->tail = chained_cell;
        return;
    }

    // 2 elements
    if (l->pre_tail == NULL && l->head != l->tail) {
        l->pre_tail = l->head;
    }

    chained_cell->next = l->head;
    l->head = chained_cell;
}

void append_linked(chained_cell *e, chained_list *l) {
    // Empty
    if (l->head == NULL) {
        l->head = e;
        l->tail = e;
        return;
    }

    // More than 1 element
    if (l->tail != l->head) {
        l->pre_tail = l->tail;
    }

    l->tail->next = e;
    l->tail = e;
}

void append_cl(void *element, chained_list *l) {
    chained_cell *linked = malloc(sizeof(void *));
    append_linked(linked, l);
}

void move(chained_list *src, chained_list *dst) {
    // Empty
    if (src->head == NULL) {
        return;
    }
    append_linked(src->tail, dst);
    // 1 or 2 elements
    if (src->pre_tail == NULL) {
        // 1
        if (src->tail == src->head) {
            src->head = NULL;
            src->tail = NULL;
            return;
        } else { // 2
            src->tail = src->head;
            src->tail->next = NULL;
            return;
        }
    }
    // More
    src->tail = src->pre_tail;
    src->tail->next = NULL;
}

void free_chained_list(chained_list *l) {
    chained_cell *current = l->head;
    while (current != NULL) {
        chained_cell *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(l);
}

void widget_breadth_list(void *start, chained_list *result) {
    chained_cell *linked_start = malloc(sizeof(chained_cell));
    linked_start->val = start;
    linked_start->next = NULL;
    chained_list to_see = {linked_start, NULL, linked_start};

    chained_cell *current;
    while (to_see.head != NULL) {
        move(&to_see, result);

        // Add children for next children stage
        void *children = ((ei_widget_t *) result->tail->val)->children_head;
        if (children != NULL) {
            while (children != NULL) {
                append_left(children, &to_see);
                children = ((ei_widget_t *) children)->next_sibling;
            }
        }
    }
    result->tail->next = NULL;
}

