# Roadmap

## Phase 1 — Foundation
**Goal:** Project skeleton, core data model, arena allocator, versioning macros.

- [x] CMake build system
- [x] Version macros (`version.h`)
- [x] Pluggable allocator (`memory.h` / `memory.c`)
- [x] Object/instance/resource data model (`object.h` / `object.c`)
- [x] Object registry (register, lookup, iterate) — backed by `sorted_array`
- [x] Resource instance management (add/remove riids per object instance)
- [x] Resource kind enum (encodes operations + multiplicity, prevents invalid combos)
- [x] Log callback interface (`utils/common.h`)
- [x] Error reporting strategy (Vulkan-style `lwm2mcc_result_t` for public API calls)
- [x] Built-in object definitions: Security (0), Server (1), Device (3) — baseline definitions and registration flow in place
- [ ] PAL interface headers (`transport.h`, `security.h`, `platform.h`)

### PAL Header Plan

`include/lwm2m/transport.h`
- Purpose: network and CoAP transport abstraction used by the core state machine.
- Minimum types: endpoint/address type, opaque transport handle/config, transport result enum mapping to `lwm2mcc_result_t`.
- Minimum operations: open/init, send, receive/poll, close.
- Constraints: transport API must not expose libcoap-specific types in public headers.

`include/lwm2m/security.h`
- Purpose: DTLS/TLS session abstraction independent of backend implementation.
- Minimum types: security mode enum (PSK, RPK, X.509), credential container, opaque security handle.
- Minimum operations: session setup, credential binding, shutdown.
- Constraints: no mbedTLS/OpenSSL types in public headers.

`include/lwm2m/platform.h`
- Purpose: platform services needed by core logic.
- Minimum operations: monotonic time (`now_ms`), optional random bytes, optional platform logging bridge.
- Constraints: avoid POSIX-only types in public API.

Acceptance criteria for Phase 1 completion of this item:
- The three headers compile as part of the public include set.
- Public APIs are backend-agnostic and contain no third-party transport/security types.
- At least one documented reference mapping in docs (Linux + libcoap + mbedTLS) explains how implementations bind to these interfaces.

**Deliverable:** A library that compiles, has an object tree you can populate and read, but doesn't talk to anything yet.

## Phase 2 — CoAP + Registration
**Goal:** Talk to an LWM2M server. Register, update, deregister.

- [ ] `lwm2m_coap_message_t` struct and helpers
- [ ] Linux/libcoap transport PAL implementation
- [ ] Registration state machine (Register, Update, Deregister)
- [ ] `lwm2m_step()` event loop integration
- [ ] Test against Leshan (Eclipse LWM2M server)

**Deliverable:** A client that registers with Leshan and stays registered.

## Phase 3 — Device Management
**Goal:** Server can Read, Write, Execute, Create, Delete resources.

- [ ] TLV encoder/decoder
- [ ] Device management request handler
- [ ] Read (single resource, object instance, object)
- [ ] Write (single resource, object instance)
- [ ] Execute
- [ ] Create / Delete object instances
- [ ] Discover (/.well-known/core and per-object)

**Deliverable:** Full Read/Write/Execute from Leshan UI.

## Phase 4 — Observation & Reporting
**Goal:** Server can observe resources and receive notifications.

- [ ] Observe/Cancel-Observe handling
- [ ] Notification generation (on change / periodic)
- [ ] Notification attributes (pmin, pmax, gt, lt, st)
- [ ] SenML JSON encoder (1.1+)
- [ ] SenML CBOR encoder (1.1+)
- [ ] Composite Read (1.1+)
- [ ] Composite Observe (1.1+)
- [ ] Send operation (1.1+)

**Deliverable:** Live-updating values visible in Leshan.

## Phase 5 — Security
**Goal:** DTLS support.

- [ ] Linux/mbedTLS security PAL implementation
- [ ] PSK (Pre-Shared Key) mode
- [ ] Certificate mode (Raw Public Key + X.509)
- [ ] Bootstrap interface
- [ ] Security credential provisioning

**Deliverable:** Encrypted communication with a server.

## Phase 6 — Hardening & Polish
**Goal:** Production readiness.

- [ ] Queue mode (sleeping clients)
- [ ] Block-wise transfer (large payloads)
- [ ] Firmware Update object (obj 5)
- [ ] Unit tests (fff + CUnit)
- [ ] CI pipeline
- [ ] Doxygen-generated API reference
- [ ] Example applications

**Deliverable:** A library someone else could pick up and use.

## Non-Goals (For Now)
- LWM2M Gateway functionality
- MQTT transport binding
- HTTP transport binding
- OSCORE security
- Multi-server bootstrap from smartcard
