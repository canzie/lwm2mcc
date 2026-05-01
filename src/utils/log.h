/**
 * @file log.h
 * @brief Logging with levels, colors, and timestamps
 */

#ifndef LWM2MCC__LOG_H
#define LWM2MCC__LOG_H

#include <stdio.h>

typedef enum {
    LWM2MCC_LOG_TRACE,
    LWM2MCC_LOG_DEBUG,
    LWM2MCC_LOG_INFO,
    LWM2MCC_LOG_WARN,
    LWM2MCC_LOG_ERROR,
    LWM2MCC_LOG_FATAL,
    LWM2MCC_LOG_OFF,
} lwm2mcc_log_level_t;

/**
 * @brief Set the minimum log level
 * @param level Messages below this level are suppressed
 */
void lwm2mcc_log_set_level(lwm2mcc_log_level_t level);

/**
 * @brief Enable or disable colored output
 * @param enable Non-zero to enable, zero to disable
 */
void lwm2mcc_log_set_color(int enable);

/**
 * @brief Log a message
 * @param level Log level
 * @param file Source file name (__FILE__)
 * @param line Source line number (__LINE__)
 * @param fmt printf-style format string
 */
void lwm2mcc_log_write(lwm2mcc_log_level_t level, const char *file, int line, const char *fmt, ...)
    __attribute__((format(printf, 4, 5)));

#define LWM2MCC_LOG_TRACE(...) lwm2mcc_log_write(LWM2MCC_LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LWM2MCC_LOG_DEBUG(...) lwm2mcc_log_write(LWM2MCC_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LWM2MCC_LOG_INFO(...)  lwm2mcc_log_write(LWM2MCC_LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LWM2MCC_LOG_WARN(...)  lwm2mcc_log_write(LWM2MCC_LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LWM2MCC_LOG_ERROR(...) lwm2mcc_log_write(LWM2MCC_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LWM2MCC_LOG_FATAL(...) lwm2mcc_log_write(LWM2MCC_LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif /* LWM2MCC__LOG_H */
