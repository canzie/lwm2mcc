/**
 * @file min_heap.h
 * @brief Generic min-heap with a user-provided comparator. Elements are stored inline.
 */

#ifndef LWM2MCC__MIN_HEAP_H
#define LWM2MCC__MIN_HEAP_H

#include "lwm2m/memory.h"

#include <stddef.h>
#include <stdint.h>

typedef struct min_heap min_heap_t;

typedef int32_t (*min_heap_cmp_fn)(const void *a, const void *b);

/**
 * @brief Create a min-heap
 * @param elem_size Size of each element in bytes
 * @param elem_align Alignment requirement of each element
 * @param cmp Comparator, returns negative if a < b, 0 if equal, positive if a > b
 * @param alloc Allocator, must outlive the heap
 * @return Heap pointer
 */
min_heap_t *min_heap_create(size_t elem_size, size_t elem_align, min_heap_cmp_fn cmp,
                             const lwm2mcc_allocator_t *alloc);

/**
 * @brief Destroy a heap and free all memory
 * @param heap Heap to destroy, NULL-safe
 */
void min_heap_destroy(min_heap_t *heap);

/**
 * @brief Insert a copy of an element
 * @param heap Heap
 * @param elem Element to copy in
 */
void min_heap_insert(min_heap_t *heap, const void *elem);

/**
 * @brief Return a pointer to the minimum element without removing it
 * @param heap Heap
 * @return Pointer to the minimum element, or NULL if the heap is empty
 */
void *min_heap_peek(const min_heap_t *heap);

/**
 * @brief Remove the minimum element
 * @param heap Heap
 */
void min_heap_pop(min_heap_t *heap);

/**
 * @brief Return a pointer to the element at a given index
 * @param heap Heap
 * @param index Index into the backing array (0-based)
 * @return Pointer to the element, or NULL if out of bounds
 */
void *min_heap_at(const min_heap_t *heap, uint32_t index);

/**
 * @brief Remove the element at a given index and restore the heap property
 * @param heap Heap
 * @param index Index of the element to remove
 */
void min_heap_remove_at(min_heap_t *heap, uint32_t index);

/**
 * @brief Return the number of elements in the heap
 * @param heap Heap
 * @return Element count
 */
uint32_t min_heap_size(const min_heap_t *heap);

#endif /* LWM2MCC__MIN_HEAP_H */
