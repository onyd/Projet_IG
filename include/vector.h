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

size_t append_vector(vector *a, void *element);

size_t remove_vector(vector *a, void *element);

void concatenation(vector *a, vector *b);

void copy(vector *a, vector *b);

void clear(vector *v, size_t *size);

void resize(vector *v, size_t size);

void upsize(vector *v, uint32_t i);

void free_vector(vector *a);

void print_vector(vector *a);

#endif //PROJETC_IG_VECTOR_H
