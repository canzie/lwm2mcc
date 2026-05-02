/**
 * @file sorted_array.c
 * @brief Sorted array implementation
 */

#include "sorted_array.h"

#include <string.h>

#define INITIAL_CAPACITY 4
#define GROWTH_FACTOR    2

struct sorted_array {
    void *data;
    size_t elem_size;
    size_t stride;
    sorted_array_cmp_fn cmp;
    uint32_t count;
    uint32_t capacity;
    const lwm2mcc_allocator_t *alloc;
};

static void *s_elem_at(const sorted_array_t *arr, uint32_t index)
{
    return (char *)arr->data + (size_t)index * arr->stride;
}

/**
 * @brief Binary search for an element
 * @param arr Array to search
 * @param elem Element to compare against
 * @param[out] out_pos Index of match, or insertion point if not found
 * @return true if found, false if not
 */
static bool s_binary_search(const sorted_array_t *arr, const void *elem, uint32_t *out_pos)
{
    uint32_t lo = 0;
    uint32_t hi = arr->count;

    while (lo < hi) {
        uint32_t mid = lo + (hi - lo) / 2;
        int32_t result = arr->cmp(s_elem_at(arr, mid), elem);

        if (result < 0) {
            lo = mid + 1;
        } else if (result > 0) {
            hi = mid;
        } else {
            *out_pos = mid;
            return true;
        }
    }

    *out_pos = lo;
    return false;
}

static void s_ensure_capacity(sorted_array_t *arr, uint32_t needed)
{
    if (needed <= arr->capacity) {
        return;
    }

    uint32_t new_cap = arr->capacity;
    if (new_cap == 0) {
        new_cap = INITIAL_CAPACITY;
    }
    while (new_cap < needed) {
        new_cap *= GROWTH_FACTOR;
    }

    arr->data = lwm2mcc_realloc(arr->alloc, arr->data, (size_t)new_cap * arr->stride);
    arr->capacity = new_cap;
}

sorted_array_t *sorted_array_create(size_t elem_size, size_t elem_align, sorted_array_cmp_fn cmp, const lwm2mcc_allocator_t *alloc)
{
    sorted_array_t *arr = lwm2mcc_alloc(alloc, sizeof(*arr));
    size_t stride = (elem_size + elem_align - 1) & ~(elem_align - 1);

    *arr = (sorted_array_t){
        .data = NULL,
        .elem_size = elem_size,
        .stride = stride,
        .cmp = cmp,
        .count = 0,
        .capacity = 0,
        .alloc = alloc,
    };

    return arr;
}

void sorted_array_destroy(sorted_array_t *arr)
{
    if (arr == NULL) {
        return;
    }

    const lwm2mcc_allocator_t *alloc = arr->alloc;
    lwm2mcc_free(alloc, arr->data);
    lwm2mcc_free(alloc, arr);
}

bool sorted_array_insert(sorted_array_t *arr, const void *elem)
{
    uint32_t pos;
    if (s_binary_search(arr, elem, &pos)) {
        return false;
    }

    s_ensure_capacity(arr, arr->count + 1);

    if (pos < arr->count) {
        memmove(s_elem_at(arr, pos + 1), s_elem_at(arr, pos), (size_t)(arr->count - pos) * arr->stride);
    }

    memcpy(s_elem_at(arr, pos), elem, arr->elem_size);
    arr->count++;
    return true;
}

bool sorted_array_remove(sorted_array_t *arr, const void *elem)
{
    uint32_t pos;
    if (!s_binary_search(arr, elem, &pos)) {
        return false;
    }

    uint32_t remaining = arr->count - pos - 1;
    if (remaining > 0) {
        memmove(s_elem_at(arr, pos), s_elem_at(arr, pos + 1), (size_t)remaining * arr->stride);
    }

    arr->count--;
    return true;
}

uint32_t sorted_array_search(const sorted_array_t *arr, const void *elem)
{
    uint32_t pos;
    if (!s_binary_search(arr, elem, &pos)) {
        return SORTED_ARRAY_INVALID_INDEX;
    }
    return pos;
}

uint32_t sorted_array_count(const sorted_array_t *arr)
{
    return arr->count;
}

void *sorted_array_at(const sorted_array_t *arr, uint32_t index)
{
    if (index >= arr->count) {
        return NULL;
    }
    return s_elem_at(arr, index);
}
