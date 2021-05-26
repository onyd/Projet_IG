#ifndef PROJETC_IG_VECTOR_H
#define PROJETC_IG_VECTOR_H

#include "stddef.h"
#include "stdint.h"

/**
 * @brief	linked structure that stores data for the picking
 */
typedef struct {
    void **data;
    uint32_t last_idx;
    uint32_t current_idx;
    uint32_t size;
} vector;

/**
 * @brief	Create a vector with the given size.
 */
vector *create_vector(uint32_t size);

/**
 * @brief	return the content in the i case of the vector.
 */
void *get(vector *v, uint32_t i);

/**
 * @brief	append an element in a vector a.
 */
uint32_t append_vector(vector *a, void *element);

/**
 * @brief	Remove an element in a vector a.
 */
uint32_t remove_vector(vector *a, void *element);


/**
 * @brief	Resize a vector if he is full or too empty.
 */
void resize(vector *v, uint32_t size);

/**
 * @brief	Free a vector.
 */
void free_vector(vector *a);

void print_vector(vector *a);

#endif //PROJETC_IG_VECTOR_H
