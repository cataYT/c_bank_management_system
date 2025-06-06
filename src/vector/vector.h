#pragma once

#include <stdlib.h>
#include <stdbool.h>

struct vector {
    void *items;
    size_t e_size;
    size_t size;
    size_t capacity;
};

#define NULL_VECTOR (struct vector){NULL, 0, 0, 0}

struct vector create_vector(const size_t capacity, const size_t e_size);
const bool push_back(struct vector *vec, const void *element);
const bool pop_search(struct vector *vec, const void *element);
const bool check_null_vector(struct vector *vec);
const bool free_vector(struct vector *vec);