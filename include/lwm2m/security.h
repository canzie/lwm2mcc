/**
 * @file security.h
 * @brief DTLS security abstraction
 */

#ifndef LWM2MCC__SECURITY_H
#define LWM2MCC__SECURITY_H

#include "lwm2m/transport.h"

#include <stddef.h>
#include <stdint.h>

/** @brief DTLS security modes as defined by the LWM2M Security object (resource 2) */
typedef enum {
    LWM2MCC_SECURITY_MODE_PSK   = 0,
    LWM2MCC_SECURITY_MODE_RPK   = 1,
    LWM2MCC_SECURITY_MODE_CERT  = 2,
    LWM2MCC_SECURITY_MODE_NOSEC = 3,
} lwm2mcc_security_mode_t;

/**
 * @brief Security credentials passed to the handshake callback
 *
 * Active union member is determined by mode.
 * All byte pointers are valid only for the duration of the handshake call.
 */
typedef struct {
    lwm2mcc_security_mode_t mode;
    union {
        struct {
            const uint8_t *identity;
            size_t         identity_len;
            const uint8_t *key;
            size_t         key_len;
        } psk;
        struct {
            const uint8_t *public_key;
            size_t         public_key_len;
            const uint8_t *private_key;
            size_t         private_key_len;
        } rpk;
        struct {
            const uint8_t *cert_chain;
            size_t         cert_chain_len;
            const uint8_t *private_key;
            size_t         private_key_len;
        } cert;
    };
} lwm2mcc_security_info_t;

/**
 * @brief Perform a DTLS handshake with a remote endpoint
 * @param ctx         User-provided context
 * @param remote      Remote server endpoint
 * @param credentials Credentials to use for this session
 * @return 0 on success, negative on error
 */
typedef int (*lwm2mcc_security_handshake_fn_t)(void *ctx, const lwm2mcc_endpoint_t *remote,
                                                const lwm2mcc_security_info_t *credentials);

/**
 * @brief Close a DTLS session with a remote endpoint
 * @param ctx    User-provided context
 * @param remote Remote server endpoint whose session to close
 */
typedef void (*lwm2mcc_security_close_fn_t)(void *ctx, const lwm2mcc_endpoint_t *remote);

/**
 * @brief DTLS security operations
 *
 * Fill in the function pointers and pass this struct to lwm2mcc_create().
 * May be NULL when operating in NoSec mode.
 */
typedef struct {
    lwm2mcc_security_handshake_fn_t handshake;
    lwm2mcc_security_close_fn_t     close;
    void                           *user_ctx;
} lwm2mcc_security_ops_t;

#endif /* LWM2MCC__SECURITY_H */
