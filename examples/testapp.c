/**
 * @file testapp.c
 * @brief Minimal test application for lwm2mcc
 */

#include "lwm2m/lwm2m.h"
#include "lwm2m/object.h"
#include "lwm2m/objects/device.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    lwm2mcc_context_t *ctx = lwm2mcc_create(NULL);

    printf("lwm2mcc context created (version %u.%u)\n", LWM2MCC_VERSION_MAJOR(LWM2MCC_VERSION),
           LWM2MCC_VERSION_MINOR(LWM2MCC_VERSION));

    lwm2mcc_register_builtin_objects(ctx);
    printf("Registered builtin objects\n");

    lwm2mcc_resource_def_t device_resources[LWM2MCC_DEVICE_RESOURCE_COUNT];
    memcpy(device_resources, LWM2MCC_DEVICE_RESOURCES, sizeof(LWM2MCC_DEVICE_RESOURCES));

    device_resources[LWM2MCC_RID_DEVICE_MANUFACTURER].presence = LWM2MCC_RES_PRESENT;
    device_resources[LWM2MCC_RID_DEVICE_MODEL_NUMBER].presence = LWM2MCC_RES_PRESENT;
    device_resources[LWM2MCC_RID_DEVICE_FIRMWARE_VERSION].presence = LWM2MCC_RES_PRESENT;

    lwm2mcc_object_def_t device_def = {
        .oid = LWM2MCC_OID_DEVICE,
        .name = "Device",
        .resources = device_resources,
        .resource_count = LWM2MCC_DEVICE_RESOURCE_COUNT,
    };

    lwm2mcc_object_register(ctx, &device_def);
    printf("Registered object /%u (%s)\n", device_def.oid, device_def.name);

    lwm2mcc_object_instance_add(ctx, LWM2MCC_OID_DEVICE, 0, NULL);
    printf("Added instance /3/0\n");

    lwm2mcc_object_instance_remove(ctx, LWM2MCC_OID_DEVICE, 0);
    lwm2mcc_object_unregister(ctx, LWM2MCC_OID_DEVICE);
    lwm2mcc_destroy(ctx);
    printf("Cleanup complete\n");

    return 0;
}
