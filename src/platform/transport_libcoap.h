/**
 * @file transport_libcoap.h
 * @brief libcoap-backed transport PAL implementation
 */

#ifndef LWM2MCC__TRANSPORT_LIBCOAP_H
#define LWM2MCC__TRANSPORT_LIBCOAP_H

#include "lwm2m/memory.h"
#include "lwm2m/transport.h"

/**
 * @brief Initialise the libcoap transport and populate an ops struct
 * @param ops Transport ops struct to fill in, user_ctx is set internally
 * @param allocator Allocator used to allocate and free the internal context
 */
void lwm2mcc_libcoap_transport_init(lwm2mcc_transport_ops_t *ops, const lwm2mcc_allocator_t *allocator);

/**
 * @brief Release all resources allocated by lwm2mcc_libcoap_transport_init
 * @param ops The same ops struct passed to lwm2mcc_libcoap_transport_init
 */
void lwm2mcc_libcoap_transport_shutdown(lwm2mcc_transport_ops_t *ops);

#endif /* LWM2MCC__TRANSPORT_LIBCOAP_H */
