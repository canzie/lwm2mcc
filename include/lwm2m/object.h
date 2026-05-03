/**
 * @file object.h
 * @brief LWM2M object model types and registration API
 */

#ifndef LWM2MCC__OBJECT_H
#define LWM2MCC__OBJECT_H

#include "lwm2m/version.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** @brief Object ID */
typedef uint16_t lwm2mcc_oid_t;

/** @brief Object Instance ID */
typedef uint16_t lwm2mcc_oiid_t;

/** @brief Resource ID */
typedef uint16_t lwm2mcc_rid_t;

/** @brief Resource Instance ID */
typedef uint16_t lwm2mcc_riid_t;

/** @brief Sentinel for "no specific ID" */
#define LWM2MCC_ID_INVALID UINT16_MAX

/** @brief LWM2M resource data types */
typedef enum {
    LWM2MCC_RESOURCE_NONE,
    LWM2MCC_RESOURCE_STRING,
    LWM2MCC_RESOURCE_INTEGER,
    LWM2MCC_RESOURCE_FLOAT,
    LWM2MCC_RESOURCE_BOOLEAN,
    LWM2MCC_RESOURCE_OPAQUE,
    LWM2MCC_RESOURCE_TIME,
    LWM2MCC_RESOURCE_OBJLNK,
#if LWM2MCC_VERSION >= LWM2MCC_VERSION_1_1
    LWM2MCC_RESOURCE_UNSIGNED_INTEGER,
    LWM2MCC_RESOURCE_CORELNK,
#endif
} lwm2mcc_resource_type_t;

/** @brief Whether a resource is mandatory per the spec */
typedef enum {
    LWM2MCC_RES_OPTIONAL,
    LWM2MCC_RES_MANDATORY,
} lwm2mcc_mandatory_t;

/** @brief Whether a resource is currently present or absent */
typedef enum {
    LWM2MCC_RES_ABSENT,
    LWM2MCC_RES_PRESENT,
} lwm2mcc_presence_t;

/** @brief Resource kind (encodes operations and multiplicity) */
typedef enum {
    LWM2MCC_RES_R,
    LWM2MCC_RES_W,
    LWM2MCC_RES_RW,
    LWM2MCC_RES_E,
    LWM2MCC_RES_RM,
    LWM2MCC_RES_WM,
    LWM2MCC_RES_RWM,
} lwm2mcc_resource_kind_t;

/**
 * @brief Static resource definition
 *
 * Describes a single resource within an object. Typically defined as a const
 * array that describes all resources for an object.
 */
typedef struct {
    lwm2mcc_rid_t rid;
    const char *name;
    lwm2mcc_resource_type_t type;
    lwm2mcc_resource_kind_t kind;
    lwm2mcc_mandatory_t mandatory;
    lwm2mcc_presence_t presence;
} lwm2mcc_resource_def_t;

typedef struct lwm2mcc_context lwm2mcc_context_t;
typedef struct lwm2mcc_output lwm2mcc_output_t;
typedef struct lwm2mcc_input lwm2mcc_input_t;

/**
 * @brief Read callback — emit the resource value into the output context
 * @param ctx Client context
 * @param oiid Object Instance ID
 * @param rid Resource ID
 * @param riid Resource Instance ID (LWM2MCC_ID_INVALID for single resources)
 * @param output Output context to emit the value into
 * @param user_data Per-instance user data
 * @return 0 on success, negative on error
 */
typedef int (*lwm2mcc_read_cb_t)(lwm2mcc_context_t *ctx, lwm2mcc_oiid_t oiid, lwm2mcc_rid_t rid, lwm2mcc_riid_t riid,
                                 lwm2mcc_output_t *output, void *user_data);

/**
 * @brief Write callback — consume the incoming value from the input context
 * @param ctx Client context
 * @param oiid Object Instance ID
 * @param rid Resource ID
 * @param riid Resource Instance ID (LWM2MCC_ID_INVALID for single resources)
 * @param input Input context to read the incoming value from
 * @param user_data Per-instance user data
 * @return 0 on success, negative on error
 */
typedef int (*lwm2mcc_write_cb_t)(lwm2mcc_context_t *ctx, lwm2mcc_oiid_t oiid, lwm2mcc_rid_t rid, lwm2mcc_riid_t riid,
                                  lwm2mcc_input_t *input, void *user_data);

/**
 * @brief Execute callback
 * @param ctx Client context
 * @param oiid Object Instance ID
 * @param rid Resource ID
 * @param args Execute arguments from the server (may be NULL)
 * @param args_len Length of args in bytes
 * @param user_data Per-instance user data
 * @return 0 on success, negative on error
 */
typedef int (*lwm2mcc_execute_cb_t)(lwm2mcc_context_t *ctx, lwm2mcc_oiid_t oiid, lwm2mcc_rid_t rid, const uint8_t *args,
                                    size_t args_len, void *user_data);

/**
 * @brief Object definition
 *
 * Static description of an LWM2M object. The user provides this when
 * registering an object. The pointer must remain valid for the lifetime
 * of the registration.
 */
typedef struct {
    lwm2mcc_oid_t oid;
    const char *name;
    const lwm2mcc_resource_def_t *resources;
    size_t resource_count;
    lwm2mcc_read_cb_t on_read;
    lwm2mcc_write_cb_t on_write;
    lwm2mcc_execute_cb_t on_execute;
} lwm2mcc_object_def_t;

/**
 * @brief Register an object definition with the client context
 * @param ctx Client context
 * @param def Object definition (must remain valid while registered)
 */
void lwm2mcc_object_register(lwm2mcc_context_t *ctx, const lwm2mcc_object_def_t *def);

/**
 * @brief Unregister an object from the client context
 * @param ctx Client context
 * @param oid Object ID to unregister
 */
void lwm2mcc_object_unregister(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid);

/**
 * @brief Add an object instance
 * @param ctx Client context
 * @param oid Object ID
 * @param oiid Object Instance ID
 * @param user_data User data passed to callbacks for this instance
 */
void lwm2mcc_object_instance_add(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid, void *user_data);

/**
 * @brief Remove an object instance
 * @param ctx Client context
 * @param oid Object ID
 * @param oiid Object Instance ID
 */
void lwm2mcc_object_instance_remove(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid);

/**
 * @brief Add a resource instance to a multi-instance resource
 * @param ctx Client context
 * @param oid Object ID
 * @param oiid Object Instance ID
 * @param rid Resource ID (must be defined with multiple = true)
 * @param riid Resource Instance ID
 */
void lwm2mcc_resource_instance_add(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid, lwm2mcc_rid_t rid,
                                   lwm2mcc_riid_t riid);

/**
 * @brief Remove a resource instance from a multi-instance resource
 * @param ctx Client context
 * @param oid Object ID
 * @param oiid Object Instance ID
 * @param rid Resource ID
 * @param riid Resource Instance ID
 */
void lwm2mcc_resource_instance_remove(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid, lwm2mcc_rid_t rid,
                                      lwm2mcc_riid_t riid);

#endif /* LWM2MCC__OBJECT_H */
