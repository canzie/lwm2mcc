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
- [x] PAL interface headers (`transport.h`, `security.h`, `platform.h`)

**Deliverable:** A library that compiles, has an object tree you can populate and read, but doesn't talk to anything yet.

## Phase 2 — CoAP + Bootstrap + Registration
**Goal:** Connect to a Bootstrap-Server to receive credentials, then register with an LWM2M server.

Bootstrap comes first because client-initiated bootstrap is the primary provisioning path:
the device ships knowing only a Bootstrap-Server URI, so registration cannot happen until
the Bootstrap-Server has written the LWM2M server credentials into Security/Server objects.

- [x] `lwm2mcc_coap_message_t` struct and helpers (`include/lwm2m/coap.h`)
- [x] Linux/libcoap transport PAL implementation (`src/platform/transport_libcoap.c`)
- [x] `lwm2mcc_step()` event loop integration (timers + transport recv stub)
- [x] Timer subsystem (`src/tools/timers.c`) — min-heap backed, opaque, max-timeout-capped step
- [ ] Link-Format serialiser (object/instance list for `POST /rd` payload)
- [ ] Registration state machine (Register, Update, Deregister)
  - `POST /rd` with Link-Format payload
  - Handle `2.01 Created` + Location-Path
  - Schedule lifetime update `POST`s via timer
  - Deregister on stop
- [ ] Message dispatcher in `lwm2mcc_step` (route received CoAP messages to state machines)
- [ ] Unit tests (fff + CUnit) — timer subsystem, sorted array, min-heap, registration state machine
- [ ] Client-initiated bootstrap state machine
  - Detect bootstrap-server Security instance (resource 1 = true)
  - `POST /bs` on the Bootstrap-Server
  - Handle Bootstrap-Write and Bootstrap-Delete
  - Bootstrap-Finish → transition to registration
- [ ] Test against Leshan (Eclipse LWM2M server)

**Deliverable:** A client that bootstraps from Leshan's Bootstrap-Server, registers with Leshan, and stays registered.

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

**Deliverable:** Encrypted communication with a server.

## Phase 6 — Hardening & Polish
**Goal:** Production readiness.

- [ ] Queue mode (sleeping clients)
- [ ] Block-wise transfer (large payloads)
- [ ] Firmware Update object (obj 5)
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
