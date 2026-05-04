/**
 * @file common.h
 * @brief Common utility definitions
 */

#ifndef LWM2MCC__COMMON_H
#define LWM2MCC__COMMON_H

/** @brief Log levels */
typedef enum {
    LWM2MCC_LOG_LEVEL_TRACE,
    LWM2MCC_LOG_LEVEL_DEBUG,
    LWM2MCC_LOG_LEVEL_INFO,
    LWM2MCC_LOG_LEVEL_WARN,
    LWM2MCC_LOG_LEVEL_ERROR,
    LWM2MCC_LOG_LEVEL_FATAL,
    LWM2MCC_LOG_LEVEL_OFF,
} lwm2mcc_log_level_t;

typedef enum {
    LWM2MCC_SUCCESS = 0,
    LWM2MCC_TIMEOUT,
    LWM2MCC_ERR_OUT_OF_BOUNDS,
    LWM2MCC_ERR_NULL_POINTER,
    LWM2MCC_ERR_NOT_FOUND,
    LWM2MCC_ERR_TRANSPORT,
} lwm2mcc_result_t;

/**
 * @brief Log callback function
 * @param level Log level
 * @param message Log message
 */
typedef void (*lwm2mcc_log_cb_t)(lwm2mcc_log_level_t level, const char *message);

/**
 * @brief Set the log callback
 * @param cb Callback function, or NULL to disable logging
 */
void lwm2mcc_log_set_callback(lwm2mcc_log_cb_t cb);

#endif /* LWM2MCC__COMMON_H */
