/**
 * @file testapp.c
 * @brief Minimal test application for lwm2mcc
 */

#include "lwm2m/lwm2m.h"

#include <stdio.h>

int main(void)
{
    lwm2mcc_context_t *ctx = lwm2mcc_create(NULL);
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create context\n");
        return 1;
    }

    printf("lwm2mcc context created (version %u.%u)\n",
           LWM2MCC_VERSION_MAJOR(LWM2MCC_VERSION),
           LWM2MCC_VERSION_MINOR(LWM2MCC_VERSION));

    lwm2mcc_destroy(ctx);
    printf("lwm2mcc context destroyed\n");

    return 0;
}
