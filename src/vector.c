#include "stdint.h"
#include "vector.h"
#include <stdlib.h>
#include "stdio.h"
#include "ei_widget.h"

vector *create_vector(uint32_t size) {
    vector *v = calloc(1, sizeof(vector));
    v->data = calloc(size, sizeof(void *));
    v->last_idx = 0;
    v->current_idx = 0;
    v->size = size;
    return v;
}

void *get(vector *v, uint32_t i) {
    ei_widget_t *tmp = (ei_widget_t *) v;
    v = v->data[i];
    return v;
}

uint32_t append_vector(vector *v, void *element) {
    // Not enough space, up-size the vector
    if (v->last_idx == v->size) {
        v->size *= 2;
        resize(v, v->size);
    }

    // Same idx => move both
    if (v->current_idx == v->last_idx) {
        v->data[v->current_idx] = element;
        v->last_idx++;
        return v->current_idx++;
    }

    // Different idx => move current_idx to next empty cell
    v->data[v->current_idx] = element;
    uint32_t append_idx = v->current_idx;
    while (v->data[v->current_idx] != NULL && v->current_idx < v->last_idx) {
        v->current_idx++;
    }

    return append_idx;
}

uint32_t remove_vector(vector *v, void *element) {
    for (uint32_t i = 0; i < v->size; i++) {
        if (v->data[i] == element) {
            v->data[i] = NULL;
            // Last element removed => reduce last_idx partition
            if (v->last_idx == i + 1) {
                while (v->last_idx > 0 && v->data[v->last_idx - 1] == NULL) {
                    v->last_idx--;
                }
            }
            // Set current_idx to the lower empty index
            if (i < v->last_idx) {
                v->current_idx = i;
            }
            break;
        }
    }

    // Too much space, down-size the vector
    if (v->last_idx < v->size / 4) {
        v->size /= 2;
        resize(v, v->size);
    }
    return v->last_idx;
}

void concatenation(vector *a, vector *b) {
    for (uint32_t i = 0; i < b->last_idx; i++) {
        append_vector(a, get(b, i));
    }
}

void copy(vector *a, vector *b) {
    resize(b, a->size);
    b->last_idx = a->last_idx;
    b->size = a->size;
    b->current_idx = a->current_idx;

    for (uint32_t i = 0; i < b->last_idx; i++) {
        b->data[i] = a->data[i];
    }
}

void clear(vector *v, uint32_t *size) {
    if (size != NULL) {
        resize(v, *size);
    }
    for (uint32_t i = 0; i < v->last_idx; i++) {
        v->data[i] = NULL;
    }
    v->last_idx = 0;
    v->current_idx = 0;
    v->size = *size;
}

void resize(vector *v, uint32_t size) {
    v->data = realloc(v->data, size * sizeof(void *));
    v->size = size;
}

void upsize(vector *v, uint32_t i) {
    while (v->size <= i) {
        v->size *= 2;
        resize(v, v->size);
    }
}

void downsize(vector *v) {

}

void free_vector(vector *v) {
    free(v->data);
    free(v);
}

void print_vector(vector *v) {
    for (uint32_t i = 0; i < v->size; i++) {
        if (v->data[i] != NULL) {
            printf("%i\n", *((int *) v->data[i]));
        } else {
            printf("null\n");
        }
    }
    printf("\n");
}