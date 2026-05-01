/**
 * @file version.h
 * @brief LWM2M protocol version macros
 */

#ifndef LWM2MCC__VERSION_H
#define LWM2MCC__VERSION_H

#include <stdint.h>

/* Version packing: [31:22] major, [21:12] minor, [11:0] patch */
#define LWM2MCC_MAKE_VERSION(major, minor, patch) \
    ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

#define LWM2MCC_VERSION_MAJOR(v) (((uint32_t)(v) >> 22U) & 0x3FFU)
#define LWM2MCC_VERSION_MINOR(v) (((uint32_t)(v) >> 12U) & 0x3FFU)
#define LWM2MCC_VERSION_PATCH(v) ((uint32_t)(v) & 0xFFFU)

#define LWM2MCC_VERSION_1_0 LWM2MCC_MAKE_VERSION(1, 0, 0)
#define LWM2MCC_VERSION_1_1 LWM2MCC_MAKE_VERSION(1, 1, 0)
#define LWM2MCC_VERSION_1_2 LWM2MCC_MAKE_VERSION(1, 2, 0)

#ifndef LWM2MCC_VERSION
#define LWM2MCC_VERSION LWM2MCC_VERSION_1_2
#endif

#endif /* LWM2MCC__VERSION_H */
