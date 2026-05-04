/**
 * @file coap.h
 * @brief CoAP message type and associated constants
 */

#ifndef LWM2MCC__COAP_H
#define LWM2MCC__COAP_H

#include "lwm2m/version.h"

#include <stddef.h>
#include <stdint.h>

/** @brief Max token length (RFC 7252 §4.1) */
#define LWM2MCC_COAP_TOKEN_MAX   8u

/** @brief Max options per message */
#define LWM2MCC_COAP_OPTIONS_MAX 16u

/** @brief CoAP message type */
typedef enum {
    LWM2MCC_COAP_TYPE_CON = 0,
    LWM2MCC_COAP_TYPE_NON = 1,
    LWM2MCC_COAP_TYPE_ACK = 2,
    LWM2MCC_COAP_TYPE_RST = 3,
} lwm2mcc_coap_type_t;

/**
 * @brief CoAP code byte: (class << 5) | detail
 *
 * Methods use class 0. Responses use classes 2 (success), 4 (client error),
 * and 5 (server error).
 */
typedef uint8_t lwm2mcc_coap_code_t;

#define LWM2MCC_COAP_CODE(c, d) ((lwm2mcc_coap_code_t)(((unsigned)(c) << 5u) | (unsigned)(d)))

#define LWM2MCC_COAP_CODE_EMPTY                      LWM2MCC_COAP_CODE(0, 0)
#define LWM2MCC_COAP_CODE_GET                        LWM2MCC_COAP_CODE(0, 1)
#define LWM2MCC_COAP_CODE_POST                       LWM2MCC_COAP_CODE(0, 2)
#define LWM2MCC_COAP_CODE_PUT                        LWM2MCC_COAP_CODE(0, 3)
#define LWM2MCC_COAP_CODE_DELETE                     LWM2MCC_COAP_CODE(0, 4)
#define LWM2MCC_COAP_CODE_CREATED                    LWM2MCC_COAP_CODE(2, 1)
#define LWM2MCC_COAP_CODE_DELETED                    LWM2MCC_COAP_CODE(2, 2)
#define LWM2MCC_COAP_CODE_VALID                      LWM2MCC_COAP_CODE(2, 3)
#define LWM2MCC_COAP_CODE_CHANGED                    LWM2MCC_COAP_CODE(2, 4)
#define LWM2MCC_COAP_CODE_CONTENT                    LWM2MCC_COAP_CODE(2, 5)
#define LWM2MCC_COAP_CODE_BAD_REQUEST                LWM2MCC_COAP_CODE(4, 0)
#define LWM2MCC_COAP_CODE_UNAUTHORIZED               LWM2MCC_COAP_CODE(4, 1)
#define LWM2MCC_COAP_CODE_NOT_FOUND                  LWM2MCC_COAP_CODE(4, 4)
#define LWM2MCC_COAP_CODE_METHOD_NOT_ALLOWED         LWM2MCC_COAP_CODE(4, 5)
#define LWM2MCC_COAP_CODE_NOT_ACCEPTABLE             LWM2MCC_COAP_CODE(4, 6)
#define LWM2MCC_COAP_CODE_UNSUPPORTED_CONTENT_FORMAT LWM2MCC_COAP_CODE(4, 15)
#define LWM2MCC_COAP_CODE_INTERNAL_SERVER_ERROR      LWM2MCC_COAP_CODE(5, 0)

/** @brief Extract the class field (0–7) from a code byte */
#define LWM2MCC_COAP_CODE_CLASS(code)  ((uint8_t)((code) >> 5u))
/** @brief Extract the detail field (0–31) from a code byte */
#define LWM2MCC_COAP_CODE_DETAIL(code) ((uint8_t)((code) & 0x1Fu))

/** @brief CoAP option numbers used by LWM2M */
#define LWM2MCC_COAP_OPT_OBSERVE        6u
#define LWM2MCC_COAP_OPT_LOCATION_PATH  8u
#define LWM2MCC_COAP_OPT_URI_PATH       11u
#define LWM2MCC_COAP_OPT_CONTENT_FORMAT 12u
#define LWM2MCC_COAP_OPT_URI_QUERY      15u
#define LWM2MCC_COAP_OPT_ACCEPT         17u
#define LWM2MCC_COAP_OPT_LOCATION_QUERY 20u
#define LWM2MCC_COAP_OPT_BLOCK2         23u
#define LWM2MCC_COAP_OPT_BLOCK1         27u

/** @brief Content-Format values used by LWM2M (IANA CoAP Content-Formats registry) */
typedef enum {
    LWM2MCC_CONTENT_TEXT_PLAIN   = 0,
    LWM2MCC_CONTENT_LINK_FORMAT  = 40,
    LWM2MCC_CONTENT_OCTET_STREAM = 42,
    LWM2MCC_CONTENT_TLV          = 11542,
    LWM2MCC_CONTENT_JSON         = 11543,
#if LWM2MCC_VERSION >= LWM2MCC_VERSION_1_1
    LWM2MCC_CONTENT_SENML_JSON   = 110,
    LWM2MCC_CONTENT_SENML_CBOR   = 112,
    LWM2MCC_CONTENT_LWM2M_CBOR   = 11544,
#endif
} lwm2mcc_content_format_t;

/**
 * @brief A single CoAP option
 *
 * value points into caller-managed memory. For outgoing messages it points to
 * static data or an arena buffer. For incoming messages it points into the
 * transport's receive buffer and is only valid until the next recv call.
 */
typedef struct {
    uint16_t       number;
    const uint8_t *value;
    size_t         value_len;
} lwm2mcc_coap_option_t;

/**
 * @brief CoAP message
 *
 * Neither option values nor the payload are owned by this struct.
 */
typedef struct lwm2mcc_coap_message {
    lwm2mcc_coap_type_t   type;
    lwm2mcc_coap_code_t   code;
    uint16_t              mid;
    uint8_t               token[LWM2MCC_COAP_TOKEN_MAX];
    uint8_t               token_len;
    lwm2mcc_coap_option_t options[LWM2MCC_COAP_OPTIONS_MAX];
    uint8_t               option_count;
    const uint8_t        *payload;
    size_t                payload_len;
} lwm2mcc_coap_message_t;

/**
 * @brief Decode a CoAP variable-length uint option value (big-endian)
 * @param buf Encoded bytes
 * @param len Number of bytes (0–4)
 * @return Decoded value
 */
static inline uint32_t lwm2mcc_coap_decode_uint(const uint8_t *buf, size_t len)
{
    uint32_t v = 0;
    for (size_t i = 0; i < len; i++) {
        v = (v << 8u) | buf[i];
    }
    return v;
}

/**
 * @brief Encode a uint as a CoAP variable-length big-endian option value
 * @param value Value to encode
 * @param buf   Output buffer (at least 4 bytes)
 * @return Number of bytes written (0 for value == 0)
 */
static inline size_t lwm2mcc_coap_encode_uint(uint32_t value, uint8_t *buf)
{
    if (value == 0u) {
        return 0;
    }
    size_t len = 0;
    if (value > 0x00FFFFFFu) { buf[len++] = (uint8_t)(value >> 24u); }
    if (value > 0x0000FFFFu) { buf[len++] = (uint8_t)(value >> 16u); }
    if (value > 0x000000FFu) { buf[len++] = (uint8_t)(value >>  8u); }
    buf[len++] = (uint8_t)(value);
    return len;
}

#endif /* LWM2MCC__COAP_H */
