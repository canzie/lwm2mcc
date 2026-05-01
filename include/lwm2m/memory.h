/**
 * @file memory.h
 * @brief Pluggable memory allocator interface
 */

#ifndef LWM2MCC__MEMORY_H
#define LWM2MCC__MEMORY_H

#include <stddef.h>

/**
 * @brief Allocator function pointers
 *
 * All callbacks receive the user_ctx pointer for custom allocator state
 * free must accept NULL safely
 */
typedef struct {
    void *(*alloc)(size_t size, void *user_ctx);
    void *(*calloc)(size_t nmemb, size_t size, void *user_ctx);
    void *(*realloc)(void *ptr, size_t size, void *user_ctx);
    void (*free)(void *ptr, void *user_ctx);
    void *user_ctx;
} lwm2mcc_allocator_t;

/**
 * @brief Returns the default allocator (stdlib malloc/calloc/realloc/free)
 */
lwm2mcc_allocator_t lwm2mcc_default_allocator(void);

/**
 * @brief Allocate memory using the given allocator
 */
static inline void *lwm2mcc_alloc(const lwm2mcc_allocator_t *a, size_t size)
{
    return a->alloc(size, a->user_ctx);
}

/**
 * @brief Allocate zero-initialized memory using the given allocator
 */
static inline void *lwm2mcc_calloc(const lwm2mcc_allocator_t *a, size_t nmemb, size_t size)
{
    return a->calloc(nmemb, size, a->user_ctx);
}

/**
 * @brief Reallocate memory using the given allocator
 */
static inline void *lwm2mcc_realloc(const lwm2mcc_allocator_t *a, void *ptr, size_t size)
{
    return a->realloc(ptr, size, a->user_ctx);
}

/**
 * @brief Free memory using the given allocator
 */
static inline void lwm2mcc_free(const lwm2mcc_allocator_t *a, void *ptr)
{
    a->free(ptr, a->user_ctx);
}

#endif /* LWM2MCC__MEMORY_H */
