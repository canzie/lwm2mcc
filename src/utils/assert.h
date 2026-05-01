/**
 * @file assert.h
 * @brief Assert macro with logging
 */

#ifndef LWM2MCC__ASSERT_H
#define LWM2MCC__ASSERT_H

#include "log.h"

#include <stdlib.h>

#define LWM2MCC_ASSERT(cond, msg)                        \
    do {                                                 \
        if (!(cond)) {                                   \
            LWM2MCC_LOG_FATAL("ASSERT FAILED: %s", msg); \
            abort();                                     \
        }                                                \
    } while (0)

#endif /* LWM2MCC__ASSERT_H */
