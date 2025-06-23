#pragma once

#include <stdlib.h>
#include <stdbool.h>

/**
 * @typedef cmp_func
 * @brief Function pointer type for comparing an element with a key.
 *
 * A comparison function that determines whether a given element matches
 * a key. Commonly used in search or filtering operations.
 *
 * @param element Pointer to the element being tested.
 * @param key     Pointer to the key used for comparison.
 *
 * @return true if the element matches the key, false otherwise.
 */
typedef bool (*cmp_func)(const void *element, const void *key);

/**
 * @struct A generic dynamically resizable array (vector).
 *
 * The vector stores raw memory blocks of uniform size, allowing generic storage
 * of elements. Supports automatic resizing and basic operations like insertion,
 * retrieval, search, and deletion.
 */
struct vector {
    void *items;       /** Pointer to the contiguous memory block for elements. */
    size_t e_size;     /** Size of each element in bytes. */
    size_t size;       /** Current number of elements in the vector. */
    size_t capacity;   /** Allocated capacity in elements. */
};

/**
 * @brief Initializes a vector with a specified capacity and element size.
 *
 * The user is responsible for allocating the `struct vector` and calling
 * `free_vector()` afterward to release the internal memory.
 *
 * @param[in]  capacity  Initial capacity (number of elements).
 * @param[in]  e_size    Size in bytes of each element.
 * @param[out] vec       Pointer to the vector structure to initialize.
 *
 * @return `true` on success, `false` on allocation failure or invalid arguments.
 */
bool vector_initialize(const size_t capacity, const size_t e_size, struct vector *vec);

/**
 * @brief Searches for an element in the vector using a comparison function.
 *
 * If found, and if `element` is non-NULL, the matching element is copied into it.
 *
 * @param[in]  vec      Pointer to the initialized vector.
 * @param[in]  key      Pointer to the key to search for.
 * @param[out] element  Optional pointer to store the found element.
 * @param[in]  cmp      Comparison function: returns true on match.
 *
 * @return `true` if the element is found, `false` otherwise.
 */
bool vector_search_element(const struct vector *vec, const void *key, void *element, cmp_func cmp);

/**
 * @brief Retrieves an element at a specific index.
 *
 * Copies the element into the memory pointed to by `element`.
 *
 * @param[in]  vec      Pointer to the initialized vector.
 * @param[in]  index    Index of the element to retrieve.
 * @param[out] element  Buffer to store the retrieved element (`e_size` bytes).
 *
 * @return `true` on success, `false` on invalid index or parameters.
 */
bool vector_get_element(const struct vector *vec, const size_t index, void *element);

/**
 * @brief Appends an element to the end of the vector.
 *
 * Automatically resizes (doubles capacity) if needed. The element is copied
 * using the vector’s element size.
 *
 * @param[in,out] vec      Pointer to the initialized vector.
 * @param[in]     element  Pointer to the element to append.
 *
 * @return `true` on success, `false` on allocation failure or invalid input.
 */
bool vector_push_back(struct vector *vec, const void *element);

/**
 * @brief Removes the first matching element from the vector.
 *
 * Uses `memcmp()` for element comparison across `e_size` bytes.
 * On match, the element is removed and the array is compacted.
 *
 * @param[in,out] vec      Pointer to the initialized vector.
 * @param[in]     element  Pointer to the element to remove.
 *
 * @return `true` if an element was found and removed, `false` otherwise.
 */
bool vector_pop_search(struct vector *vec, const void *element);

/**
 * @brief Removes and returns the element at the given index.
 *
 * The removed element is copied into `element` (if non-NULL),
 * and subsequent elements are shifted left.
 *
 * @param[in,out] vec      Pointer to the initialized vector.
 * @param[in]     index    Index of the element to remove.
 * @param[out]    element  Optional buffer to store the removed element.
 *
 * @return `true` on success, `false` on invalid index or input.
 */
bool vector_pop_index(struct vector *vec, const size_t index, void *element);

/**
 * @brief Frees the internal memory used by the vector.
 *
 * Resets the vector to a clean state. The struct itself is not freed.
 *
 * @param[in,out] vec  Pointer to the vector to clean up.
 */
void vector_deinitialize(struct vector *vec);