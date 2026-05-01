# Architecture

## Design Principles

1. **No hidden allocations** — all memory comes from arenas, never from hidden malloc calls
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

## Key Types

### `lwm2m_context_t`
The main client context. Holds the object tree, registration state, and references to PAL implementations. One per client instance.

### `lwm2m_object_t`
Represents an LWM2M object (e.g., Device /3). Contains a linked list of instances and callbacks for CRUD operations.

### `lwm2m_arena_t`
A memory arena. See [[Memory Management]].

## Threading Model
The library is single-threaded. The user drives it by calling `lwm2m_step()` in a loop (or from a select/poll/epoll event loop). The library never creates threads.

```c
while (running) {
    struct timeval timeout;
    lwm2m_step(ctx, &timeout);
    // select/poll on transport fd with timeout
}
```
