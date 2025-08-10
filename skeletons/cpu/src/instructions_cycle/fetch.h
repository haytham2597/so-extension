#ifndef INSTRS_FETCH_H
#define INSTRS_FETCH_H
#ifndef BASE_INSTRUCTIONS_H
#include "base_instructions.h"
#endif
#include "executions/arithmetic_execute.h"
#ifndef EXTS_CAST_EXT
#include "exts/cast_ext.h"
#endif

char* fetch(t_tcb* tcb, response* resp)
{
    if(!tcb){
        log_error(logger, "TCB NULO EN %s:%d", __func__, __LINE__);
    }
    log_info(logger,"TID: %d - FETCH - Program Counter:%d",tcb->tid,tcb->regcpu->pc);
    t_packet* p = create_packet();
    add_int_to_packet(p, REQUEST_INSTRUCTIONS_MEMORY);
    add_int_to_packet(p, tcb->pid);
    add_int_to_packet(p, tcb->tid);
    if(tcb->desalojado == 1){
        tcb->regcpu->pc += 1;
        tcb->desalojado = 0;    
    }
    add_int_to_packet(p, tcb->regcpu->pc);
    
    
    log_debug(logger, "Enviando request instructions");
    send_and_free_packet(p, sock_memory);
    
    t_list* list = recv_operation_packet(sock_memory);
    int reque=0;
    memcpy(&reque, list_get(list, 0), sizeof(int));
    log_debug(logger,"REQUEUE: %d en %s:%d", reque, __func__, __LINE__);
    if(reque == REQUEST_INSTRUCTIONS_MEMORY){
        
        memcpy(resp, list_get(list, 1), sizeof(response));
        if(*resp == ENDOFLINE){
            //send_context_execution(tcb);
            log_info(logger, "END OF LINE");
            is_running=0;
            isOnFetch = 0;
            list_destroy_and_destroy_elements(list, free_element);
            return NULL;
        }
        if(list_size(list) == 3){
            char* c = buffer_to_string(list_get(list, 2));
            list_destroy_and_destroy_elements(list, free_element);
            return c;
        }
    }
    list_destroy_and_destroy_elements(list, free_element);
    return NULL;
}

#endif