/**
 * @file testapp.c
 * @brief Minimal test application for lwm2mcc
 */

#include "lwm2m/lwm2m.h"
#include "lwm2m/object.h"

#include <stdio.h>

static const lwm2mcc_resource_def_t s_device_resources[] = {
    {.rid = 0, .name = "Manufacturer", .type = LWM2MCC_RESOURCE_STRING, .operations = LWM2MCC_OP_READ, .mandatory = true},
    {.rid = 1, .name = "Model Number", .type = LWM2MCC_RESOURCE_STRING, .operations = LWM2MCC_OP_READ, .mandatory = true},
    {.rid = 3, .name = "Firmware Version", .type = LWM2MCC_RESOURCE_STRING, .operations = LWM2MCC_OP_READ},
    {.rid = 4, .name = "Reboot", .type = LWM2MCC_RESOURCE_NONE, .operations = LWM2MCC_OP_EXECUTE, .mandatory = true},
    {.rid = 11,
     .name = "Error Code",
     .type = LWM2MCC_RESOURCE_INTEGER,
     .operations = LWM2MCC_OP_READ,
     .multiple = true,
     .mandatory = true},
};

static const lwm2mcc_object_def_t s_device_def = {
    .oid = 3,
    .name = "Device",
    .resources = s_device_resources,
    .resource_count = sizeof(s_device_resources) / sizeof(s_device_resources[0]),
};

int main(void)
{
    lwm2mcc_context_t *ctx = lwm2mcc_create(NULL);

    printf("lwm2mcc context created (version %u.%u)\n", LWM2MCC_VERSION_MAJOR(LWM2MCC_VERSION),
           LWM2MCC_VERSION_MINOR(LWM2MCC_VERSION));

    lwm2mcc_object_register(ctx, &s_device_def);
    printf("Registered object /%u (%s)\n", s_device_def.oid, s_device_def.name);

    lwm2mcc_instance_add(ctx, 3, 0, NULL);
    printf("Added instance /3/0\n");

    lwm2mcc_instance_remove(ctx, 3, 0);
    lwm2mcc_object_unregister(ctx, 3);
    lwm2mcc_destroy(ctx);
    printf("Cleanup complete\n");

    return 0;
}
