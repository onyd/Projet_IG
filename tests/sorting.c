#include "utils.h"
#include "stdlib.h"

int main() {
    struct linked_acive_edges *tca = malloc(sizeof(struct linked_acive_edges));
    tca->head = NULL;

    struct linked_edges *tc1 = malloc(sizeof(struct linked_edges));
    struct linked_edges *tc2 = malloc(sizeof(struct linked_edges));
    struct linked_edges *tc3 = malloc(sizeof(struct linked_edges));
    struct linked_edges *tc4 = malloc(sizeof(struct linked_edges));
    struct linked_edges *tc5 = malloc(sizeof(struct linked_edges));
    struct linked_edges *tc6 = malloc(sizeof(struct linked_edges));
    struct linked_edges *tc7 = malloc(sizeof(struct linked_edges));

    tc1->x_ymax = -1;
    tc2->x_ymax = 1;
    tc3->x_ymax = 2;
    tc4->x_ymax = 3;
    tc5->x_ymax = 4;
    tc6->x_ymax = 5;
    tc7->x_ymax = 6;

    tc1->x_ymin = -1;
    tc1->x_ymin = -1;
    tc2->x_ymin = 1;
    tc3->x_ymin = 2;
    tc4->x_ymin = 3;
    tc5->x_ymin = 4;
    tc6->x_ymin = 5;
    tc7->x_ymin = 6;

    tc1->next = tc2;
    tc2->next = tc3;
    tc3->next = tc4;
    tc4->next = tc5;
    tc5->next = tc6;
    tc6->next = tc7;
    tc7->next = NULL;

    append_left_tca(tc1, tca);
    display(tca);

    struct linked_edges *tc8 = malloc(sizeof(struct linked_edges));
    tc8->x_ymax = -2;
    tc8->x_ymin = -1;
    tc8->next = NULL;
    sorting_insert(tc8, tca);

    display(tca);

    struct linked_edges *tc9 = malloc(sizeof(struct linked_edges));
    tc9->x_ymax = 2;
    tc9->next = NULL;
    sorting_insert(tc9, tca);

    display(tca);

    delete_y(tc8, tca);
    display(tca);

    delete_y(tc3, tca);
    display(tca);


}

