#ifndef PROJETC_IG_VECTOR_H
#define PROJETC_IG_VECTOR_H

typedef struct {
    void **data;
    size_t used;
    size_t size;
} vector;


void create_vector(vector *a, size_t size);

void append(vector *a, void *element);

void delete(vector *a, void *element);

void free_vector(vector *a);

void print_vector(vector *a);

#endif //PROJETC_IG_VECTOR_H
