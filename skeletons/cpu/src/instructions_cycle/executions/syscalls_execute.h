#ifndef EXECUTIONS_SYSCALLS_H
#define EXECUTIONS_SYSCALLS_H
#ifndef BASE_INSTRUCTIONS_H
#include "../base_instructions.h"
#endif

#ifndef LIBS_PACKET_H
#include "libs/packet.h"
#endif

int syscalls_instrs(instruction* instr, t_tcb* tcb){
    /*instr_code icode = instr->icode;
    if(!is_syscalls(icode))
        return ERROR;
    
    // Orden correcto propuesto
    log_debug(logger, "[SYSCALL] Enviando contexto a memoria antes de syscall...");

    send_context_execution(sock_memory);  // Primero actualizar memoria
    
    t_packet* p = create_packet();
    add_int_to_packet(p, icode);
    log_info(logger, "[SYSCALL] Iniciando syscall: %s (Código: %d)", instr_code_to_string(icode), icode);
    if(icode == IO){
        int sleep = 0;
        memcpy(&sleep, instr->args[0], sizeof(int));
        log_debug(logger, "[SYSCALL] IO - Tiempo de espera: %d", sleep);
        add_int_to_packet(p, sleep);
    }
    if(icode == PROCESS_CREATE){
        //TODO: Fix null terminated..
        //WARNING: Lo puse en RESOURCE instr->resource justamente para prevenir error de sector de memoria
        char* file_instrs = malloc(strlen(instr->resource)+1);
        memcpy(file_instrs, instr->resource, strlen(instr->resource)+1);
        int sz=0,priority=0;
        memcpy(&sz, instr->args[1], get_sizeof(instr->types[1]));
        memcpy(&priority, instr->args[2], get_sizeof(instr->types[2]));
        add_string_to_packet(p, file_instrs);
        add_int_to_packet(p, sz);
        add_int_to_packet(p, priority);
        log_debug(logger, "[SYSCALL] PROCESS_CREATE - Archivo: %s, Tamaño: %d, Prioridad: %d", instr->resource, sz, priority);
        free(file_instrs);
    }
    if (instr->icode == PROCESS_EXIT && tcb->tid != 0) {
        log_error(logger, "Sólo el TID 0 puede ejecutar PROCESS_EXIT");
        return ERROR;
    }
    if(icode == THREAD_CREATE)
    {
        char* file_instrs = malloc(strlen(instr->resource)+1);
        memcpy(file_instrs, instr->resource, strlen(instr->resource)+1);
        int priority=0;
        memcpy(&priority, instr->args[1], get_sizeof(instr->types[1]));
        log_debug(logger, "[SYSCALL] THREAD_CREATE - Archivo: %s, Prioridad: %d", file_instrs, priority);
        add_string_to_packet(p, file_instrs);  
        add_int_to_packet(p, priority);     
        log_info(logger, "[SYSCALL] THREAD_CREATE - Datos agregados al paquete: Archivo: %s, Prioridad: %d", file_instrs, priority);
        free(file_instrs);
    }
    if(icode >= THREAD_JOIN && icode <= THREAD_CANCEL)
    {
        log_info(logger, "[SYSCALL] %s - Iniciando syscall.", instr_code_to_string(icode));
        int tid =0;
        memcpy(&tid, instr->args[0], get_sizeof(instr->types[0]));
        add_int_to_packet(p, tid);
        log_info(logger, "[SYSCALL] %s - TID agregado al paquete: %d, PRIORIDAD DEL PROCESO CALLER: %d", instr_code_to_string(icode), tid, tcb->priority);
    }
    if(icode >= MUTEX_CREATE && icode <= MUTEX_UNLOCK){
        add_string_to_packet(p, instr->resource);
        log_debug(logger, "[SYSCALL] %s - Recurso: %s", instr_code_to_string(icode), instr->resource);
    }
    

    log_debug(logger, "[SYSCALL] Enviando syscall al kernel...");
    add_tcb_to_packet(p, tcb);
    send_and_free_packet(p, sock_client_dispatch); // Luego notificar al kernel
    */
    return 0;
}


#endif