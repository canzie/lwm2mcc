# Architecture

## Design Principles

1. **No hidden allocations** — all memory goes through the pluggable allocator, never direct malloc calls
2. **Backend agnostic** — core library has zero knowledge of libcoap, mbedTLS, or any platform
3. **Compile-time version selection** — LWM2M version features are gated by preprocessor macros
4. **Minimal surface** — the public API should be small and obvious

## Layered Architecture

```
┌─────────────────────────────────────────────┐
│              User Application               │
├─────────────────────────────────────────────┤
│              Public API (lwm2m.h)           │
├──────────┬──────────┬───────────────────────┤
│  Object  │ Regist-  │  Device Management    │
│  Model   │ ration   │  (Read/Write/Execute) │
├──────────┴──────────┴───────────────────────┤
│              Codec Layer                    │
│         (TLV / SenML JSON / CBOR)           │
├─────────────────────────────────────────────┤
│         Platform Abstraction Layer          │
│    ┌──────────┬──────────┬─────────────┐    │
│    │Transport │ Security │  Platform   │    │
│    │ (CoAP)   │ (DTLS)   │(timers,log) │    │
│    └──────────┴──────────┴─────────────┘    │
├─────────────────────────────────────────────┤
│         Concrete Backend (swappable)        │
│    e.g. libcoap + mbedTLS + Linux           │
└─────────────────────────────────────────────┘
```

## Data Flow

### Registration (Client → Server)
1. User configures Security (obj 0) and Server (obj 1) objects
2. User calls `lwm2m_start()`
3. Registration engine builds CoAP POST to `/rd`
4. Transport PAL serializes and sends via CoAP
5. Server responds with location path
6. Registration engine schedules lifetime updates

### Read Request (Server → Client)
1. CoAP GET arrives via transport PAL
2. Core parses the URI path to identify the target object/instance/resource
3. If the resource has a read callback, it is invoked
4. Codec encodes the value in the negotiated content format
5. CoAP response sent back via transport PAL

## Built-in Objects

Security (0) and Server (1) are fully library-managed — registered internally via `lwm2mcc_register_default_objects()`. The library reads their data to drive registration and connection logic.

Device (3) is auto-created with mandatory resources. The user populates values via callbacks and can add optional resources.

## Key Types

### `lwm2mcc_context_t`
The main client context (opaque). Holds the allocator and registered objects. One per client instance. Public code uses `lwm2mcc_allocator()` and `lwm2mcc_registered_objects()`, while internal code uses the private helpers in `lwm2m_internal.h`.

## Error Handling Strategy

Public API calls return `lwm2mcc_result_t` (Vulkan-style result enum), and any data outputs are returned through explicit output parameters.

Current intent by layer:
- Public API: always `lwm2mcc_result_t` return, with out-parameters for produced values.
- Internal/private helpers: direct pointers/values are allowed where appropriate, without forcing result-style wrapping.

This keeps the public surface consistent while letting internal code stay simple and efficient.

### `lwm2mcc_object_def_t`
Static description of an LWM2M object (e.g., Device /3). Holds oid, name, resource definitions (flat const array), and read/write/execute callbacks. User-owned, must outlive the registration. Callbacks are per-object — the oiid parameter distinguishes instances.

### `lwm2mcc_resource_def_t`
Describes a single resource: rid, name, type, kind (`lwm2mcc_resource_kind_t` encodes operations + multiplicity), and mandatory flag.

### `lwm2mcc_resource_kind_t`
Enum of valid resource kinds: `R`, `W`, `RW`, `E`, `RM`, `WM`, `RWM`. Prevents invalid combinations (e.g., execute + multiple).

### `lwm2mcc_allocator_t`
Pluggable allocator. See [[Memory Management]].

## Threading Model
The library is single-threaded. The user drives it by calling `lwm2m_step()` in a loop (or from a select/poll/epoll event loop). The library never creates threads.

```c
while (running) {
    struct timeval timeout;
    lwm2m_step(ctx, &timeout);
    // select/poll on transport fd with timeout
}
```
