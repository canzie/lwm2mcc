/**
 * @file security.c
 * @brief LWM2M Security object (OID 0) definition
 */

#include "lwm2m/object.h"

// clang-format off
static const lwm2mcc_resource_def_t s_resources[] = {
    {.rid = 0,  .name = "LWM2M Server URI",                   .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 1,  .name = "Bootstrap-Server",                   .type = LWM2MCC_RESOURCE_BOOLEAN, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 2,  .name = "Security Mode",                      .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 3,  .name = "Public Key or Identity",             .type = LWM2MCC_RESOURCE_OPAQUE,  .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 4,  .name = "Server Public Key",                  .type = LWM2MCC_RESOURCE_OPAQUE,  .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 5,  .name = "Secret Key",                         .type = LWM2MCC_RESOURCE_OPAQUE,  .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 10, .name = "Short Server ID",                    .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL, .presence = LWM2MCC_RES_ABSENT},
    {.rid = 11, .name = "Client Hold Off Time",               .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL, .presence = LWM2MCC_RES_ABSENT},
    {.rid = 12, .name = "Bootstrap-Server Account Timeout",   .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL, .presence = LWM2MCC_RES_ABSENT},
};
// clang-format on

static const lwm2mcc_object_def_t s_def = {
    .oid = 0,
    .name = "LWM2M Security",
    .resources = s_resources,
    .resource_count = sizeof(s_resources) / sizeof(s_resources[0]),
};

const lwm2mcc_object_def_t *lwm2mcc_builtin_security_def(void)
{
    return &s_def;
}
