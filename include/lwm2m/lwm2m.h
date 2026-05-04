/**
 * @file lwm2m.h
 * @brief LWM2MCC - LWM2M C Client library entry point
 */

#ifndef LWM2MCC__LWM2M_H
#define LWM2MCC__LWM2M_H

#include "lwm2m/memory.h"
#include "lwm2m/platform.h"
#include "lwm2m/security.h"
#include "lwm2m/transport.h"
#include "lwm2m/utils/common.h"
#include "lwm2m/version.h"

/** @brief Opaque client context */
typedef struct lwm2mcc_context lwm2mcc_context_t;

typedef struct sorted_array sorted_array_t;

/**
 * @brief Client configuration passed to lwm2mcc_create
 *
 * transport is required for any network operation. security and platform
 * may be NULL when the corresponding capability is not needed.
 */
typedef struct {
    const lwm2mcc_allocator_t  *allocator;
    lwm2mcc_transport_ops_t    *transport;
    lwm2mcc_security_ops_t     *security;
    lwm2mcc_platform_ops_t     *platform;
} lwm2mcc_config_t;

/**
 * @brief Create a new LWM2M client context
 * @param config Client configuration
 * @param[out] ctx Output context pointer
 * @return LWM2MCC_SUCCESS
 */
lwm2mcc_result_t lwm2mcc_create(const lwm2mcc_config_t *config, lwm2mcc_context_t **ctx);

/**
 * @brief Destroy a client context and free all associated memory
 * @param ctx Context to destroy
 * @return LWM2MCC_SUCCESS
 */
lwm2mcc_result_t lwm2mcc_destroy(lwm2mcc_context_t *ctx);

/**
 * @brief Get the allocator from a context
 * @param ctx Client context
 * @param[out] allocator Output allocator pointer
 * @return LWM2MCC_SUCCESS
 */
lwm2mcc_result_t lwm2mcc_allocator(const lwm2mcc_context_t *ctx, const lwm2mcc_allocator_t **allocator);

/**
 * @brief Get the context's registered-objects array
 * @param ctx Client context
 * @param[out] objects Receives the context's registered-objects array pointer
 * @return LWM2MCC_SUCCESS
 */
lwm2mcc_result_t lwm2mcc_registered_objects(const lwm2mcc_context_t *ctx, sorted_array_t **objects);

/**
 * @brief Register the mandatory built-in objects (Security, Server, Device)
 * @param ctx Client context
 * @return LWM2MCC_SUCCESS
 */
lwm2mcc_result_t lwm2mcc_register_builtin_objects(lwm2mcc_context_t *ctx);

/**
 * @brief Open the transport and start the client state machine
 * @param ctx Client context
 * @return LWM2MCC_SUCCESS on success, LWM2MCC_ERR_TRANSPORT if the transport fails to open
 */
lwm2mcc_result_t lwm2mcc_start(lwm2mcc_context_t *ctx);

/**
 * @brief Deregister from all servers and close the transport
 * @param ctx Client context
 * @return LWM2MCC_SUCCESS
 */
lwm2mcc_result_t lwm2mcc_stop(lwm2mcc_context_t *ctx);

/**
 * @brief Drive the client, one step: process incoming messages and fire pending timers
 * @param ctx Client context
 * @param max_timeout_ms Upper bound on the returned timeout; use this when no timers are scheduled
 * @param[out] timeout_ms How long the caller may sleep before calling step again
 * @return LWM2MCC_SUCCESS
 */
lwm2mcc_result_t lwm2mcc_step(lwm2mcc_context_t *ctx, uint32_t max_timeout_ms,
                               uint32_t *timeout_ms);

#endif /* LWM2MCC__LWM2M_H */
