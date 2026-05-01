# Versioning

Compile-time LWM2M protocol version selection, inspired by Vulkan's versioning macros.

## Version Macros

```c
/* Pack major.minor.patch into a single comparable uint32_t
 * Layout: [31:22] major (10 bits), [21:12] minor (10 bits), [11:0] patch (12 bits) */
#define LWM2MCC_MAKE_VERSION(major, minor, patch) \
    ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

#define LWM2MCC_VERSION_MAJOR(v) (((uint32_t)(v) >> 22U) & 0x3FFU)
#define LWM2MCC_VERSION_MINOR(v) (((uint32_t)(v) >> 12U) & 0x3FFU)
#define LWM2MCC_VERSION_PATCH(v) ((uint32_t)(v) & 0xFFFU)

#define LWM2MCC_VERSION_1_0 LWM2MCC_MAKE_VERSION(1, 0, 0)
#define LWM2MCC_VERSION_1_1 LWM2MCC_MAKE_VERSION(1, 1, 0)
#define LWM2MCC_VERSION_1_2 LWM2MCC_MAKE_VERSION(1, 2, 0)
```

## Compile-Time Selection

Set via CMake:
```bash
cmake -DLWM2M_TARGET_VERSION=1.1 ..
```

Defaults to 1.2. CMake parses the string and defines `LWM2MCC_VERSION` as the packed macro.

If `LWM2MCC_VERSION` is not defined (e.g. not using CMake), `version.h` defaults to 1.2.

## Feature Guards in Code

```c
#if LWM2MCC_VERSION >= LWM2MCC_VERSION_1_1
    // SenML JSON/CBOR, Send, Composite ops
#endif

#if LWM2MCC_VERSION >= LWM2MCC_VERSION_1_2
    // LWM2M 1.2 specific features
#endif
```

## Printing the Version

The extraction macros involve bitwise ops that can't be stringified by the preprocessor. Use printf at runtime:

```c
printf("version %u.%u\n",
       LWM2MCC_VERSION_MAJOR(LWM2MCC_VERSION),
       LWM2MCC_VERSION_MINOR(LWM2MCC_VERSION));
```

## Registration String

The version is advertised during registration:
```
POST /rd?ep=my_device&lwm2m=1.2&lt=86400&b=U
```

## What Changes Per Version

| Feature                | 1.0 | 1.1 | 1.2 |
|------------------------|-----|-----|-----|
| TLV encoding           | yes | yes | yes |
| JSON encoding (old)    | yes | yes | yes |
| SenML JSON encoding    |     | yes | yes |
| SenML CBOR encoding    |     | yes | yes |
| Send operation         |     | yes | yes |
| Composite Read         |     | yes | yes |
| Composite Write        |     | yes | yes |
| Composite Observe      |     | yes | yes |
| LwM2M CBOR encoding   |     |     | yes |
| Gateway functionality  |     |     | yes |

## Why Compile-Time?

- **Code size** — compiling out unused codecs and operations saves flash on embedded targets
- **No runtime surprises** — you know at build time exactly which features are available
- **Simple** — one macro check, no runtime version negotiation
