#ifndef PROJETC_IG_VECTOR_H
#define PROJETC_IG_VECTOR_H

#include "stddef.h"

typedef struct {
    void **data;
    uint32_t last_idx;
    uint32_t current_idx;
    uint32_t size;
} vector;


vector *create_vector(uint32_t size);

void *get(vector *v, uint32_t i);

uint32_t append_vector(vector *a, void *element);

uint32_t remove_vector(vector *a, void *element);

void concatenation(vector *a, vector *b);

void copy(vector *a, vector *b);

void clear(vector *v, uint32_t *size);

void resize(vector *v, uint32_t size);

void upsize(vector *v, uint32_t i);

void free_vector(vector *a);

void print_vector(vector *a);

#endif //PROJETC_IG_VECTOR_H
