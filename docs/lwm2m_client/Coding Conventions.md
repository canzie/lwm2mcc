# Coding Conventions

## Naming

- Public symbols: `lwm2mcc_` prefix
- Include guards: `LWM2MCC__<FILENAME>_H` (double underscore)
- Static variables and functions: `s_` prefix
- Macros/constants: `LWM2MCC_` prefix, uppercase

## Style

- Formatting handled by clang-format
- No periods at the end of Doxygen comment last lines
- Never use a non-boolean type as a boolean (e.g. `if (ptr != NULL)` not `if (ptr)`)
- Doxygen tags: keep it simple — `@file`, `@brief`, `@param`, `@return`, `@note` only

## Include Ordering

In headers:
```c
#include "lwm2m/internal_header.h"

#include <stdint.h>
```

In source files — own header first, isolated:
```c
#include "my_module.h"

#include "lwm2m/other.h"
#include "tools/sorted_array.h"

#include <string.h>
```

## File Layout

- Public headers: `include/lwm2m/`
- Public utility headers: `include/lwm2m/utils/`
- Private headers and sources: `src/`, side by side (e.g. `log.h` and `log.c` in same directory)
- Internal data structures: `src/tools/`
- Header and source always share the same base name
- CMake auto-discovers sources via `GLOB_RECURSE`

## API Style

- Public functions return `void` (no return-based error codes)
- Create-style functions return a pointer (`NULL` on failure)
- Error reporting strategy is TBD (exploring errno-like with source tracking)
- User-facing log callback for diagnostics (`utils/common.h`)

## Error Handling

- Default allocators assert on failure via `LWM2MCC_ASSERT`
- Custom allocators define their own failure behavior
- Sorted array operations assert on NULL input
