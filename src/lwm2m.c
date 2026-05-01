/**
 * @file lwm2m.c
 * @brief Client context creation and destruction
 */

#include "lwm2m/lwm2m.h"

#include <string.h>

struct lwm2mcc_context {
    lwm2mcc_allocator_t allocator;
};

lwm2mcc_context_t *lwm2mcc_create(const lwm2mcc_allocator_t *allocator)
{
    lwm2mcc_allocator_t alloc;
    if (allocator != NULL) {
        alloc = *allocator;
    } else {
        alloc = lwm2mcc_default_allocator();
    }

    lwm2mcc_context_t *ctx = lwm2mcc_calloc(&alloc, 1, sizeof(*ctx));
    if (ctx == NULL) {
        return NULL;
    }

    ctx->allocator = alloc;

    return ctx;
}

void lwm2mcc_destroy(lwm2mcc_context_t *ctx)
{
    if (ctx == NULL) {
        return;
    }

    lwm2mcc_allocator_t alloc = ctx->allocator;
    lwm2mcc_free(&alloc, ctx);
}
