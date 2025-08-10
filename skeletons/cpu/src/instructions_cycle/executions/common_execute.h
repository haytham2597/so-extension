#ifndef EXECUTIONS_COMMON_H
#define EXECUTIONS_COMMON_H
#ifndef BASE_INSTRUCTIONS_H
#include "../base_instructions.h"
#endif
#ifndef LIBS_LOGGER_H
#include "libs/logger.h"
#endif

int common_instrs(instruction* instr, t_tcb* tcb)
{ 
    /*instr_code icode = instr->icode;
    if(icode == SET){
        
        memcpy(instr->args[0], instr->args[1], sizeof(uint32_t));

        log_debug(logger, "SET: Registro actualizado con valor %d", *((uint32_t*)instr->args[1]));
        return 0;
    }
    if(icode == WRITE)
    {   
        log_warning(logger, "[WRITE_MEM] ====== Iniciando escritura en memoria ======");
        uint32_t regDatos = 0;//int base_dir = get_base_dir();
        uint32_t dirLogica = 0;
        log_debug(logger, "[WRITE_MEM] Verificando argumentos: Dirección Lógica (args[0]) = %d, Valor (args[1]) = %d", 
              *(uint32_t*)instr->args[0], *(uint32_t*)instr->args[1]);

        memcpy(&dirLogica, instr->args[0], sizeof(uint32_t));
        memcpy(&regDatos, instr->args[1], sizeof(uint32_t)); // que tipo de tam le asigno a esto? 
        //DIMA: Que lo maneje la memoria, le estoy pasando 4 bytes (uint32_t), a la CPU no le importa como joraca de tamaño se asigna. Por que eso le importa la memoria nadie más.
        log_warning(logger, "[WRITE_MEM] Dirección lógica: %d, Valor a escribir: %d", dirLogica, regDatos);
        log_warning(logger, "[WRITE_MEM] Base: %d, Límite: %d", tcb->regcpu->base, tcb->regcpu->limit);
        uint32_t base = tcb->regcpu->base;
        uint32_t limite = tcb->regcpu->limit;
        int dirFisica = translate_mmu(dirLogica, base, limite);
        
        if(dirFisica == -1){
            handle_segmentation_fault();
            return -1;
        }

        t_packet* p = create_packet();
        add_int_to_packet(p, WRITE);
        add_int_to_packet(p, dirFisica);
        add_int_to_packet(p, regDatos);
        add_int_to_packet(p, current_tcb->pid);
        add_int_to_packet(p, current_tcb->tid);
        send_and_free_packet(p, sock_memory);

        //DIMA: Diferenciese de log_debug al del info. Tenemos puesto este debug para verlos nosotros mismos si la cosa no se puso fea. El info es el log obligatorio
        log_info(logger, "## TID: %d - Acción: ESCRIBIR - Dirección Física: %d", tcb->tid, dirFisica);
        log_debug(logger, "Escribiendo valor %d en direccion fisica %d", regDatos, dirFisica);  
    }
    if(icode == READ){
        log_warning(logger, "[READ_MEM] ====== Iniciando lectura de memoria ======");
        uint32_t dirLogica = 0;
        memcpy(&dirLogica, instr->args[1], sizeof(uint32_t));
        log_warning(logger, "[READ_MEM] Dirección lógica a leer: %d", dirLogica);
        log_warning(logger, "[READ_MEM] Base: %d, Límite: %d", current_tcb->regcpu->base, current_tcb->regcpu->limit);
        uint32_t base = current_tcb->regcpu->base; 
        uint32_t limite = current_tcb->regcpu->limit;
        uint32_t dirFisica = translate_mmu(dirLogica, base, limite);
        
        if(dirFisica == -1){
            handle_segmentation_fault();
            return -1;
        }

        t_packet* p = create_packet();
        add_int_to_packet(p, READ);
        add_int_to_packet(p, dirFisica);
        add_int_to_packet(p, current_tcb->pid);
        add_int_to_packet(p, current_tcb->tid);
        send_and_free_packet(p, sock_memory);

        t_list* l = recv_operation_packet(sock_memory);
        response resp = ERROR;
        memcpy(&resp, list_get(l, 0), sizeof(int));
        if(resp == SUCCESS){
            int value=0;
            memcpy(&value, list_get(l, 1), sizeof(int));
            memcpy(instr->args[0], &value, sizeof(uint32_t));


            
            //DIMA: El warning del compilador llora puse formato %d
            log_info(logger, "## TID: %d - Acción: LEER - Dirección Física: %d - Valor Leído: %d", current_tcb->tid, dirFisica, value);
            log_warning(logger, "[READ_MEM] ====== Fin lectura de memoria ======");
        }else{
            log_error(logger, "[READ_MEM] Error al leer memoria. Respuesta: %d", resp);
        }
        list_destroy_and_destroy_elements(l, free_element);
        
    }
    if(icode == JNZ)
    {   
        log_warning(logger, "[JNZ] ====== Iniciando salto condicional ======");
        uint32_t reg=0;
        uint32_t pc=0;
        memcpy(&reg, instr->args[0], sizeof(uint32_t));
        memcpy(&pc, instr->args[1], sizeof(uint32_t));
        log_warning(logger, "[JNZ] Valor registro: %d, PC destino: %d", reg, pc);
        if(reg != 0)
        {
            tcb->regcpu->pc = pc;
            log_warning(logger, "[JNZ] Salto realizado - Nuevo PC: %d", pc);
            return pc;
        }
        log_warning(logger, "[JNZ] No se realizó el salto");
        log_warning(logger, "[JNZ] ====== Fin salto condicional ======");
    }
    if(icode == LOG)
    {
        uint32_t v = 0;
        memcpy(&v, instr->args[0], sizeof(uint32_t));
        //TODO: show what register is ie: AX, BX, etc.
        log_info(logger, "## TID: %d - LOG - Valor del registro: %d", tcb->tid, v); 
    }*/
    return 0;
}

#endif