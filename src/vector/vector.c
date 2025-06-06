#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"

struct vector create_vector(const size_t capacity, const size_t e_size)
{
    if (capacity <= 0) {
        fprintf(stderr, "capacity cannot be 0 or negative\n");
        return NULL_VECTOR;
    }

    if (e_size <= 0) {
        fprintf(stderr, "element size cannot be 0 or negative\n");
        return NULL_VECTOR;
    }

    struct vector vec = NULL_VECTOR;
    vec.items = malloc(e_size * capacity);
    if (!vec.items) {
        fprintf(stderr, "failed to allocated memory for vector.\n");
        return NULL_VECTOR;
    }
    vec.e_size = e_size;
    memset(vec.items, 0, e_size);
    vec.capacity = capacity;
    return vec;
}

const bool push_back(struct vector *vec, const void *element)
{
    if (!vec) {
        fprintf(stderr, "vector is null, cannot push\n");
        return false;
    }

    if (!element) {
        fprintf(stderr, "element is null, cannot push\n");
        return false;
    }

    if (vec->size >= vec->capacity) {
        // Avoid multiplying zero
        vec->capacity = vec->capacity ? vec->capacity * 2 : 1;

        void *new_block = realloc(vec->items, vec->capacity * vec->e_size);
        if (!new_block) {
            fprintf(stderr, "failed to extend vector\n");
            return false;
        }
        vec->items = new_block;
    }
    
    size_t offset = vec->size * vec->e_size;
    void *dest = (char *)vec->items + offset; // char* for advancing 1 byte
    memcpy(dest, element, vec->e_size);
    vec->size++;

    return true;
}

/*
removing C

copying from right (higher memory address) to left (lower memory address)

[A, B, C, D, E] -> [A, B, D, D, E] -> [A, B, D, E, E] -> [A, B, D, E, E] -> vec->size--
                          ^ memcpy              ^ memcpy
*/

const bool pop_search(struct vector *vec, const void *element)
{
    if (!vec) {
        fprintf(stderr, "vector is null, cannot pop\n");
        return false;
    }

    if (!element) {
        fprintf(stderr, "element is null, cannot pop\n");
        return false;
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
            return true;  // Remove only the first match
        }
    }

    return true;
}

const bool check_null_vector(struct vector *vec)
{
    if (vec->capacity == 0 || vec->e_size == 0 || vec->items == 0) {
        return true;
    }
    return false;
}

const bool free_vector(struct vector *vec)
{
    if (!vec) {
        return false;
    }
    
    free(vec->items);
    vec->items = NULL;
    vec->e_size = 0;
    vec->size = 0;
    vec->capacity = 0;

    return true;
}