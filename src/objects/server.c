/**
 * @file server.c
 * @brief LWM2M Server object (OID 1) definition
 */

#include "lwm2m/object.h"

// clang-format off
static const lwm2mcc_resource_def_t s_resources[] = {
    {.rid = 0, .name = "Short Server ID",                            .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 1, .name = "Lifetime",                                   .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 2, .name = "Default Minimum Period",                     .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL, .presence = LWM2MCC_RES_ABSENT},
    {.rid = 3, .name = "Default Maximum Period",                     .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL, .presence = LWM2MCC_RES_ABSENT},
    {.rid = 4, .name = "Disable",                                    .type = LWM2MCC_RESOURCE_NONE,    .kind = LWM2MCC_RES_E,  .mandatory = LWM2MCC_RES_OPTIONAL, .presence = LWM2MCC_RES_ABSENT},
    {.rid = 5, .name = "Disable Timeout",                            .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL, .presence = LWM2MCC_RES_ABSENT},
    {.rid = 6, .name = "Notification Storing When Disabled or Offline", .type = LWM2MCC_RESOURCE_BOOLEAN, .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 7, .name = "Binding",                                    .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = 8, .name = "Registration Update Trigger",                .type = LWM2MCC_RESOURCE_NONE,    .kind = LWM2MCC_RES_E,  .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
};
// clang-format on

static const lwm2mcc_object_def_t s_def = {
    .oid = 1,
    .name = "LWM2M Server",
    .resources = s_resources,
    .resource_count = sizeof(s_resources) / sizeof(s_resources[0]),
};

const lwm2mcc_object_def_t *lwm2mcc_builtin_server_def(void)
{
    return &s_def;
}
