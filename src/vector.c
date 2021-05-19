#include "stdint.h"
#include "vector.h"
#include "stdlib.h"
#include "stdio.h"

vector *create_vector(size_t size) {
    vector *v = calloc(1, sizeof (vector));
    v->data = calloc(size, sizeof(void *));
    v->used = 0;
    v->size = size;
    return v;
}

void *get(vector *v, size_t i) {
    return v->data[i];
}

void append_vector(vector *v, void *element) {
    // Not enough space, up-size the vector
    if (v->used == v->size) {
        v->size *= 2;
        v->data = realloc(v->data, v->size * sizeof(void *));
    }
    v->data[v->used++] = element;
}

size_t remove_vector(vector *v, void *element) {
    for (uint32_t i = 0; i < v->size; i++) {
        if (v->data[i] == element) {
            v->data[i] = NULL;
            // Last element removed => reduce used partition
            if (v->used == v->size - 1) {
                v->used--;
            }
            break;
        }
    }

    // Too much space, down-size the vector
    if (v->used < v->size / 2) {
        v->size /= 2;
        v->data = realloc(v->data, v->size * sizeof(void *));
    }
    return v->used;
}

void free_vector(vector *v) {
    free(v->data);
    v->data = NULL;
    v->used = v->size = 0;
    free(v);
}

void print_vector(vector *v) {
    for (uint32_t i = 0; i < v->size; i++) {
        printf("%p\n", v->data[i]);
    }
    printf("\n");
}