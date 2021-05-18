#ifndef PROJETC_IG_VECTOR_H
#define PROJETC_IG_VECTOR_H

#include "stddef.h"

typedef struct {
    void **data;
    size_t used;
    size_t size;
} vector;


void create_vector(vector *a, size_t size);

void *get(vector *v, size_t i);

void append(vector *a, void *element);

size_t delete(vector *a, void *element);

void free_vector(vector *a);

void print_vector(vector *a);

#endif //PROJETC_IG_VECTOR_H
