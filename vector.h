#pragma once

#include <stdlib.h>

struct vector
{
    void* items;
    size_t e_size;
    unsigned int size;
    unsigned int capacity;
};

struct vector create_vector(const unsigned int capacity, const size_t e_size);
void push_back(struct vector* vec, const void* element);
void pop_search(struct vector* vec, const void* element);
void free_vector(struct vector* vec);