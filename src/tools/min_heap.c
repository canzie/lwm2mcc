/**
 * @file min_heap.c
 * @brief Generic min-heap implementation
 */

#include "min_heap.h"

#include "../utils/assert.h"

#include <stdbool.h>
#include <string.h>

#define INITIAL_CAPACITY 4
#define GROWTH_FACTOR    2

struct min_heap {
    void *data;
    size_t elem_size;
    size_t stride;
    min_heap_cmp_fn cmp;
    uint32_t count;
    uint32_t capacity;
    const lwm2mcc_allocator_t *alloc;
};

static void *s_elem_at(const min_heap_t *heap, uint32_t index)
{
    return (char *)heap->data + (size_t)index * heap->stride;
}

static void s_swap(min_heap_t *heap, uint32_t i, uint32_t j)
{
    char tmp[heap->elem_size];
    void *a = s_elem_at(heap, i);
    void *b = s_elem_at(heap, j);
    memcpy(tmp, a, heap->elem_size);
    memcpy(a, b, heap->elem_size);
    memcpy(b, tmp, heap->elem_size);
}

static void s_sift_up(min_heap_t *heap, uint32_t i)
{
    while (i > 0) {
        uint32_t parent = (i - 1) / 2;
        if (heap->cmp(s_elem_at(heap, parent), s_elem_at(heap, i)) <= 0) {
            break;
        }
        s_swap(heap, parent, i);
        i = parent;
    }
}

static void s_sift_down(min_heap_t *heap, uint32_t i)
{
    while (true) {
        uint32_t smallest = i;
        uint32_t left = 2 * i + 1;
        uint32_t right = 2 * i + 2;

        if (left < heap->count && heap->cmp(s_elem_at(heap, left), s_elem_at(heap, smallest)) < 0) {
            smallest = left;
        }
        if (right < heap->count && heap->cmp(s_elem_at(heap, right), s_elem_at(heap, smallest)) < 0) {
            smallest = right;
        }
        if (smallest == i) {
            break;
        }
        s_swap(heap, i, smallest);
        i = smallest;
    }
}

static void s_ensure_capacity(min_heap_t *heap, uint32_t needed)
{
    if (needed <= heap->capacity) {
        return;
    }

    uint32_t new_cap = heap->capacity == 0 ? INITIAL_CAPACITY : heap->capacity;
    while (new_cap < needed) {
        new_cap *= GROWTH_FACTOR;
    }

    heap->data = lwm2mcc_realloc(heap->alloc, heap->data, (size_t)new_cap * heap->stride);
    heap->capacity = new_cap;
}

min_heap_t *min_heap_create(size_t elem_size, size_t elem_align, min_heap_cmp_fn cmp,
                             const lwm2mcc_allocator_t *alloc)
{
    min_heap_t *heap = lwm2mcc_alloc(alloc, sizeof(min_heap_t));
    size_t stride = (elem_size + elem_align - 1) & ~(elem_align - 1);

    *heap = (min_heap_t){
        .data = NULL,
        .elem_size = elem_size,
        .stride = stride,
        .cmp = cmp,
        .count = 0,
        .capacity = 0,
        .alloc = alloc,
    };

    return heap;
}

void min_heap_destroy(min_heap_t *heap)
{
    if (heap == NULL) {
        return;
    }

    const lwm2mcc_allocator_t *alloc = heap->alloc;
    lwm2mcc_free(alloc, heap->data);
    lwm2mcc_free(alloc, heap);
}

void min_heap_insert(min_heap_t *heap, const void *elem)
{
    LWM2MCC_ASSERT(heap != NULL, "");
    s_ensure_capacity(heap, heap->count + 1);
    memcpy(s_elem_at(heap, heap->count), elem, heap->elem_size);
    s_sift_up(heap, heap->count);
    heap->count++;
}

void *min_heap_peek(const min_heap_t *heap)
{
    if (heap->count == 0) {
        return NULL;
    }
    return s_elem_at(heap, 0);
}

void min_heap_pop(min_heap_t *heap)
{
    LWM2MCC_ASSERT(heap != NULL, "");
    if (heap->count == 0) {
        return;
    }
    min_heap_remove_at(heap, 0);
}

void *min_heap_at(const min_heap_t *heap, uint32_t index)
{
    if (index >= heap->count) {
        return NULL;
    }
    return s_elem_at(heap, index);
}

void min_heap_remove_at(min_heap_t *heap, uint32_t index)
{
    LWM2MCC_ASSERT(heap != NULL, "");
    if (index >= heap->count) {
        return;
    }

    heap->count--;
    if (index < heap->count) {
        memcpy(s_elem_at(heap, index), s_elem_at(heap, heap->count), heap->elem_size);
        s_sift_up(heap, index);
        s_sift_down(heap, index);
    }
}

uint32_t min_heap_size(const min_heap_t *heap)
{
    return heap->count;
}
