#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"

bool vector_initialize(const size_t capacity, const size_t e_size, struct vector *vec)
{
    if (capacity == 0) {
        fprintf(stderr, "capacity is 0 at vector_initialize()\n");
        return false;
    }

    if (e_size == 0) {
        fprintf(stderr, "element size is 0 at vector_initialize()\n");
        return false;
    }

    vec->items = malloc(e_size * capacity);
    if (!vec->items) {
        fprintf(stderr, "malloc failed at vector_initialize()\n");
        return false;
    }
    vec->e_size = e_size;
    memset(vec->items, 0, e_size);
    vec->capacity = capacity;

    return true;
}

bool vector_search_element(const struct vector *vec, const void *key, void *element, cmp_func cmp)
{
    if (!vec) {
        fprintf(stderr, "vector is null at vector_search_element()\n");
        return false;
    }

    if (!vec->items) {
        fprintf(stderr, "vector is empty at vector_search_element()\n");
        return false;
    }

    if (!key) {
        fprintf(stderr, "key is null at vector_search_element()\n");
        return false;
    }

    if (!cmp) {
        fprintf(stderr, "comparision function is null at vector_search_element()\n");
        return false;
    }

    for (size_t i = 0; i < vec->size; i++) {
        void *vec_element = (char *)vec->items + i * vec->e_size;
        if (cmp(vec_element, key)) {
            if (element) {
                memcpy(element, vec_element, vec->e_size);
            }
            return true;
        }
    }

    return false;
}

bool vector_get_element(const struct vector *vec, const size_t index, void *element)
{
    if (!vec) {
        fprintf(stderr, "vector is null at vector_get_element()\n");
        return false;
    }

    if (!element) {
        fprintf(stderr, "element is null at vector_get_element()\n");
        return false;
    }

    if (index >= vec->size) {
        fprintf(stderr, "index is greater than vector size at vector_get_element()\n");
        return false;
    }

    void *src = (char *)vec->items + (index * vec->e_size);
    memcpy(element, src, vec->e_size);

    return true;
}

bool vector_push_back(struct vector *vec, const void *element)
{
    if (!vec) {
        fprintf(stderr, "vector is null at vector_push_back()\n");
        return false;
    }

    if (!element) {
        fprintf(stderr, "element is null at vector_push_back()\n");
        return false;
    }

    if (vec->size >= vec->capacity) {
        // Avoid multiplying zero
        vec->capacity = vec->capacity ? vec->capacity * 2 : 1;

        void *new_block = realloc(vec->items, vec->capacity * vec->e_size);
        if (!new_block) {
            fprintf(stderr, "realloc failed at vector_push_back()\n");
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

bool vector_pop_search(struct vector *vec, const void *element)
{
    if (!vec) {
        fprintf(stderr, "vector is null at vector_pop_search()\n");
        return false;
    }

    if (!element) {
        fprintf(stderr, "element is null at vector_pop_search()\n");
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

    return false;
}

bool vector_pop_index(struct vector *vec, const size_t index, void *element)
{
    if (!vec) {
        fprintf(stderr, "vector is null at vector_pop_index()\n");
        return false;
    }

    if (!element) {
        fprintf(stderr, "element is null at vector_pop_index()\n");
        return false;
    }

    if (index >= vec->size) {
        fprintf(stderr, "index is out of bounds at vector_pop_index()\n");
        return false;
    }

    // Get pointer to the element to pop
    void *ele_ptr = (char *)vec->items + (index * vec->e_size);
    memcpy(element, ele_ptr, vec->e_size); // Copy to output

    // Shift remaining elements left
    if (index < vec->size - 1) {
        void *src = (char *)vec->items + ((index + 1) * vec->e_size);
        memmove(ele_ptr, src, (vec->size - index - 1) * vec->e_size);
    }

    vec->size--;
    return true;
}

void vector_deinitialize(struct vector *vec)
{
    if (!vec) {
        return;
    }
    
    free(vec->items);
    vec->items = NULL;
    vec->e_size = 0;
    vec->size = 0;
    vec->capacity = 0;
}