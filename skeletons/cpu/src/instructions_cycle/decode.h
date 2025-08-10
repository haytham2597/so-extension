#ifndef INSTRS_DECODE_H
#define INSTRS_DECODE_H

#ifndef BASE_INSTRUCTIONS_H
#include "base_instructions.h"
#endif

void* get_value(t_tcb* tcb, char* reg){
    if(string_equals_ignore_case(reg, "PX"))
        return &tcb->regcpu->pc;
    if(string_equals_ignore_case(reg, "AX"))
        return &tcb->regcpu->ax;
    if(string_equals_ignore_case(reg, "BX"))
        return &tcb->regcpu->bx;
    if(string_equals_ignore_case(reg, "CX"))
        return &tcb->regcpu->cx;
    if(string_equals_ignore_case(reg, "DX"))
        return &tcb->regcpu->dx;
    if(string_equals_ignore_case(reg, "EX"))
        return &tcb->regcpu->ex;
    if(string_equals_ignore_case(reg, "FX"))
        return &tcb->regcpu->fx;
    if(string_equals_ignore_case(reg, "GX"))
        return &tcb->regcpu->gx;
    if(string_equals_ignore_case(reg, "HX"))
        return &tcb->regcpu->hx;
    if(string_equals_ignore_case(reg, "BASE")) //Esto no deberiamos tenerlo no??? Sería raro
        return &tcb->regcpu->base;
    if(string_equals_ignore_case(reg, "LIMIT")) //Esto no deberiamos tenerlo no??? Sería raro
        return &tcb->regcpu->limit;
    return NULL;
}

void decode_instruction(instruction* instrs, t_tcb* tcb, char** spl){
    //int len = len_args(instrs->icode);
    /*int len = string_array_size(spl)-1;
    instrs->sz_args = len;
    int ll = len-1;
    for(int i=ll;i>=0;i--){
        char* c = string_array_pop(spl);
        char* cv = malloc(strlen(c)+1);
        memcpy(cv, c, strlen(c)+1);
        free(c);
        if(cv == NULL){
            continue;
        }
        if(string_is_empty(cv)){
            free(cv);
            continue;
        }
        instrs->types[i] = DT_UNSIGNEDINT;
        if(is_syscalls(instrs->icode) && i==0)
        {
            //TODO: FIX DECODE SYSCALLL
            if(is_icode_in_range(instrs->icode, MUTEX_CREATE, MUTEX_UNLOCK) || is_icode_in_range(instrs->icode, PROCESS_CREATE, THREAD_CREATE))
            {
                instrs->types[i] = DT_STRING;
                strcpy(instrs->resource,cv);
                free(cv);
                continue;
            }
        }
        if(string_length(cv) == 2 && string_ends_with(cv, "X")) //Is supossed to be register
        {
            instrs->args[i] = get_value(tcb, cv);
            free(cv);
            continue;
        }
        instrs->args[i] = malloc(sizeof(void*));
        uint32_t v = atoi(cv);
        free(cv);
        memcpy(instrs->args[i], &v, sizeof(void*));
    }
    */
}
void decode(instruction* instr, char* instrstr, t_tcb* tcb){
    if(string_contains(instrstr, "\n"))
        instrstr[strcspn(instrstr, "\n")] = 0;
    char** spl = string_split(instrstr, " ");
    log_debug(logger, "Decodificando: %s", instrstr);
    decode_instruction(instr, tcb, spl);
    string_array_destroy(spl);
}

#endif