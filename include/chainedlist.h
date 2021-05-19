#ifndef PROJETC_IG_CHAINEDLIST_H
#define PROJETC_IG_CHAINEDLIST_H

typedef struct chained_cell {
    void *val;
    struct chained_cell *next;
} chained_cell;

typedef struct chained_list {
    struct chained_cell *head;
    struct chained_cell *pre_tail;
    struct chained_cell *tail;
} chained_list;

chained_list *create_chained_list();

void append_left_linked(chained_cell *e, chained_list *l);

void append_linked(chained_cell *e, chained_list *l);

void append_left_chained_list(void *element, chained_list *l);

void append_chained_list(void *element, chained_list *l);

void move(chained_list *src, chained_list *dst);

void clear(chained_list *l);

void free_chained_list(chained_list *start);

ei_linked_rect_t *to_linked_rect(chained_list *l);

#endif //PROJETC_IG_CHAINEDLIST_H
