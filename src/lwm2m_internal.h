/**
 * @file lwm2m_internal.h
 * @brief Internal LWM2M client context helpers
 */

#ifndef LWM2MCC__LWM2M_INTERNAL_H
#define LWM2MCC__LWM2M_INTERNAL_H

#include "lwm2m/lwm2m.h"

const lwm2mcc_allocator_t *_lwm2mcc_allocator(const lwm2mcc_context_t *ctx);
sorted_array_t **_lwm2mcc_objects(lwm2mcc_context_t *ctx);
sorted_array_t *_lwm2mcc_registered_objects(const lwm2mcc_context_t *ctx);

#endif /* LWM2MCC__LWM2M_INTERNAL_H */