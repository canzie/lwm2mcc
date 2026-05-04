# Project Structure

```
lwm2mcc/
├── CMakeLists.txt
├── include/
│   └── lwm2m/
│       ├── lwm2m.h           # Entry point: context lifecycle, start/stop/step
│       ├── coap.h            # CoAP message struct, code macros, option numbers
│       ├── transport.h       # Transport ops vtable (open/send/recv/get_fd/close)
│       ├── security.h        # Security ops vtable (DTLS abstraction)
│       ├── platform.h        # Platform ops vtable (now_ms, random, log)
│       ├── memory.h          # Pluggable allocator interface
│       ├── object.h          # Object model types and registration API
│       ├── version.h         # Version macros (Vulkan-style)
│       ├── objects/
│       │   └── device.h      # Device object (OID 3) resource IDs and defaults
│       └── utils/
│           └── common.h      # lwm2mcc_result_t, log level enum, log callback
├── src/
│   ├── lwm2m.c               # Context struct, create/destroy/start/stop/step
│   ├── lwm2m_internal.h      # Internal accessors (_lwm2mcc_*)
│   ├── memory.c              # Default allocator (stdlib)
│   ├── object.c              # Object/instance/resource instance registry
│   ├── objects/
│   │   ├── security.c        # Built-in Security object (OID 0)
│   │   └── server.c          # Built-in Server object (OID 1)
│   ├── platform/
│   │   ├── transport_libcoap.h
│   │   └── transport_libcoap.c  # libcoap-3 transport backend
│   ├── tools/
│   │   ├── sorted_array.h/c  # Generic sorted array with binary search
│   │   ├── min_heap.h/c      # Generic min-heap (used by timer subsystem)
│   │   └── timers.h/c        # Timer subsystem — opaque, min-heap backed
│   └── utils/
│       ├── log.h/c           # Internal logging (levels, ANSI colors)
│       └── assert.h          # LWM2MCC_ASSERT — logs FATAL then aborts
├── examples/
│   └── testapp.c             # Device object registration demo
└── docs/
    └── lwm2m_client/         # Obsidian vault (this)
```

## Layout Rules

- Public headers go in `include/lwm2m/` — this is all the user includes
- Private headers and sources live side by side in `src/`
- Internal tools (data structures) go in `src/tools/`
- Internal utilities (logging, assert) go in `src/utils/`
- Header and source share the same name (e.g. `memory.h` → `memory.c`)
- CMake auto-discovers sources via `GLOB_RECURSE`

## What Exists Now

- **lwm2m.h / lwm2m.c** — opaque `lwm2mcc_context_t`, create/destroy/start/stop/step, timer-backed event loop
- **coap.h** — `lwm2mcc_coap_message_t`, CoAP code macros, option number constants
- **transport.h / security.h / platform.h** — ops vtables for network, DTLS, and platform services
- **version.h** — `LWM2MCC_MAKE_VERSION`, version constants, compile-time selection
- **memory.h / memory.c** — allocator interface with convenience wrappers, stdlib defaults with assert on failure
- **object.h / object.c** — object model (ID typedefs, resource defs with kind enum, callbacks), object/instance/resource-instance registry backed by sorted arrays
- **objects/security.c / server.c** — built-in Security (0) and Server (1) object definitions
- **objects/device.h** — Device object (OID 3) resource ID constants and baseline definitions
- **utils/common.h** — `lwm2mcc_result_t`, log level enum, public log callback interface
- **tools/sorted_array** — generic sorted array with binary search, user-provided comparator, inline element storage
- **tools/min_heap** — generic min-heap, user-provided comparator, inline element storage
- **tools/timers** — timer subsystem backed by min-heap; opaque `lwm2mcc_timers_t`; `start`/`stop`/`destroy` per timer; `step` fires expired timers and returns capped sleep duration
- **platform/transport_libcoap** — libcoap-3 transport backend (opt-in via `-DLWM2MCC_TRANSPORT=libcoap`)
- **utils/log** — internal logging (levels, ANSI colors, timestamps)
- **utils/assert** — `LWM2MCC_ASSERT(cond, msg)` that logs via FATAL and aborts
