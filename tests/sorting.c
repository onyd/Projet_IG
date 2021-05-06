#include "utils.h"

int main() {
    struct table_cote_actif tac;

    struct table_cote tc1 = {0, 0, -1, 4, 0, 0, 0, NULL};
    struct table_cote tc2 = {0, 0, 1, 4, 0, 0, 0, NULL};
    struct table_cote tc3 = {0, 0, 2, 3, 0, 0, 0, NULL};
    struct table_cote tc4 = {0, 0, 3, 1, 0, 0, 0, NULL};
    struct table_cote tc5 = {0, 0, 4, 4, 0, 0, 0, NULL};
    struct table_cote tc6 = {0, 0, 5, 2, 0, 0, 0, NULL};
    struct table_cote tc7 = {0, 0, 6, 9, 0, 0, 0, NULL};

    tc1.next = &tc2;
    tc2.next = &tc3;
    tc3.next = &tc4;
    tc4.next = &tc5;
    tc5.next = &tc6;
    tc6.next = &tc7;

    append_left(&tc1, &tac);

    struct table_cote tc8 = {0, 0, 0, 2, 0, 0, 0, NULL};
    sorting_insert(&tc8, &tac);

    display(&tac);

    struct table_cote tc9 = {0, 0, 1, 6, 0, 0, 0, NULL};
    sorting_insert(&tc9, &tac);

    display(&tac);

}

