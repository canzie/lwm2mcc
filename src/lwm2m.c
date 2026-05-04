/**
 * @file lwm2m.c
 * @brief Client context creation and destruction
 */

#include "lwm2m/lwm2m.h"

#include "lwm2m_internal.h"
#include "lwm2m/object.h"
#include "lwm2m/utils/common.h"
#include "tools/sorted_array.h"

#include <string.h>

extern const lwm2mcc_object_def_t *lwm2mcc_builtin_security_def(void);
extern const lwm2mcc_object_def_t *lwm2mcc_builtin_server_def(void);

struct lwm2mcc_context {
    lwm2mcc_allocator_t allocator;
    sorted_array_t *objects;
};

const lwm2mcc_allocator_t *_lwm2mcc_allocator(const lwm2mcc_context_t *ctx)
{
    return &ctx->allocator;
}

sorted_array_t **_lwm2mcc_objects(lwm2mcc_context_t *ctx)
{
    return &ctx->objects;
}

sorted_array_t *_lwm2mcc_registered_objects(const lwm2mcc_context_t *ctx)
{
    return ctx->objects;
}

lwm2mcc_result_t lwm2mcc_create(const lwm2mcc_allocator_t *allocator, lwm2mcc_context_t **ctx)
{
    lwm2mcc_allocator_t alloc;
    if (allocator != NULL) {
        alloc = *allocator;
    } else {
        alloc = lwm2mcc_default_allocator();
    }

    lwm2mcc_context_t *new_ctx = lwm2mcc_calloc(&alloc, 1, sizeof(*new_ctx));
    new_ctx->allocator = alloc;
    *ctx = new_ctx;
    return LWM2MCC_SUCCESS;
}

lwm2mcc_result_t lwm2mcc_destroy(lwm2mcc_context_t *ctx)
{
    if (ctx == NULL) {
        return LWM2MCC_SUCCESS;
    }

    sorted_array_destroy(ctx->objects);

    lwm2mcc_allocator_t alloc = ctx->allocator;
    lwm2mcc_free(&alloc, ctx);

    return LWM2MCC_SUCCESS;
}

lwm2mcc_result_t lwm2mcc_allocator(const lwm2mcc_context_t *ctx, const lwm2mcc_allocator_t **allocator)
{
    *allocator = _lwm2mcc_allocator(ctx);
    return LWM2MCC_SUCCESS;
}

lwm2mcc_result_t lwm2mcc_registered_objects(const lwm2mcc_context_t *ctx, sorted_array_t **objects)
{
    *objects = _lwm2mcc_registered_objects(ctx);
    return LWM2MCC_SUCCESS;
}

lwm2mcc_result_t lwm2mcc_register_builtin_objects(lwm2mcc_context_t *ctx)
{
    lwm2mcc_object_register(ctx, lwm2mcc_builtin_security_def());
    lwm2mcc_object_register(ctx, lwm2mcc_builtin_server_def());
    return LWM2MCC_SUCCESS;
}
