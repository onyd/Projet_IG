//
// Created by antho on 06/05/2021.
//
#include "utils.h"

void swap(uint32_t *a, uint32_t *b) {
    uint32_t tmp = *a;
    (*a) = *b;
    (*b) = tmp;
}

