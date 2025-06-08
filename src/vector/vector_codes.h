#pragma once

#define VECTOR_ERRORS          \
    X(OK_VECTOR,        0)     \
    X(ERR_MALLOC_VEC,       -1337) \
    X(ERR_REALLOC,      -796)  \
    X(ERR_VEC_UNKNOWN,  -5)    \
    X(ERR_VEC_CAPACITY, -4)    \
    X(ERR_VEC_E_SIZE,   -3)    \
    X(ERR_VEC_NULL,     -2)    \
    X(ERR_VEC_E,         -1)