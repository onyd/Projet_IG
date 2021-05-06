//
// Created by antho on 06/05/2021.
//

#ifndef PROJETC_IG_UTILS_H
#define PROJETC_IG_UTILS_H

#include "stdint.h"

struct table_cote {
    int ymax;
    int xymin;
    int E;
    int dx;
    int dy;
    struct table_cote *next;
};

void swap(uint32_t *a, uint32_t *b);

// retourne le max de a et b
int max(int a, int b);

#endif //PROJETC_IG_UTILS_H
