# Project Structure

```
lwm2mcc/
├── CMakeLists.txt
├── .claude/
│   └── settings.json
├── include/
│   └── lwm2m/
│       ├── lwm2m.h          # Entry point, context create/destroy
│       ├── version.h         # Version macros (Vulkan-style)
│       └── memory.h          # Pluggable allocator interface
├── src/
│   ├── lwm2m.c              # Context implementation
│   ├── memory.c              # Default allocator (stdlib)
│   └── utils/
│       ├── log.h             # Logging (levels, colors, timestamps)
│       ├── log.c
│       └── assert.h          # Assert macro with logging
├── examples/
│   └── testapp.c             # Minimal create/destroy test
└── docs/
    └── lwm2m_client/         # Obsidian vault (this)
```

## Layout Rules

- Public headers go in `include/lwm2m/` — this is all the user includes
- Private headers and sources live side by side in `src/`
- Header and source share the same name (e.g. `memory.h` → `memory.c`)
- CMake auto-discovers sources via `GLOB_RECURSE`

## What Exists Now

- **lwm2m.h / lwm2m.c** — opaque `lwm2mcc_context_t`, create/destroy
- **version.h** — `LWM2MCC_MAKE_VERSION`, version constants, compile-time selection
- **memory.h / memory.c** — allocator interface with convenience wrappers, stdlib defaults with assert on failure
- **utils/log** — spdlog-inspired logging with levels, ANSI colors, timestamps
- **utils/assert** — `LWM2MCC_ASSERT(cond, msg)` that logs via FATAL and aborts

## What's Next

As the project grows, new modules get added following the same pattern. Future files (not yet created):
- Object model (`object.h`/`object.c`)
- PAL interfaces (`transport.h`, `security.h`, `platform.h`)
- Codec implementations (TLV, SenML)
