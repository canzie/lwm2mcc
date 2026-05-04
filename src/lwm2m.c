/**
 * @file lwm2m.c
 * @brief Client context creation and destruction
 */

#include "lwm2m/lwm2m.h"

#include "lwm2m_internal.h"
#include "lwm2m/object.h"
#include "lwm2m/utils/common.h"
#include "tools/timers.h"
#include "tools/sorted_array.h"
#include "utils/assert.h"

#include <string.h>

extern const lwm2mcc_object_def_t *lwm2mcc_builtin_security_def(void);
extern const lwm2mcc_object_def_t *lwm2mcc_builtin_server_def(void);

struct lwm2mcc_context {
    lwm2mcc_allocator_t allocator;
    lwm2mcc_transport_ops_t transport;
    lwm2mcc_security_ops_t security;
    lwm2mcc_platform_ops_t platform;
    sorted_array_t *objects;
    lwm2mcc_timers_t *timers;
};

const lwm2mcc_allocator_t *_lwm2mcc_allocator(const lwm2mcc_context_t *ctx)
{
    return &ctx->allocator;
}

lwm2mcc_transport_ops_t *_lwm2mcc_transport(lwm2mcc_context_t *ctx)
{
    return &ctx->transport;
}

sorted_array_t **_lwm2mcc_objects(lwm2mcc_context_t *ctx)
{
    return &ctx->objects;
}

sorted_array_t *_lwm2mcc_registered_objects(const lwm2mcc_context_t *ctx)
{
    return ctx->objects;
}

lwm2mcc_timers_t *_lwm2mcc_timers(lwm2mcc_context_t *ctx)
{
    return ctx->timers;
}

lwm2mcc_result_t lwm2mcc_create(const lwm2mcc_config_t *config, lwm2mcc_context_t **ctx)
{
    lwm2mcc_allocator_t alloc;
    if (config != NULL && config->allocator != NULL) {
        alloc = *config->allocator;
    } else {
        alloc = lwm2mcc_default_allocator();
    }

    *ctx = lwm2mcc_calloc(&alloc, 1, sizeof(lwm2mcc_context_t));
    (*ctx)->allocator = alloc;

    if (config != NULL) {
        if (config->transport != NULL) { (*ctx)->transport = *config->transport; }
        if (config->security != NULL) { (*ctx)->security = *config->security; }
        if (config->platform != NULL) { (*ctx)->platform = *config->platform; }
    }

    (*ctx)->timers = lwm2mcc_timers_create(&(*ctx)->platform, &(*ctx)->allocator);

    return LWM2MCC_SUCCESS;
}

lwm2mcc_result_t lwm2mcc_destroy(lwm2mcc_context_t *ctx)
{
    if (ctx == NULL) {
        return LWM2MCC_SUCCESS;
    }

    sorted_array_destroy(ctx->objects);
    lwm2mcc_timers_destroy(ctx->timers);

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

lwm2mcc_result_t lwm2mcc_start(lwm2mcc_context_t *ctx)
{
    LWM2MCC_ASSERT(ctx != NULL, "ctx is NULL");
    LWM2MCC_ASSERT(ctx->transport.open != NULL, "transport.open is required");
    LWM2MCC_ASSERT(ctx->platform.now_ms != NULL, "platform.now_ms is required");

    if (ctx->transport.open(ctx->transport.user_ctx) != 0) {
        return LWM2MCC_ERR_TRANSPORT;
    }

    return LWM2MCC_SUCCESS;
}

lwm2mcc_result_t lwm2mcc_stop(lwm2mcc_context_t *ctx)
{
    if (ctx == NULL) {
        return LWM2MCC_SUCCESS;
    }

    if (ctx->transport.close != NULL) {
        ctx->transport.close(ctx->transport.user_ctx);
    }

    return LWM2MCC_SUCCESS;
}

lwm2mcc_result_t lwm2mcc_step(lwm2mcc_context_t *ctx, uint32_t max_timeout_ms,
                               uint32_t *timeout_ms)
{
    LWM2MCC_ASSERT(ctx != NULL, "ctx is NULL");
    LWM2MCC_ASSERT(timeout_ms != NULL, "timeout_ms is NULL");

    *timeout_ms = lwm2mcc_timers_step(ctx->timers, max_timeout_ms);

    return LWM2MCC_SUCCESS;
}
