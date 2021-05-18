#include "stdint.h"
#include "vector.h"
#include "stdlib.h"
#include "stdio.h"

void create_vector(vector *a, size_t size) {
    a->data = calloc(size, sizeof(void *));
    a->used = 0;
    a->size = size;
}

void *get(vector *v, size_t i) {
    return v->data[i];
}

void append(vector *a, void *element) {
    // Not enough space, up-size the vector
    if (a->used == a->size) {
        a->size *= 2;
        a->data = realloc(a->data, a->size * sizeof(void *));
    }
    a->data[a->used++] = element;
}

size_t delete(vector *a, void *element) {
    for (uint32_t i = 0; i < a->size; i++) {
        if (a->data[i] == element) {
            a->data[i] = NULL;
            // Last element removed => reduce used partition
            if (a->used == a->size - 1) {
                a->used--;
            }
            break;
        }
    }

    // Too much space, down-size the vector
    if (a->used < a->size / 2) {
        a->size /= 2;
        a->data = realloc(a->data, a->size * sizeof(void *));
    }
    return a->used;
}

void free_vector(vector *a) {
    free(a->data);
    a->data = NULL;
    a->used = a->size = 0;
}

void print_vector(vector *a) {
    for (uint32_t i = 0; i < a->size; i++) {
        printf("%p\n", a->data[i]);
    }
    printf("\n");
}