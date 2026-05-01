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

## File Layout

- Public headers: `include/lwm2m/`
- Private headers and sources: `src/`, side by side (e.g. `log.h` and `log.c` in same directory)
- Header and source always share the same base name
- CMake auto-discovers sources via `GLOB_RECURSE`

## Error Handling

- Default allocators assert on failure via `LWM2MCC_ASSERT`
- Custom allocators define their own failure behavior
