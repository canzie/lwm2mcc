# Platform Abstraction Layer

The PAL decouples the LWM2M core from any concrete CoAP library, DTLS library, or OS. Users implement these interfaces to port the client to their platform.

## Design: Struct of Function Pointers

Each PAL interface is a struct of function pointers. The user fills it in and passes it to `lwm2m_init()`.

```c
lwm2m_transport_ops_t transport = {
    .send = my_coap_send,
    .recv = my_coap_recv,
    .get_fd = my_coap_get_fd,
};

lwm2m_security_ops_t security = {
    .handshake = my_dtls_handshake,
    .encrypt = my_dtls_encrypt,
    .decrypt = my_dtls_decrypt,
};

lwm2m_platform_ops_t platform = {
    .now_ms = my_get_time_ms,
    .log = my_log_func,
};

lwm2m_config_t config = {
    .transport = &transport,
    .security = &security,
    .platform = &platform,
    .session_arena = &session_arena,
};

lwm2m_context_t *ctx = lwm2m_init(&config);
```

## Transport Interface (CoAP)

```c
typedef struct {
    /**
     * Send a CoAP message.
     * @param ctx      User-provided transport context
     * @param remote   Remote endpoint (address + port)
     * @param data     Serialized CoAP message
     * @param len      Length in bytes
     * @return         0 on success, negative on error
     */
    int (*send)(void *ctx, const lwm2m_endpoint_t *remote,
                const uint8_t *data, size_t len);

    /**
     * Receive a CoAP message (non-blocking).
     * @param ctx      User-provided transport context
     * @param remote   [out] Source endpoint
     * @param buf      Buffer for incoming message
     * @param buf_len  Buffer capacity
     * @return         Bytes received, 0 if nothing available, negative on error
     */
    int (*recv)(void *ctx, lwm2m_endpoint_t *remote,
                uint8_t *buf, size_t buf_len);

    /**
     * Get the file descriptor for select/poll (optional).
     * Return -1 if not applicable (e.g., bare-metal).
     */
    int (*get_fd)(void *ctx);

    /** User-provided context pointer, passed as first arg to all callbacks */
    void *user_ctx;
} lwm2m_transport_ops_t;
```

### Open Question: CoAP Abstraction Level

Two possible approaches:

**Option A: Raw UDP** — the library builds and parses CoAP messages itself. The transport just sends/receives raw bytes. More control, more code in the library.

**Option B: CoAP-level** — the transport deals in CoAP messages (code, options, payload). The library doesn't touch CoAP framing. Less code in the library, but the PAL interface is richer.

Current leaning: **Option B**. LWM2M is tightly coupled to CoAP semantics (observe, block transfer, etc.), and most platforms already have a CoAP library. Forcing raw UDP means reimplementing CoAP poorly. We define a `lwm2m_coap_message_t` struct and let the transport implementation map it to/from whatever CoAP library they use.

## Security Interface (DTLS)

```c
typedef struct {
    /**
     * Perform DTLS handshake with a remote endpoint.
     * @return 0 on success, negative on error
     */
    int (*handshake)(void *ctx, const lwm2m_endpoint_t *remote,
                     const lwm2m_security_info_t *credentials);

    /**
     * Close a DTLS session.
     */
    void (*close)(void *ctx, const lwm2m_endpoint_t *remote);

    /** User-provided context pointer */
    void *user_ctx;
} lwm2m_security_ops_t;
```

Note: when DTLS is in use, the transport's send/recv go through the DTLS layer transparently. The security interface handles session setup/teardown; the actual encrypt/decrypt is internal to the DTLS implementation wrapping the transport.

## Platform Interface

```c
typedef struct {
    /**
     * Get current time in milliseconds (monotonic clock preferred).
     */
    uint64_t (*now_ms)(void *ctx);

    /**
     * Log a message.
     * @param level  Log level (ERROR, WARN, INFO, DEBUG)
     * @param msg    Null-terminated log message
     */
    void (*log)(void *ctx, lwm2m_log_level_t level, const char *msg);

    /** User-provided context pointer */
    void *user_ctx;
} lwm2m_platform_ops_t;
```

## Reference Implementations

The `src/platform/linux/` directory provides a reference implementation:

| Interface  | Backend      | Notes                          |
|-----------|-------------|-------------------------------|
| Transport | libcoap     | UDP + CoAP message mapping     |
| Security  | mbedTLS     | DTLS 1.2, PSK + certificate    |
| Platform  | POSIX       | clock_gettime, fprintf logging |

These are optional. The core library compiles and works without them.
