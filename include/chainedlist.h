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

void append_left(chained_cell *widget, chained_list *l);

void append_linked(chained_cell *e, chained_list *l);

void append_cl(void *element, chained_list *l);

void move(chained_list *src, chained_list *dst);

void free_chained_list(chained_list *start);

void widget_breadth_list(void *start, chained_list *result);

#endif //PROJETC_IG_CHAINEDLIST_H
