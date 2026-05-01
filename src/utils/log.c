/**
 * @file log.c
 * @brief Logging implementation
 */

#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

static lwm2mcc_log_level_t s_min_level = LWM2MCC_LOG_TRACE;
static int s_color_enabled = 1;

static const char *s_level_names[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL",
};

static const char *s_level_colors[] = {
    "\x1b[37m", /* TRACE: white */
    "\x1b[36m", /* DEBUG: cyan */
    "\x1b[32m", /* INFO:  green */
    "\x1b[33m", /* WARN:  yellow */
    "\x1b[31m", /* ERROR: red */
    "\x1b[35m", /* FATAL: magenta */
};

static const char *s_reset = "\x1b[0m";

void lwm2mcc_log_set_level(lwm2mcc_log_level_t level)
{
    s_min_level = level;
}

void lwm2mcc_log_set_color(int enable)
{
    s_color_enabled = enable;
}

void lwm2mcc_log_write(lwm2mcc_log_level_t level, const char *file, int line, const char *fmt, ...)
{
    if (level < s_min_level) {
        return;
    }

    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);

    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &tm);

    if (s_color_enabled != 0) {
        fprintf(stderr, "%s %s%-5s%s %s:%d: ", time_buf, s_level_colors[level], s_level_names[level], s_reset, file, line);
    } else {
        fprintf(stderr, "%s %-5s %s:%d: ", time_buf, s_level_names[level], file, line);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");

    if (level > LWM2MCC_LOG_TRACE) {
        fflush(stderr);
    }
}
