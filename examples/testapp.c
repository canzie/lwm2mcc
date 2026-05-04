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
    lwm2mcc_context_t *ctx = NULL;
    lwm2mcc_result_t res = lwm2mcc_create(NULL, &ctx);
    if (res != LWM2MCC_SUCCESS) {
        fprintf(stderr, "Failed to create context: %d\n", res);
        return 1;
    }

    printf("lwm2mcc context created (version %u.%u)\n", LWM2MCC_VERSION_MAJOR(LWM2MCC_VERSION),
           LWM2MCC_VERSION_MINOR(LWM2MCC_VERSION));

    res = lwm2mcc_register_builtin_objects(ctx);
    if (res != LWM2MCC_SUCCESS) {
        fprintf(stderr, "Failed to register builtin objects: %d\n", res);
        lwm2mcc_destroy(ctx);
        return 1;
    }
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

    res = lwm2mcc_object_register(ctx, &device_def);
    if (res != LWM2MCC_SUCCESS) {
        fprintf(stderr, "Failed to register object: %d\n", res);
        lwm2mcc_destroy(ctx);
        return 1;
    }
    printf("Registered object /%u (%s)\n", device_def.oid, device_def.name);

    res = lwm2mcc_object_instance_add(ctx, LWM2MCC_OID_DEVICE, 0, NULL);
    if (res != LWM2MCC_SUCCESS) {
        fprintf(stderr, "Failed to add instance: %d\n", res);
        lwm2mcc_destroy(ctx);
        return 1;
    }
    printf("Added instance /3/0\n");

    res = lwm2mcc_object_instance_remove(ctx, LWM2MCC_OID_DEVICE, 0);
    if (res != LWM2MCC_SUCCESS) {
        fprintf(stderr, "Failed to remove instance: %d\n", res);
    }

    res = lwm2mcc_object_unregister(ctx, LWM2MCC_OID_DEVICE);
    if (res != LWM2MCC_SUCCESS) {
        fprintf(stderr, "Failed to unregister object: %d\n", res);
    }

    res = lwm2mcc_destroy(ctx);
    if (res != LWM2MCC_SUCCESS) {
        fprintf(stderr, "Failed to destroy context: %d\n", res);
        return 1;
    }
    printf("Cleanup complete\n");

    return 0;
}
