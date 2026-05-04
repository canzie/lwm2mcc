/**
 * @file platform.h
 * @brief Platform services abstraction
 */

#ifndef LWM2MCC__PLATFORM_H
#define LWM2MCC__PLATFORM_H

#include "lwm2m/utils/common.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Return a monotonic timestamp in milliseconds
 *
 * Must not go backwards. POSIX clock_gettime(CLOCK_MONOTONIC) is the typical
 * backing implementation; embedded targets may use a hardware timer counter.
 * @param ctx User-provided context
 * @return Current time in milliseconds
 */
typedef uint64_t (*lwm2mcc_platform_now_ms_fn_t)(void *ctx);

/**
 * @brief Fill a buffer with cryptographically random bytes
 *
 * Required when the security layer needs entropy (e.g. DTLS handshake nonces).
 * May be NULL if the security implementation sources entropy itself.
 * @param ctx    User-provided context
 * @param buf    Buffer to fill
 * @param len    Number of bytes to generate
 * @return 0 on success, negative if entropy is unavailable
 */
typedef int (*lwm2mcc_platform_random_fn_t)(void *ctx, uint8_t *buf, size_t len);

/**
 * @brief Platform logging bridge
 *
 * Optional hook for forwarding library log output to a platform-native
 * logging facility (e.g. syslog, RTT, UART). May be NULL; set the global
 * callback via lwm2mcc_log_set_callback() instead if preferred.
 * @param ctx     User-provided context
 * @param level   Log level
 * @param message Null-terminated log message
 */
typedef void (*lwm2mcc_platform_log_fn_t)(void *ctx, lwm2mcc_log_level_t level, const char *message);

/**
 * @brief Platform service operations
 *
 * Fill in the function pointers and pass this struct to lwm2mcc_create().
 * now_ms is required. random and log are optional (NULL disables them).
 */
typedef struct {
    lwm2mcc_platform_now_ms_fn_t now_ms;
    lwm2mcc_platform_random_fn_t random;
    lwm2mcc_platform_log_fn_t    log;
    void                        *user_ctx;
} lwm2mcc_platform_ops_t;

#endif /* LWM2MCC__PLATFORM_H */
