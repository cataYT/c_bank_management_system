#include "vector.h"

vector create_vector(unsigned int capacity, size_t e_size)
{
    vector vec = {NULL, 0, 0, e_size};
    vec.items = malloc(e_size * capacity);
    if (!vec.items)
    {
        perror("failed to allocated memory for vector.\n");
        exit(1);
    }
    vec.capacity = capacity;
    return vec;
}

void push_back(vector* vec, void* element)
{
    if (vec->size >= vec->capacity)
    {
        // Avoid multiplying zero
        vec->capacity = vec->capacity ? vec->capacity * 2 : 1;

        void* new_block = realloc(vec->items, vec->capacity * vec->e_size);
        if (!new_block)
        {
            perror("failed to extend vector");
            exit(1);
        }
        vec->items = new_block;
    }
    
    size_t offset = vec->size * vec->e_size;
    void* dest = (char*)vec->items + offset; // char* for advancing 1 byte
    memcpy(dest, element, vec->e_size);
    vec->size++;
}

/*
removing C

copying from right (higher memory address) to left (lower memory address)

[A, B, C, D, E] -> [A, B, D, D, E] -> [A, B, D, E, E] -> [A, B, D, E, E] -> vec->size--
                          ^ memcpy              ^ memcpy
*/

void pop_search(vector* vec, void* element)
{
    size_t i = 0;
    size_t j;
    for (i; i < vec->size; i++)
    {
        void* current = (char*)vec->items + i * vec->e_size;
        // Compare current element with the target element
        if (memcmp(current, element, vec->e_size) == 0)
        {
            // Found the element to remove

            // Shift all elements after i one slot to the left
            for (j = i + 1; j < vec->size; j++)
            {
                size_t byte_offset = j * vec->e_size;
                void* src = (char*)vec->items + byte_offset;            // calculates address at index j
                void* dest = (char*)vec->items + (j - 1) * vec->e_size; // calculates address at index just before j
                memmove(dest, src, vec->e_size);
            }

            vec->size--;
            return;  // Remove only the first match
        }
    }
}

unsigned int vector_length(vector* vec)
{
    return vec->size;
}

void free_vector(vector* vec)
{
    if (!vec->items)
    {
        return;
    }
    
    free(vec->items);
    vec->items = NULL;
    vec->capacity = 0;
    vec->e_size = 0;
    vec->size = 0;
}