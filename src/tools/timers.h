/**
 * @file timers.h
 * @brief Internal timer scheduling
 */

#ifndef LWM2MCC__TIMERS_H
#define LWM2MCC__TIMERS_H

#include "lwm2m/memory.h"
#include "lwm2m/platform.h"

#include <stdint.h>

typedef struct lwm2mcc_timers lwm2mcc_timers_t;
typedef struct lwm2mcc_timer lwm2mcc_timer_t;

typedef void (*lwm2mcc_timer_cb_t)(lwm2mcc_timer_t *timer, void *user_data);

/**
 * @brief Create a timer subsystem
 * @param platform Platform ops; must outlive the subsystem
 * @param alloc Allocator; must outlive the subsystem
 * @return Timer subsystem
 */
lwm2mcc_timers_t *lwm2mcc_timers_create(const lwm2mcc_platform_ops_t *platform,
                                         const lwm2mcc_allocator_t *alloc);

/**
 * @brief Destroy a timer subsystem and free all active timers
 *
 * Stopped-but-not-destroyed timers must be destroyed before calling this.
 * @param timers Subsystem to destroy, NULL-safe
 */
void lwm2mcc_timers_destroy(lwm2mcc_timers_t *timers);

/**
 * @brief Allocate a timer
 * @param timers Timer subsystem
 * @param cb Callback invoked when the timer fires
 * @param user_data Passed to the callback
 * @return Pointer to the timer
 */
lwm2mcc_timer_t *lwm2mcc_timer_create(lwm2mcc_timers_t *timers, lwm2mcc_timer_cb_t cb,
                                       void *user_data);

/**
 * @brief Start a timer
 * @param timer Timer created by lwm2mcc_timer_create
 * @param delay_ms Delay before the first firing in milliseconds
 * @param period_ms Interval between subsequent firings, ignored when repeat is 0
 * @param repeat 0 for one-shot, negative for infinite repeat, positive for N more firings
 */
void lwm2mcc_timer_start(lwm2mcc_timer_t *timer, uint32_t delay_ms, uint32_t period_ms,
                          int64_t repeat);

/**
 * @brief Stop a running timer without destroying it
 * @param timer Timer to stop, may be NULL
 */
void lwm2mcc_timer_stop(lwm2mcc_timer_t *timer);

/**
 * @brief Destroy a timer and free its memory
 * @param timer Timer to destroy, may be NULL
 */
void lwm2mcc_timer_destroy(lwm2mcc_timer_t *timer);

/**
 * @brief Fire any expired timers and return how long the caller may sleep
 * @param timers Timer subsystem
 * @param max_timeout_ms Upper bound on the returned value; use this when no timers are scheduled
 * @return Milliseconds until the next scheduled timer, capped at max_timeout_ms
 */
uint32_t lwm2mcc_timers_step(lwm2mcc_timers_t *timers, uint32_t max_timeout_ms);

#endif /* LWM2MCC__TIMERS_H */
