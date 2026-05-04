# Project Structure

```
lwm2mcc/
├── CMakeLists.txt
├── .claude/
│   └── settings.json
├── include/
│   └── lwm2m/
│       ├── lwm2m.h          # Entry point, context create/destroy, getters
│       ├── version.h         # Version macros (Vulkan-style)
│       ├── memory.h          # Pluggable allocator interface
│       ├── object.h          # Object model types and registration API
│       └── utils/
│           └── common.h      # User-facing log callback
├── src/
│   ├── lwm2m.c              # Context implementation
│   ├── memory.c              # Default allocator (stdlib)
│   ├── object.c              # Object/instance/resource instance registry
│   ├── tools/
│   │   ├── sorted_array.h    # Generic sorted array with binary search
│   │   └── sorted_array.c
│   └── utils/
│       ├── log.h             # Internal logging (levels, colors, timestamps)
│       ├── log.c
│       └── assert.h          # Assert macro with logging
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

- **lwm2m.h / lwm2m.c** — opaque `lwm2mcc_context_t`, create/destroy, getters for allocator and registered objects
- **version.h** — `LWM2MCC_MAKE_VERSION`, version constants, compile-time selection
- **memory.h / memory.c** — allocator interface with convenience wrappers, stdlib defaults with assert on failure
- **object.h / object.c** — object model (ID typedefs, resource defs with kind enum, callbacks), object/instance/resource-instance registry backed by sorted arrays
- **utils/common.h** — public log callback interface (level enum + callback typedef)
- **tools/sorted_array** — generic sorted array with binary search, user-provided comparator, inline element storage, asserts on NULL
- **utils/log** — internal logging (levels, ANSI colors, timestamps)
- **utils/assert** — `LWM2MCC_ASSERT(cond, msg)` that logs via FATAL and aborts

## What's Next

- Extend error handling docs with per-function return-code guarantees and examples
- Built-in objects follow-up: tighten Device (3) defaults/auto-create policy and complete coverage tests
- PAL interfaces (`transport.h`, `security.h`, `platform.h`)
- Event loop (`lwm2mcc_step()`)
- Codec implementations (TLV, SenML)
