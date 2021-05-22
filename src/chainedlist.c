#include <ei_widget.h>
#include "chainedlist.h"
#include <stdlib.h>

chained_list *create_chained_list() {
    chained_list *l = calloc(1, sizeof(chained_list));
    return l;
}

void append_left_linked(chained_cell *e, chained_list *l) {
    // Empty
    if (l->head == NULL) {
        l->head = e;
        l->tail = e;
        return;
    }

    // 2 elements
    if (l->pre_tail == NULL && l->head != l->tail) {
        l->pre_tail = l->head;
    }

    e->next = l->head;
    l->head = e;
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

void append_left_chained_list(void *element, chained_list *l) {
    chained_cell *linked = calloc(1, sizeof(chained_cell));
    linked->val = element;
    append_left_chained_list(linked, l);
}

void append_chained_list(void *element, chained_list *l) {
    chained_cell *linked = calloc(1, sizeof(chained_cell));
    linked->val = element;
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

void clear(chained_list *l) {
    chained_cell *current = l->head;
    while (current != NULL) {
        chained_cell *tmp = current;
        current = current->next;
        free(tmp);
    }
}

void free_chained_list(chained_list *l) {
    clear(l);
    free(l);
}

ei_linked_rect_t *to_linked_rect(chained_list *l) {
    ei_linked_rect_t *result = calloc(1, sizeof(ei_linked_rect_t));
    chained_cell *current = l->head;
    ei_linked_rect_t *current_result = result;
    while (current->next != NULL) {
        current_result->rect = *((ei_rect_t *)current->val);
        current_result->next = calloc(1, sizeof(ei_linked_rect_t));
        current = current->next;
        current_result = current_result->next;
    }
    current_result->rect = *((ei_rect_t *)current->val);
    current_result->next = NULL;

    return result;
}
//void widget_breadth_list(void *start, chained_list *result) {
//    chained_cell *linked_start = malloc(sizeof(chained_cell));
//    linked_start->val = start;
//    linked_start->next = NULL;
//    chained_list to_see = {linked_start, NULL, linked_start};
//
//    chained_cell *current;
//    while (to_see.head != NULL) {
//        move(&to_see, result);
//
//        // Add children for next children stage
//        void *children = ((ei_widget_t *) result->tail->val)->children_head;
//        if (children != NULL) {
//            while (children != NULL) {
//                append_left(children, &to_see);
//                children = ((ei_widget_t *) children)->next_sibling;
//            }
//        }
//    }
//    result->tail->next = NULL;
//}

