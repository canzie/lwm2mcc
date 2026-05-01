# Memory Management

## Approach

The library never calls malloc/free directly. All allocations go through `lwm2mcc_allocator_t` — a struct of function pointers the user provides at context creation. If none is provided, the default stdlib allocator is used.

This lets users plug in whatever memory strategy fits their platform — arenas, pool allocators, static buffers, or plain malloc.

## Allocator Interface

```c
typedef struct {
    void *(*alloc)(size_t size, void *user_ctx);
    void *(*calloc)(size_t nmemb, size_t size, void *user_ctx);
    void *(*realloc)(void *ptr, size_t size, void *user_ctx);
    void (*free)(void *ptr, void *user_ctx);
    void *user_ctx;
} lwm2mcc_allocator_t;
```

Every callback receives `user_ctx` so custom allocators can carry state (e.g. a pointer to an arena).

## Convenience Wrappers

To avoid passing `user_ctx` manually everywhere, `memory.h` provides inline wrappers:

```c
lwm2mcc_alloc(allocator, size)
lwm2mcc_calloc(allocator, nmemb, size)
lwm2mcc_realloc(allocator, ptr, size)
lwm2mcc_free(allocator, ptr)
```

## Default Allocator

`lwm2mcc_default_allocator()` returns an allocator backed by stdlib malloc/calloc/realloc/free. The default allocator asserts on allocation failure rather than returning NULL.

## Usage

```c
// Default (stdlib)
lwm2mcc_context_t *ctx = lwm2mcc_create(NULL);

// Custom allocator
lwm2mcc_allocator_t my_alloc = {
    .alloc   = my_arena_alloc,
    .calloc  = my_arena_calloc,
    .realloc = my_arena_realloc,
    .free    = my_arena_free,
    .user_ctx = &my_arena,
};
lwm2mcc_context_t *ctx = lwm2mcc_create(&my_alloc);
```
