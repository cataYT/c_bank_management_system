#pragma once

#include <stdlib.h>
#include <stdbool.h>

struct vector
{
    void *items;
    size_t e_size;
    size_t size;
    size_t capacity;
};

bool create_vector(const size_t capacity, const size_t e_size, struct vector *vec);
bool push_back(struct vector *vec, const void* element);
bool pop_search(struct vector *vec, const void* element);
bool free_vector(struct vector *vec);