/**
 * @file common.h
 * @brief Common utility definitions
 */

#ifndef LWM2MCC__COMMON_H
#define LWM2MCC__COMMON_H

/** @brief Log levels */
typedef enum {
    LWM2MCC_LOG_TRACE,
    LWM2MCC_LOG_DEBUG,
    LWM2MCC_LOG_INFO,
    LWM2MCC_LOG_WARN,
    LWM2MCC_LOG_ERROR,
    LWM2MCC_LOG_FATAL,
} lwm2mcc_log_level_t;

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
