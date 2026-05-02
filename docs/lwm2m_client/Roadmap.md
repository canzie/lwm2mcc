# Roadmap

## Phase 1 — Foundation
**Goal:** Project skeleton, core data model, arena allocator, versioning macros.

- [x] CMake build system
- [x] Version macros (`version.h`)
- [x] Pluggable allocator (`memory.h` / `memory.c`)
- [x] Object/instance/resource data model (`object.h` / `object.c`)
- [x] Object registry (register, lookup, iterate) — backed by `sorted_array`
- [ ] Built-in object definitions: Security (0), Server (1), Device (3)
- [ ] PAL interface headers (`transport.h`, `security.h`, `platform.h`)

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
