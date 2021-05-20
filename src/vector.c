#include "stdint.h"
#include "vector.h"
#include "stdlib.h"
#include "stdio.h"

vector *create_vector(size_t size) {
    vector *v = calloc(1, sizeof(vector));
    v->data = calloc(size, sizeof(void *));
    v->last_idx = 0;
    v->current_idx = 0;
    v->size = size;
    return v;
}

void *get(vector *v, size_t i) {
    return v->data[i];
}

void append_vector(vector *v, void *element) {
    // Not enough space, up-size the vector
    if (v->last_idx == v->size) {
        v->size *= 2;
        v->data = realloc(v->data, v->size * sizeof(void *));
    }
    if (v->current_idx == v->last_idx) {
        v->data[v->current_idx++] = element;
        v->last_idx++;
    } else {
        v->data[v->current_idx] = element;
        uint32_t i = v->current_idx;
        // Set current_idx to next empty cell
        while (v->data[i] != NULL && i < v->size) {
            i++;
        }
        v->current_idx = i;
    }

}

size_t remove_vector(vector *v, void *element) {
    for (uint32_t i = 0; i < v->size; i++) {
        if (v->data[i] == element) {
            v->data[i] = NULL;
            // Last element removed => reduce last_idx partition
            if (v->last_idx == v->size - 1) {
                v->last_idx--;
            }
            // Set current_idx to the lower empty index
            if (i < v->last_idx) {
                v->current_idx = i;
            }
            break;
        }
    }

    // Too much space, down-size the vector
    if (v->last_idx < v->size / 2) {
        v->size /= 2;
        v->data = realloc(v->data, v->size * sizeof(void *));
    }
    return v->last_idx;
}

void free_vector(vector *v) {
    free(v->data);
    free(v);
}

void print_vector(vector *v) {
    for (uint32_t i = 0; i < v->size; i++) {
        printf("%p\n", v->data[i]);
    }
    printf("\n");
}