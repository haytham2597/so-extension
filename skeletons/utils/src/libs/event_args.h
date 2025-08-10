#ifndef LIBS_EVENT_ARGS_H
#define LIBS_EVENT_ARGS_H

#ifndef UTILS_STRUCTS_H
#include "../utils/structs.h"
#endif 

packet_callback_event_args parse_packet_event_arg(void* params){
    int sock_client = 0, sock_server_listen = 0;
    op_code_module ocm_from, ocm_internal;
    int offset = 0;
    memcpy(&ocm_from, params + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&sock_client, params + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&sock_server_listen, params + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&ocm_internal, params + offset, sizeof(int));
    packet_callback_event_args pcea;
    pcea.ocm_from=ocm_from;
    pcea.sock_client=sock_client;
    pcea.sock_server_listen=sock_server_listen;
    pcea.ocm_internal=ocm_internal;
    return pcea;
}

#endif