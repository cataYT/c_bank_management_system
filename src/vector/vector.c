#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"

enum vector_error create_vector(const size_t capacity, const size_t e_size, struct vector *vec)
{
    if (capacity == 0) {
        fprintf(stderr, "capacity is 0 at create_vector()\n");
        return ERR_VEC_CAPACITY;
    }

    if (e_size == 0) {
        fprintf(stderr, "element size is 0 at create_vector()\n");
        return ERR_VEC_E_SIZE;
    }

    vec->items = malloc(e_size * capacity);
    if (!vec->items) {
        fprintf(stderr, "malloc failed at create_vector()\n");
        return ERR_MALLOC_VEC;
    }
    vec->e_size = e_size;
    memset(vec->items, 0, e_size);
    vec->capacity = capacity;

    return OK_VECTOR;
}

enum vector_error push_back(struct vector *vec, const void *element)
{
    if (!vec) {
        fprintf(stderr, "vector is null at push_back()\n");
        return ERR_VEC_NULL;
    }

    if (!element) {
        fprintf(stderr, "element is null at push_back()\n");
        return ERR_VEC_E;
    }

    if (vec->size >= vec->capacity) {
        // Avoid multiplying zero
        vec->capacity = vec->capacity ? vec->capacity * 2 : 1;

        void *new_block = realloc(vec->items, vec->capacity * vec->e_size);
        if (!new_block) {
            fprintf(stderr, "realloc failed at push_back()\n");
            return ERR_REALLOC;
        }
        vec->items = new_block;
    }
    
    size_t offset = vec->size * vec->e_size;
    void *dest = (char *)vec->items + offset; // char* for advancing 1 byte
    memcpy(dest, element, vec->e_size);
    vec->size++;

    return OK_VECTOR;
}

/*
removing C

copying from right (higher memory address) to left (lower memory address)

[A, B, C, D, E] -> [A, B, D, D, E] -> [A, B, D, E, E] -> [A, B, D, E, E] -> vec->size--
                          ^ memcpy              ^ memcpy
*/

enum vector_error pop_search(struct vector *vec, const void *element)
{
    if (!vec) {
        fprintf(stderr, "vector is null at pop_search()\n");
        return ERR_VEC_NULL;
    }

    if (!element) {
        fprintf(stderr, "element is null at pop_search()\n");
        return ERR_VEC_E;
    }

    for (size_t i = 0; i < vec->size; i++) {
        void *current = (char*)vec->items + i * vec->e_size;
        // Compare current element with the target element
        if (memcmp(current, element, vec->e_size) == 0) {
            // Found the element to remove

            // Shift all elements after i one slot to the left
            for (size_t j = i + 1; j < vec->size; j++) {
                size_t byte_offset = j * vec->e_size;
                void *src = (char*)vec->items + byte_offset;            // calculates address at index j
                void *dest = (char*)vec->items + (j - 1) * vec->e_size; // calculates address at index just before j
                memcpy(dest, src, vec->e_size);
            }

            vec->size--;
            return OK_VECTOR;  // Remove only the first match
        }
    }

    return ERR_VEC_UNKNOWN;
}

bool vector_is_null(struct vector *vec)
{
    return !vec || !vec->items || vec->e_size == 0 || vec->capacity == 0;
}

enum vector_error free_vector(struct vector *vec)
{
    if (!vec) {
        return ERR_VEC_NULL;
    }
    
    free(vec->items);
    vec->items = NULL;
    vec->e_size = 0;
    vec->size = 0;
    vec->capacity = 0;

    return OK_VECTOR;
}