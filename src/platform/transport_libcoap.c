/**
 * @file transport_libcoap.c
 * @brief libcoap-backed transport PAL implementation
 */

#include "transport_libcoap.h"

#include "lwm2m/coap.h"
#include "lwm2m/memory.h"
#include "lwm2m/transport.h"

#include <coap3/coap.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_SESSIONS     4
#define OPT_BUF_SIZE     512
#define PAYLOAD_BUF_SIZE 2048

typedef struct {
    char            uri[LWM2MCC_ENDPOINT_URI_MAX];
    coap_session_t *session;
} session_entry_t;

typedef struct {
    coap_context_t            *coap_ctx;
    const lwm2mcc_allocator_t *allocator;
    session_entry_t            sessions[MAX_SESSIONS];
    lwm2mcc_coap_message_t     pending_msg;
    lwm2mcc_endpoint_t         pending_remote;
    uint8_t                    opt_buf[OPT_BUF_SIZE];
    uint8_t                    payload_buf[PAYLOAD_BUF_SIZE];
    bool                       has_pending;
} libcoap_ctx_t;

static coap_session_t *s_find_or_create_session(libcoap_ctx_t *ctx, const lwm2mcc_endpoint_t *remote)
{
    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (ctx->sessions[i].session != NULL &&
            strncmp(ctx->sessions[i].uri, remote->uri, LWM2MCC_ENDPOINT_URI_MAX) == 0) {
            return ctx->sessions[i].session;
        }
    }

    coap_uri_t parsed_uri;
    if (coap_split_uri((const uint8_t *)remote->uri, strlen(remote->uri), &parsed_uri) != 0) {
        return NULL;
    }

    char host[256];
    size_t host_len = parsed_uri.host.length < sizeof(host) - 1u ? parsed_uri.host.length : sizeof(host) - 1u;
    memcpy(host, parsed_uri.host.s, host_len);
    host[host_len] = '\0';

    char port_str[8];
    snprintf(port_str, sizeof(port_str), "%u", parsed_uri.port);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *res = NULL;
    if (getaddrinfo(host, port_str, &hints, &res) != 0 || res == NULL) {
        return NULL;
    }

    coap_address_t addr;
    coap_address_init(&addr);
    addr.size = (socklen_t)res->ai_addrlen;
    memcpy(&addr.addr, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    coap_session_t *session = coap_new_client_session(ctx->coap_ctx, NULL, &addr, COAP_PROTO_UDP);
    if (session == NULL) {
        return NULL;
    }

    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (ctx->sessions[i].session == NULL) {
            strncpy(ctx->sessions[i].uri, remote->uri, LWM2MCC_ENDPOINT_URI_MAX - 1);
            ctx->sessions[i].uri[LWM2MCC_ENDPOINT_URI_MAX - 1] = '\0';
            ctx->sessions[i].session                            = session;
            break;
        }
    }

    return session;
}

static coap_response_t s_response_handler(coap_session_t   *session, const coap_pdu_t *sent, const coap_pdu_t *received,
                                           const coap_mid_t  id)
{
    (void)sent;
    (void)id;

    libcoap_ctx_t *ctx = coap_get_app_data(coap_session_get_context(session));

    if (ctx->has_pending) {
        return COAP_RESPONSE_OK;
    }

    ctx->pending_msg.type = (lwm2mcc_coap_type_t)coap_pdu_get_type(received);
    ctx->pending_msg.code = (lwm2mcc_coap_code_t)coap_pdu_get_code(received);
    ctx->pending_msg.mid  = (uint16_t)coap_pdu_get_mid(received);

    coap_bin_const_t token = coap_pdu_get_token(received);
    uint8_t token_len = (uint8_t)(token.length <= LWM2MCC_COAP_TOKEN_MAX
                                               ? token.length
                                               : LWM2MCC_COAP_TOKEN_MAX);
    ctx->pending_msg.token_len = token_len;
    memcpy(ctx->pending_msg.token, token.s, token_len);

    ctx->pending_msg.option_count = 0;
    size_t opt_buf_used           = 0;

    coap_opt_iterator_t oi;
    coap_option_iterator_init(received, &oi, COAP_OPT_ALL);
    coap_opt_t *opt;
    while ((opt = coap_option_next(&oi)) != NULL &&
           ctx->pending_msg.option_count < LWM2MCC_COAP_OPTIONS_MAX) {
        uint32_t       len = coap_opt_length(opt);
        const uint8_t *val = coap_opt_value(opt);
        if (opt_buf_used + len > OPT_BUF_SIZE) {
            break;
        }
        memcpy(ctx->opt_buf + opt_buf_used, val, len);
        lwm2mcc_coap_option_t *out = &ctx->pending_msg.options[ctx->pending_msg.option_count];
        out->number                = oi.number;
        out->value                 = ctx->opt_buf + opt_buf_used;
        out->value_len             = len;
        opt_buf_used              += len;
        ctx->pending_msg.option_count++;
    }

    size_t         payload_len = 0;
    const uint8_t *payload     = NULL;
    if (coap_get_data(received, &payload_len, &payload) != 0 && payload_len > 0) {
        size_t copy_len              = payload_len < PAYLOAD_BUF_SIZE ? payload_len : PAYLOAD_BUF_SIZE;
        memcpy(ctx->payload_buf, payload, copy_len);
        ctx->pending_msg.payload     = ctx->payload_buf;
        ctx->pending_msg.payload_len = copy_len;
    } else {
        ctx->pending_msg.payload     = NULL;
        ctx->pending_msg.payload_len = 0;
    }

    ctx->pending_remote.uri[0] = '\0';
    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (ctx->sessions[i].session == session) {
            strncpy(ctx->pending_remote.uri, ctx->sessions[i].uri, LWM2MCC_ENDPOINT_URI_MAX - 1);
            ctx->pending_remote.uri[LWM2MCC_ENDPOINT_URI_MAX - 1] = '\0';
            break;
        }
    }

    ctx->has_pending = true;
    return COAP_RESPONSE_OK;
}

static int s_open(void *user_ctx)
{
    libcoap_ctx_t *ctx = user_ctx;

    ctx->coap_ctx = coap_new_context(NULL);
    if (ctx->coap_ctx == NULL) {
        return -1;
    }

    coap_context_set_app_data(ctx->coap_ctx, ctx);
    coap_register_response_handler(ctx->coap_ctx, s_response_handler);
    return 0;
}

static int s_send(void *user_ctx, const lwm2mcc_endpoint_t *remote, const lwm2mcc_coap_message_t *msg)
{
    libcoap_ctx_t *ctx = user_ctx;

    coap_session_t *session = s_find_or_create_session(ctx, remote);
    if (session == NULL) {
        return -1;
    }

    coap_pdu_t *pdu = coap_new_pdu((coap_pdu_type_t)msg->type, (coap_pdu_code_t)msg->code, session);
    if (pdu == NULL) {
        return -1;
    }

    if (msg->token_len > 0) {
        coap_add_token(pdu, msg->token_len, msg->token);
    }

    coap_optlist_t *optlist = NULL;
    for (uint8_t i = 0; i < msg->option_count; i++) {
        coap_insert_optlist(&optlist,
                            coap_new_optlist(msg->options[i].number,
                                             msg->options[i].value_len,
                                             msg->options[i].value));
    }
    if (optlist != NULL) {
        coap_add_optlist_pdu(pdu, &optlist);
        coap_delete_optlist(optlist);
    }

    if (msg->payload != NULL && msg->payload_len > 0) {
        coap_add_data(pdu, msg->payload_len, msg->payload);
    }

    return coap_send(session, pdu) == COAP_INVALID_MID ? -1 : 0;
}

static int s_recv(void *user_ctx, lwm2mcc_endpoint_t *remote, lwm2mcc_coap_message_t *msg)
{
    libcoap_ctx_t *ctx = user_ctx;

    coap_io_process(ctx->coap_ctx, COAP_IO_NO_WAIT);

    if (!ctx->has_pending) {
        return 0;
    }

    *msg             = ctx->pending_msg;
    *remote          = ctx->pending_remote;
    ctx->has_pending = false;
    return 1;
}

static int s_get_fd(void *user_ctx)
{
    libcoap_ctx_t *ctx = user_ctx;
    return coap_context_get_coap_fd(ctx->coap_ctx);
}

static void s_close(void *user_ctx)
{
    libcoap_ctx_t *ctx = user_ctx;

    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (ctx->sessions[i].session != NULL) {
            coap_session_release(ctx->sessions[i].session);
            ctx->sessions[i].session = NULL;
        }
    }

    if (ctx->coap_ctx != NULL) {
        coap_free_context(ctx->coap_ctx);
        ctx->coap_ctx = NULL;
    }
}

void lwm2mcc_libcoap_transport_init(lwm2mcc_transport_ops_t *ops, const lwm2mcc_allocator_t *allocator)
{
    coap_startup();

    libcoap_ctx_t *ctx = lwm2mcc_calloc(allocator, 1, sizeof(libcoap_ctx_t));
    ctx->allocator     = allocator;
    ops->open          = s_open;
    ops->send          = s_send;
    ops->recv          = s_recv;
    ops->get_fd        = s_get_fd;
    ops->close         = s_close;
    ops->user_ctx      = ctx;
}

void lwm2mcc_libcoap_transport_shutdown(lwm2mcc_transport_ops_t *ops)
{
    if (ops->user_ctx == NULL) {
        return;
    }

    libcoap_ctx_t *ctx = ops->user_ctx;
    s_close(ctx);
    lwm2mcc_free(ctx->allocator, ctx);
    ops->user_ctx = NULL;
    coap_cleanup();
}
