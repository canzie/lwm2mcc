/**
 * @file memory.c
 * @brief Default allocator implementation
 */

#include "lwm2m/memory.h"
#include "utils/assert.h"

#include <stdlib.h>

static void *s_alloc(size_t size, void *user_ctx)
{
    (void)user_ctx;
    void *ptr = malloc(size);
    LWM2MCC_ASSERT(ptr != NULL, "malloc failed");
    return ptr;
}

static void *s_calloc(size_t nmemb, size_t size, void *user_ctx)
{
    (void)user_ctx;
    void *ptr = calloc(nmemb, size);
    LWM2MCC_ASSERT(ptr != NULL, "calloc failed");
    return ptr;
}

static void *s_realloc(void *ptr, size_t size, void *user_ctx)
{
    (void)user_ctx;
    void *result = realloc(ptr, size);
    LWM2MCC_ASSERT(result != NULL, "realloc failed");
    return result;
}

static void s_free(void *ptr, void *user_ctx)
{
    (void)user_ctx;
    free(ptr);
}

lwm2mcc_allocator_t lwm2mcc_default_allocator(void)
{
    return (lwm2mcc_allocator_t){
        .alloc = s_alloc,
        .calloc = s_calloc,
        .realloc = s_realloc,
        .free = s_free,
        .user_ctx = NULL,
    };
}
