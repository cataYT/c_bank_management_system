#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "vector_codes.h"

struct vector
{
    void *items;
    size_t e_size;
    size_t size;
    size_t capacity;
};

enum vector_error {
#define X(name, value) name = value,
    VECTOR_ERRORS
#undef X
};

enum vector_error create_vector(const size_t capacity, const size_t e_size, struct vector *vec);
enum vector_error push_back(struct vector *vec, const void* element);
enum vector_error pop_search(struct vector *vec, const void* element);
bool vector_is_null(struct vector *vec);
enum vector_error free_vector(struct vector *vec);