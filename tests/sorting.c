#include "utils.h"
#include "stdlib.h"

int main() {
    struct table_cote_actif *tac = malloc(sizeof (struct table_cote_actif));
    tac->head = NULL;

    struct table_cote *tc1 = malloc(sizeof (struct table_cote));
    struct table_cote *tc2 = malloc(sizeof (struct table_cote));
    struct table_cote *tc3 = malloc(sizeof (struct table_cote));
    struct table_cote *tc4 = malloc(sizeof (struct table_cote));
    struct table_cote *tc5 = malloc(sizeof (struct table_cote));
    struct table_cote *tc6 = malloc(sizeof (struct table_cote));
    struct table_cote *tc7 = malloc(sizeof (struct table_cote));

    tc1->xpmax = -1;
    tc2->xpmax = 1;
    tc3->xpmax = 2;
    tc4->xpmax = 3;
    tc5->xpmax = 4;
    tc6->xpmax = 5;
    tc7->xpmax = 6;

    tc1->next = tc2;
    tc2->next = tc3;
    tc3->next = tc4;
    tc4->next = tc5;
    tc5->next = tc6;
    tc6->next = tc7;

    append_left(tc1, tac);
    display(tac);

    struct table_cote *tc8 = malloc(sizeof (struct table_cote));
    tc8->xpmax = 0;
    sorting_insert(tc8, tac);

    display(tac);

    struct table_cote *tc9 = malloc(sizeof (struct table_cote));
    tc2->xpmax = 2;
    sorting_insert(tc9, tac);

    display(tac);

}

