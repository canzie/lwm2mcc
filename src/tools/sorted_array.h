/**
 * @file sorted_array.h
 * @brief Generic sorted array with binary search, using a user-provided
 *        comparator. Elements are stored inline.
 */

#ifndef LWM2MCC__SORTED_ARRAY_H
#define LWM2MCC__SORTED_ARRAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lwm2m/memory.h"

typedef struct sorted_array sorted_array_t;

#define SORTED_ARRAY_INVALID_INDEX UINT32_MAX

/**
 * @brief Comparator function
 * @param a First element
 * @param b Second element
 * @return Negative if a < b, 0 if equal, positive if a > b
 */
typedef int32_t (*sorted_array_cmp_fn)(const void *a, const void *b);

/**
 * @brief Create a sorted array
 * @param elem_size Size of each element
 * @param elem_align Alignment requirement of each element
 * @param cmp Comparator function
 * @param alloc Allocator (stored internally, must outlive the array)
 * @return Array pointer
 */
sorted_array_t *sorted_array_create(size_t elem_size, size_t elem_align, sorted_array_cmp_fn cmp, const lwm2mcc_allocator_t *alloc);

/**
 * @brief Destroy a sorted array and free all memory
 * @param arr Array to destroy (NULL-safe)
 */
void sorted_array_destroy(sorted_array_t *arr);

/**
 * @brief Insert a copy of an element in sorted order
 * @param arr Array
 * @param elem Element to copy in
 * @return true on success, false if an equal element already exists
 */
bool sorted_array_insert(sorted_array_t *arr, const void *elem);

/**
 * @brief Remove an element
 * @param arr Array
 * @param elem Element to compare against
 * @return true on success, false if not found
 * @note Caller must clean up element internals before removing
 */
bool sorted_array_remove(sorted_array_t *arr, const void *elem);

/**
 * @brief Binary search for an element
 * @param arr Array
 * @param elem Element to compare against
 * @return Index of matching element, or SORTED_ARRAY_INVALID_INDEX
 */
uint32_t sorted_array_search(const sorted_array_t *arr, const void *elem);

/**
 * @brief Get element count
 * @param arr Array
 * @return Number of elements
 */
uint32_t sorted_array_count(const sorted_array_t *arr);

/**
 * @brief Get pointer to element at index
 * @param arr Array
 * @param index Index (0-based)
 * @return Pointer to element, or NULL if out of bounds
 */
void *sorted_array_at(const sorted_array_t *arr, uint32_t index);

#endif /* LWM2MCC__SORTED_ARRAY_H */
