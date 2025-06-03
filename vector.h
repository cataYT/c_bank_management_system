#pragma once

#include <stdlib.h>
#include <memory.h>

typedef struct vector
{
    void* items;
    unsigned int size;
    unsigned int capacity;
    size_t e_size;
} vector;

vector create_vector(unsigned int capacity, size_t e_size);
void push_back(vector* vec, void* element);
void pop_search(vector* vec, void* element);
unsigned int vector_length(vector* vec);
void free_vector(vector* vec);