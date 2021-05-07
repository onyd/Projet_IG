#include "utils.h"
#include "stdlib.h"

int main() {
    struct table_cote_actif *tca = malloc(sizeof (struct table_cote_actif));
    tca->head = NULL;

    struct table_cote *tc1 = malloc(sizeof (struct table_cote));
    struct table_cote *tc2 = malloc(sizeof (struct table_cote));
    struct table_cote *tc3 = malloc(sizeof (struct table_cote));
    struct table_cote *tc4 = malloc(sizeof (struct table_cote));
    struct table_cote *tc5 = malloc(sizeof (struct table_cote));
    struct table_cote *tc6 = malloc(sizeof (struct table_cote));
    struct table_cote *tc7 = malloc(sizeof (struct table_cote));

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

    append_left(tc1, tca);
    display(tca);

    struct table_cote *tc8 = malloc(sizeof (struct table_cote));
    tc8->x_ymax = -2;
    tc8->x_ymin = -1;
    sorting_insert(tc8, tca);

    display(tca);

    struct table_cote *tc9 = malloc(sizeof (struct table_cote));
    tc9->x_ymax = 2;
    sorting_insert(tc9, tca);

    display(tca);

}

