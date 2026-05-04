/**
 * @file lwm2m.h
 * @brief LWM2MCC - LWM2M C Client library entry point
 */

#ifndef LWM2MCC__LWM2M_H
#define LWM2MCC__LWM2M_H

#include "lwm2m/memory.h"
#include "lwm2m/utils/common.h"
#include "lwm2m/version.h"

/** @brief Opaque client context */
typedef struct lwm2mcc_context lwm2mcc_context_t;

typedef struct sorted_array sorted_array_t;

/**
 * @brief Create a new LWM2M client context
 * @param allocator Custom allocator, or NULL for stdlib defaults
 * @param[out] ctx Output context pointer
 * @return LWM2MCC_SUCCESS
 */
lwm2mcc_result_t lwm2mcc_create(const lwm2mcc_allocator_t *allocator, lwm2mcc_context_t **ctx);

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

#endif /* LWM2MCC__LWM2M_H */
