/**
 * @file lwm2m.h
 * @brief LWM2MCC - LWM2M C Client library entry point
 */

#ifndef LWM2MCC__LWM2M_H
#define LWM2MCC__LWM2M_H

#include "lwm2m/memory.h"
#include "lwm2m/version.h"

/** @brief Opaque client context */
typedef struct lwm2mcc_context lwm2mcc_context_t;

/**
 * @brief Create a new LWM2M client context
 * @param allocator Custom allocator, or NULL for stdlib defaults
 * @return Context pointer, or NULL on failure
 */
lwm2mcc_context_t *lwm2mcc_create(const lwm2mcc_allocator_t *allocator);

/**
 * @brief Destroy a client context and free all associated memory
 * @param ctx Context to destroy
 */
void lwm2mcc_destroy(lwm2mcc_context_t *ctx);

/**
 * @brief Get the allocator from a context
 * @param ctx Client context
 * @return Pointer to the context's allocator
 */
const lwm2mcc_allocator_t *lwm2mcc_allocator(const lwm2mcc_context_t *ctx);

typedef struct sorted_array sorted_array_t;

/**
 * @brief Get the object store from a context
 * @param ctx Client context
 * @return Pointer to the object store sorted array pointer
 */
sorted_array_t **lwm2mcc_objects(lwm2mcc_context_t *ctx);

#endif /* LWM2MCC__LWM2M_H */
