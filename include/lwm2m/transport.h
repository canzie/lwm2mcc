/**
 * @file transport.h
 * @brief CoAP transport abstraction
 */

#ifndef LWM2MCC__TRANSPORT_H
#define LWM2MCC__TRANSPORT_H

#include "lwm2m/coap.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Remote server endpoint
 *
 * URI is taken directly from Security object resource 0,
 * e.g. "coap://hostname:5683" or "coaps://hostname:5684".
 * The transport implementation resolves and caches the network address.
 */
#define LWM2MCC_ENDPOINT_URI_MAX 256

typedef struct {
    char uri[LWM2MCC_ENDPOINT_URI_MAX];
} lwm2mcc_endpoint_t;

/** @brief Initialise the transport (create socket, bind interface, etc.) */
typedef int (*lwm2mcc_transport_open_fn_t)(void *ctx);

/**
 * @brief Send a CoAP message to a remote endpoint
 * @param ctx    User-provided context
 * @param remote Destination endpoint
 * @param msg    Message to send; valid for the duration of the call
 */
typedef int (*lwm2mcc_transport_send_fn_t)(void *ctx, const lwm2mcc_endpoint_t *remote,
                                           const lwm2mcc_coap_message_t *msg);

/**
 * @brief Receive a CoAP message (non-blocking)
 * @param ctx    User-provided context
 * @param remote [out] Source endpoint of the received message
 * @param msg    [out] Caller-allocated message; implementation fills it in
 * @return 1 if a message was received, 0 if nothing available, negative on error
 */
typedef int (*lwm2mcc_transport_recv_fn_t)(void *ctx, lwm2mcc_endpoint_t *remote,
                                           lwm2mcc_coap_message_t *msg);

/**
 * @brief Return a file descriptor suitable for select/poll/epoll
 * @return File descriptor, or -1 if not applicable (e.g. bare-metal polling)
 */
typedef int (*lwm2mcc_transport_get_fd_fn_t)(void *ctx);

/** @brief Close the transport and release underlying resources */
typedef void (*lwm2mcc_transport_close_fn_t)(void *ctx);

/**
 * @brief CoAP transport operations
 *
 * Fill in the function pointers and pass this struct to lwm2mcc_create().
 * All callbacks receive user_ctx as their first argument.
 * A negative return value from any callback signals a transport error.
 */
typedef struct {
    lwm2mcc_transport_open_fn_t   open;
    lwm2mcc_transport_send_fn_t   send;
    lwm2mcc_transport_recv_fn_t   recv;
    lwm2mcc_transport_get_fd_fn_t get_fd;
    lwm2mcc_transport_close_fn_t  close;
    void                         *user_ctx;
} lwm2mcc_transport_ops_t;

#endif /* LWM2MCC__TRANSPORT_H */
