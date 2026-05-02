/**
 * @file object.c
 * @brief Object model implementation
 */

#include "lwm2m/object.h"
#include "lwm2m/lwm2m.h"
#include "tools/sorted_array.h"

typedef struct {
    lwm2mcc_oid_t oid;
    const lwm2mcc_object_def_t *def;
    sorted_array_t *instances;
} object_entry_t;

typedef struct {
    lwm2mcc_oiid_t oiid;
    void *user_data;
} instance_entry_t;

static int32_t s_cmp_object(const void *a, const void *b)
{
    const object_entry_t *ea = a;
    const object_entry_t *eb = b;
    return (int32_t)ea->oid - (int32_t)eb->oid;
}

static int32_t s_cmp_instance(const void *a, const void *b)
{
    const instance_entry_t *ea = a;
    const instance_entry_t *eb = b;
    return (int32_t)ea->oiid - (int32_t)eb->oiid;
}

static sorted_array_t *s_ensure_objects(lwm2mcc_context_t *ctx)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        *objs = sorted_array_create(sizeof(object_entry_t), _Alignof(object_entry_t), s_cmp_object, lwm2mcc_allocator(ctx));
    }
    return *objs;
}

int32_t lwm2mcc_object_register(lwm2mcc_context_t *ctx, const lwm2mcc_object_def_t *def)
{
    sorted_array_t *objs = s_ensure_objects(ctx);

    object_entry_t entry = {
        .oid = def->oid,
        .def = def,
        .instances = NULL,
    };

    if (!sorted_array_insert(objs, &entry)) {
        return -1;
    }

    return 0;
}

int32_t lwm2mcc_object_unregister(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        return -1;
    }

    object_entry_t key = {.oid = oid};
    uint32_t idx = sorted_array_search(*objs, &key);
    if (idx == SORTED_ARRAY_INVALID_INDEX) {
        return -1;
    }

    object_entry_t *entry = sorted_array_at(*objs, idx);
    sorted_array_destroy(entry->instances);
    sorted_array_remove(*objs, &key);

    return 0;
}

int32_t lwm2mcc_instance_add(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid, void *user_data)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        return -1;
    }

    object_entry_t obj_key = {.oid = oid};
    uint32_t idx = sorted_array_search(*objs, &obj_key);
    if (idx == SORTED_ARRAY_INVALID_INDEX) {
        return -1;
    }

    object_entry_t *entry = sorted_array_at(*objs, idx);
    if (entry->instances == NULL) {
        entry->instances =
            sorted_array_create(sizeof(instance_entry_t), _Alignof(instance_entry_t), s_cmp_instance, lwm2mcc_allocator(ctx));
    }

    instance_entry_t inst = {
        .oiid = oiid,
        .user_data = user_data,
    };

    if (!sorted_array_insert(entry->instances, &inst)) {
        return -1;
    }

    return 0;
}

int32_t lwm2mcc_instance_remove(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        return -1;
    }

    object_entry_t obj_key = {.oid = oid};
    uint32_t idx = sorted_array_search(*objs, &obj_key);
    if (idx == SORTED_ARRAY_INVALID_INDEX) {
        return -1;
    }

    object_entry_t *entry = sorted_array_at(*objs, idx);
    if (entry->instances == NULL) {
        return -1;
    }

    instance_entry_t inst_key = {.oiid = oiid};
    if (!sorted_array_remove(entry->instances, &inst_key)) {
        return -1;
    }

    return 0;
}
