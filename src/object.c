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
    sorted_array_t *object_instances;
} object_entry_t;

typedef struct {
    lwm2mcc_rid_t rid;
    lwm2mcc_riid_t riid;
} resource_instance_entry_t;

typedef struct {
    lwm2mcc_oiid_t oiid;
    void *user_data;
    sorted_array_t *resource_instances;
} object_instance_entry_t;

static int32_t s_cmp_object(const void *a, const void *b)
{
    const object_entry_t *ea = a;
    const object_entry_t *eb = b;
    return (int32_t)ea->oid - (int32_t)eb->oid;
}

static int32_t s_cmp_object_instance(const void *a, const void *b)
{
    const object_instance_entry_t *ea = a;
    const object_instance_entry_t *eb = b;
    return (int32_t)ea->oiid - (int32_t)eb->oiid;
}

static int32_t s_cmp_resource_instance(const void *a, const void *b)
{
    const resource_instance_entry_t *ea = a;
    const resource_instance_entry_t *eb = b;
    if (ea->rid != eb->rid) {
        return (int32_t)ea->rid - (int32_t)eb->rid;
    }
    return (int32_t)ea->riid - (int32_t)eb->riid;
}

static sorted_array_t *s_ensure_objects(lwm2mcc_context_t *ctx)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        *objs = sorted_array_create(sizeof(object_entry_t), _Alignof(object_entry_t), s_cmp_object, lwm2mcc_allocator(ctx));
    }
    return *objs;
}

void lwm2mcc_object_register(lwm2mcc_context_t *ctx, const lwm2mcc_object_def_t *def)
{
    sorted_array_t *objs = s_ensure_objects(ctx);

    object_entry_t entry = {
        .oid = def->oid,
        .def = def,
        .object_instances = NULL,
    };

    sorted_array_insert(objs, &entry);
}

void lwm2mcc_object_unregister(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        return;
    }

    object_entry_t key = {.oid = oid};
    uint32_t idx = sorted_array_search(*objs, &key);
    if (idx == SORTED_ARRAY_INVALID_INDEX) {
        return;
    }

    object_entry_t *entry = sorted_array_at(*objs, idx);
    sorted_array_destroy(entry->object_instances);
    sorted_array_remove(*objs, &key);
}

void lwm2mcc_object_instance_add(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid, void *user_data)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        return;
    }

    object_entry_t obj_key = {.oid = oid};
    uint32_t idx = sorted_array_search(*objs, &obj_key);
    if (idx == SORTED_ARRAY_INVALID_INDEX) {
        return;
    }

    object_entry_t *obj = sorted_array_at(*objs, idx);
    if (obj->object_instances == NULL) {
        obj->object_instances = sorted_array_create(sizeof(object_instance_entry_t), _Alignof(object_instance_entry_t),
                                                    s_cmp_object_instance, lwm2mcc_allocator(ctx));
    }

    object_instance_entry_t obj_inst = {
        .oiid = oiid,
        .user_data = user_data,
        .resource_instances = NULL,
    };

    sorted_array_insert(obj->object_instances, &obj_inst);
}

void lwm2mcc_object_instance_remove(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        return;
    }

    object_entry_t obj_key = {.oid = oid};
    uint32_t idx = sorted_array_search(*objs, &obj_key);
    if (idx == SORTED_ARRAY_INVALID_INDEX) {
        return;
    }

    object_entry_t *obj = sorted_array_at(*objs, idx);
    if (obj->object_instances == NULL) {
        return;
    }

    object_instance_entry_t obj_inst_key = {.oiid = oiid};
    uint32_t inst_idx = sorted_array_search(obj->object_instances, &obj_inst_key);
    if (inst_idx == SORTED_ARRAY_INVALID_INDEX) {
        return;
    }

    object_instance_entry_t *obj_inst = sorted_array_at(obj->object_instances, inst_idx);
    sorted_array_destroy(obj_inst->resource_instances);
    sorted_array_remove(obj->object_instances, &obj_inst_key);
}

void lwm2mcc_resource_instance_add(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid, lwm2mcc_rid_t rid,
                                   lwm2mcc_riid_t riid)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        return;
    }

    object_entry_t obj_key = {.oid = oid};
    uint32_t obj_idx = sorted_array_search(*objs, &obj_key);
    if (obj_idx == SORTED_ARRAY_INVALID_INDEX) {
        return;
    }

    object_entry_t *obj = sorted_array_at(*objs, obj_idx);
    if (obj->object_instances == NULL) {
        return;
    }

    object_instance_entry_t inst_key = {.oiid = oiid};
    uint32_t inst_idx = sorted_array_search(obj->object_instances, &inst_key);
    if (inst_idx == SORTED_ARRAY_INVALID_INDEX) {
        return;
    }

    object_instance_entry_t *obj_inst = sorted_array_at(obj->object_instances, inst_idx);
    if (obj_inst->resource_instances == NULL) {
        obj_inst->resource_instances = sorted_array_create(sizeof(resource_instance_entry_t), _Alignof(resource_instance_entry_t),
                                                           s_cmp_resource_instance, lwm2mcc_allocator(ctx));
    }

    resource_instance_entry_t res_inst = {
        .rid = rid,
        .riid = riid,
    };

    sorted_array_insert(obj_inst->resource_instances, &res_inst);
}

void lwm2mcc_resource_instance_remove(lwm2mcc_context_t *ctx, lwm2mcc_oid_t oid, lwm2mcc_oiid_t oiid, lwm2mcc_rid_t rid,
                                      lwm2mcc_riid_t riid)
{
    sorted_array_t **objs = lwm2mcc_objects(ctx);
    if (*objs == NULL) {
        return;
    }

    object_entry_t obj_key = {.oid = oid};
    uint32_t obj_idx = sorted_array_search(*objs, &obj_key);
    if (obj_idx == SORTED_ARRAY_INVALID_INDEX) {
        return;
    }

    object_entry_t *obj = sorted_array_at(*objs, obj_idx);
    if (obj->object_instances == NULL) {
        return;
    }

    object_instance_entry_t inst_key = {.oiid = oiid};
    uint32_t inst_idx = sorted_array_search(obj->object_instances, &inst_key);
    if (inst_idx == SORTED_ARRAY_INVALID_INDEX) {
        return;
    }

    object_instance_entry_t *obj_inst = sorted_array_at(obj->object_instances, inst_idx);
    if (obj_inst->resource_instances == NULL) {
        return;
    }

    resource_instance_entry_t res_key = {
        .rid = rid,
        .riid = riid,
    };

    sorted_array_remove(obj_inst->resource_instances, &res_key);
}
