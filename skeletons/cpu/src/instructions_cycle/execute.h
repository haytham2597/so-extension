#ifndef INSTRS_EXECUTE_H
#define INSTRS_EXECUTE_H
#ifndef EXECUTIONS_ARITHMETIC_H
#include "executions/arithmetic_execute.h"
#endif
#ifndef EXECUTIONS_COMMON_H
#include "executions/common_execute.h"
#endif
#ifndef EXECUTIONS_SYSCALLS_H
#include "executions/syscalls_execute.h"
#endif

void execute(instruction* instr, t_tcb* tcb)
{
    /*instr_code icode = instr->icode;
    int res = 0;
    
    if(is_icode_in_range(icode, PROCESS_CREATE, DUMP_MEMORY))
    {   
        set_syscall_in_progress();
        res = syscalls_instrs(instr, tcb);
        // Solo loguear y actualizar PC si no hay interrupción pendiente
        
        if (wait_syscall_response()) {
            if(current_interrupt.pending){
                return;
            }
            tcb->regcpu->pc += 1;
            char* instsrparam = instructions_to_params_string(instr);
            log_info(logger, "TID: %d - Ejecutando: %s - %s", tcb->tid, instr_code_to_string(icode), instsrparam);
            free(instsrparam);
            if (tcb != NULL) {  // Verificar que aún sea válido
                print_tcb(tcb);
            }
        }
        
        return;
    }

    // Procesamiento de instrucciones no-syscall
    if(is_icode_in_range(icode, SUM, SUB)) {
        arithmetic_instrs(instr, tcb);
        if(current_interrupt.pending){
            tcb->regcpu->pc += 1;
            send_context_execution(sock_memory);
            is_running =0;
            isOnFetch =0;
            sem_post(&sem_interrup_current);
            return;
        }
    }
    
    if(is_icode_in_range(icode, SET, WRITE) || is_icode_in_range(icode, JNZ, LOG)) {
        res = common_instrs(instr, tcb);
        if(res == -1){
            return;
        }
        if(current_interrupt.pending){
            tcb->regcpu->pc += 1;
            send_context_execution(sock_memory);
            is_running =0;
            isOnFetch =0;
            sem_post(&sem_interrup_current);
            return;
        }
    }

    log_debug(logger, "RES: %d, %d", tcb->regcpu->pc, res);
    if(res != 0) {
        tcb->regcpu->pc = res;
    } else {
        tcb->regcpu->pc += 1;
    }
    //print_tcb(current_tcb);
    char* instr_to_param = instructions_to_params_string(instr);
    log_info(logger, "TID: %d - Ejecutando: %s - %s", tcb->tid,instr_code_to_string(icode), instr_to_param);
    free(instr_to_param);*/
    //sleep(2);
}

#endif