/**
 * @file timers.c
 * @brief Internal timer scheduling backed by a min-heap
 */

#include "timers.h"

#include "min_heap.h"
#include "../utils/assert.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct lwm2mcc_timers {
    min_heap_t *heap;
    const lwm2mcc_platform_ops_t *platform;
    const lwm2mcc_allocator_t *alloc;
} lwm2mcc_timers_t;

typedef struct lwm2mcc_timer {
    lwm2mcc_timers_t *owner;
    uint64_t fire_at_ms;
    uint32_t period_ms;
    int64_t repeat;
    lwm2mcc_timer_cb_t cb;
    void *user_data;
    bool active;
} lwm2mcc_timer_t;

static int32_t s_cmp(const void *a, const void *b)
{
    const lwm2mcc_timer_t *ta = *(const lwm2mcc_timer_t *const *)a;
    const lwm2mcc_timer_t *tb = *(const lwm2mcc_timer_t *const *)b;
    if (ta->fire_at_ms < tb->fire_at_ms) {
        return -1;
    }
    if (ta->fire_at_ms > tb->fire_at_ms) {
        return 1;
    }
    return 0;
}

lwm2mcc_timers_t *lwm2mcc_timers_create(const lwm2mcc_platform_ops_t *platform,
                                         const lwm2mcc_allocator_t *alloc)
{
    lwm2mcc_timers_t *ts = lwm2mcc_alloc(alloc, sizeof(lwm2mcc_timers_t));
    *ts = (lwm2mcc_timers_t){
        .heap = min_heap_create(sizeof(lwm2mcc_timer_t *), _Alignof(lwm2mcc_timer_t *), s_cmp,
                                alloc),
        .platform = platform,
        .alloc = alloc,
    };
    return ts;
}

void lwm2mcc_timers_destroy(lwm2mcc_timers_t *timers)
{
    if (timers == NULL) {
        return;
    }
    uint32_t n = min_heap_size(timers->heap);
    for (uint32_t i = 0; i < n; i++) {
        lwm2mcc_free(timers->alloc, *(lwm2mcc_timer_t **)min_heap_at(timers->heap, i));
    }
    min_heap_destroy(timers->heap);
    lwm2mcc_free(timers->alloc, timers);
}

lwm2mcc_timer_t *lwm2mcc_timer_create(lwm2mcc_timers_t *timers, lwm2mcc_timer_cb_t cb,
                                       void *user_data)
{
    LWM2MCC_ASSERT(timers != NULL, "timers is NULL");
    lwm2mcc_timer_t *t = lwm2mcc_alloc(timers->alloc, sizeof(lwm2mcc_timer_t));
    *t = (lwm2mcc_timer_t){
        .owner = timers,
        .cb = cb,
        .user_data = user_data,
    };
    return t;
}

void lwm2mcc_timer_start(lwm2mcc_timer_t *timer, uint32_t delay_ms, uint32_t period_ms,
                          int64_t repeat)
{
    if (timer == NULL) {
        return;
    }
    lwm2mcc_timer_stop(timer);
    lwm2mcc_timers_t *ts = timer->owner;
    uint64_t now = ts->platform->now_ms(ts->platform->user_ctx);
    timer->fire_at_ms = now + delay_ms;
    timer->period_ms = period_ms;
    timer->repeat = repeat;
    timer->active = true;
    min_heap_insert(ts->heap, &timer);
}

void lwm2mcc_timer_stop(lwm2mcc_timer_t *timer)
{
    if (timer == NULL || !timer->active) {
        return;
    }
    timer->active = false;
    min_heap_t *heap = timer->owner->heap;
    uint32_t n = min_heap_size(heap);
    for (uint32_t i = 0; i < n; i++) {
        lwm2mcc_timer_t **slot = min_heap_at(heap, i);
        if (*slot == timer) {
            min_heap_remove_at(heap, i);
            return;
        }
    }
}

void lwm2mcc_timer_destroy(lwm2mcc_timer_t *timer)
{
    if (timer == NULL) {
        return;
    }
    lwm2mcc_timer_stop(timer);
    lwm2mcc_free(timer->owner->alloc, timer);
}

uint32_t lwm2mcc_timers_step(lwm2mcc_timers_t *timers, uint32_t max_timeout_ms)
{
    LWM2MCC_ASSERT(timers != NULL, "timers is NULL");
    uint64_t now = timers->platform->now_ms(timers->platform->user_ctx);
    min_heap_t *heap = timers->heap;

    while (true) {
        lwm2mcc_timer_t **slot = min_heap_peek(heap);
        if (slot == NULL || (*slot)->fire_at_ms > now) {
            break;
        }
        lwm2mcc_timer_t *t = *slot;
        min_heap_pop(heap);
        t->active = false;
        if (t->repeat != 0) {
            t->fire_at_ms = now + t->period_ms;
            if (t->repeat > 0) {
                t->repeat--;
            }
            t->active = true;
            min_heap_insert(heap, &t);
        }
        t->cb(t, t->user_data);
    }

    lwm2mcc_timer_t **slot = min_heap_peek(heap);
    if (slot == NULL) {
        return max_timeout_ms;
    }
    uint64_t diff = (*slot)->fire_at_ms - now;
    return diff < max_timeout_ms ? (uint32_t)diff : max_timeout_ms;
}
