#ifndef PROJETC_IG_VECTOR_H
#define PROJETC_IG_VECTOR_H

#include "stddef.h"

typedef struct {
    void **data;
    size_t last_idx;
    size_t current_idx;
    size_t size;
} vector;


vector *create_vector(size_t size);

void *get(vector *v, size_t i);

void append_vector(vector *a, void *element);

size_t remove_vector(vector *a, void *element);

void free_vector(vector *a);

void print_vector(vector *a);

#endif //PROJETC_IG_VECTOR_H
