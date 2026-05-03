/**
 * @file device.h
 * @brief LWM2M Device object (OID 3) definitions
 */

#ifndef LWM2MCC__OBJECTS_DEVICE_H
#define LWM2MCC__OBJECTS_DEVICE_H

#include "lwm2m/object.h"

#define LWM2MCC_OID_DEVICE 3

typedef enum {
    LWM2MCC_RID_DEVICE_MANUFACTURER = 0,
    LWM2MCC_RID_DEVICE_MODEL_NUMBER = 1,
    LWM2MCC_RID_DEVICE_SERIAL_NUMBER = 2,
    LWM2MCC_RID_DEVICE_FIRMWARE_VERSION = 3,
    LWM2MCC_RID_DEVICE_REBOOT = 4,
    LWM2MCC_RID_DEVICE_FACTORY_RESET = 5,
    LWM2MCC_RID_DEVICE_AVAILABLE_POWER_SOURCES = 6,
    LWM2MCC_RID_DEVICE_POWER_SOURCE_VOLTAGE = 7,
    LWM2MCC_RID_DEVICE_POWER_SOURCE_CURRENT = 8,
    LWM2MCC_RID_DEVICE_BATTERY_LEVEL = 9,
    LWM2MCC_RID_DEVICE_MEMORY_FREE = 10,
    LWM2MCC_RID_DEVICE_ERROR_CODE = 11,
    LWM2MCC_RID_DEVICE_RESET_ERROR_CODE = 12,
    LWM2MCC_RID_DEVICE_CURRENT_TIME = 13,
    LWM2MCC_RID_DEVICE_UTC_OFFSET = 14,
    LWM2MCC_RID_DEVICE_TIMEZONE = 15,
    LWM2MCC_RID_DEVICE_SUPPORTED_BINDING_AND_MODES = 16,
    LWM2MCC_RID_DEVICE_DEVICE_TYPE = 17,
    LWM2MCC_RID_DEVICE_HARDWARE_VERSION = 18,
    LWM2MCC_RID_DEVICE_SOFTWARE_VERSION = 19,
    LWM2MCC_RID_DEVICE_BATTERY_STATUS = 20,
    LWM2MCC_RID_DEVICE_MEMORY_TOTAL = 21,
    LWM2MCC_RID_DEVICE_EXTDEVINFO = 22,
} lwm2mcc_rid_device_t;

// clang-format off
static const lwm2mcc_resource_def_t LWM2MCC_DEVICE_RESOURCES[] = {
    {.rid = LWM2MCC_RID_DEVICE_MANUFACTURER,               .name = "Manufacturer",                .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_MODEL_NUMBER,               .name = "Model Number",                .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_SERIAL_NUMBER,              .name = "Serial Number",               .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_FIRMWARE_VERSION,           .name = "Firmware Version",            .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_REBOOT,                     .name = "Reboot",                      .type = LWM2MCC_RESOURCE_NONE,    .kind = LWM2MCC_RES_E,  .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = LWM2MCC_RID_DEVICE_FACTORY_RESET,              .name = "Factory Reset",               .type = LWM2MCC_RESOURCE_NONE,    .kind = LWM2MCC_RES_E,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_AVAILABLE_POWER_SOURCES,    .name = "Available Power Sources",     .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RM, .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_POWER_SOURCE_VOLTAGE,       .name = "Power Source Voltage",        .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RM, .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_POWER_SOURCE_CURRENT,       .name = "Power Source Current",        .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RM, .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_BATTERY_LEVEL,              .name = "Battery Level",               .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_MEMORY_FREE,                .name = "Memory Free",                 .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_ERROR_CODE,                 .name = "Error Code",                  .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_RM, .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = LWM2MCC_RID_DEVICE_RESET_ERROR_CODE,           .name = "Reset Error Code",            .type = LWM2MCC_RESOURCE_NONE,    .kind = LWM2MCC_RES_E,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_CURRENT_TIME,               .name = "Current Time",                .type = LWM2MCC_RESOURCE_TIME,    .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_UTC_OFFSET,                 .name = "UTC Offset",                  .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_TIMEZONE,                   .name = "Timezone",                    .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_RW, .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_SUPPORTED_BINDING_AND_MODES,.name = "Supported Binding and Modes", .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_MANDATORY, .presence = LWM2MCC_RES_PRESENT},
    {.rid = LWM2MCC_RID_DEVICE_DEVICE_TYPE,                .name = "Device Type",                 .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_HARDWARE_VERSION,           .name = "Hardware Version",            .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_SOFTWARE_VERSION,           .name = "Software Version",            .type = LWM2MCC_RESOURCE_STRING,  .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_BATTERY_STATUS,             .name = "Battery Status",              .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_MEMORY_TOTAL,               .name = "Memory Total",                .type = LWM2MCC_RESOURCE_INTEGER, .kind = LWM2MCC_RES_R,  .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
    {.rid = LWM2MCC_RID_DEVICE_EXTDEVINFO,                 .name = "ExtDevInfo",                  .type = LWM2MCC_RESOURCE_OBJLNK,  .kind = LWM2MCC_RES_RM, .mandatory = LWM2MCC_RES_OPTIONAL,  .presence = LWM2MCC_RES_ABSENT},
};
// clang-format on

#define LWM2MCC_DEVICE_RESOURCE_COUNT (sizeof(LWM2MCC_DEVICE_RESOURCES) / sizeof(LWM2MCC_DEVICE_RESOURCES[0]))

#endif /* LWM2MCC__OBJECTS_DEVICE_H */
