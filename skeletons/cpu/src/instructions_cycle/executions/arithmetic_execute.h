#ifndef EXECUTIONS_ARITHMETIC_H
#define EXECUTIONS_ARITHMETIC_H

#ifndef BASE_INSTRUCTIONS_H
#include "../base_instructions.h"
#endif

void arithmetic_instrs(instruction* instr, t_tcb* tcb)
{
    /*instr_code icode = instr->icode;
    char* instr_param = instructions_to_params_string(instr);
    log_info(logger,"TID: %d - Ejecutando: %s - %s", tcb->tid, instr_code_to_string(icode), instr_param);
    free(instr_param);
    int result=0;
    uint32_t a=0,b=0;
    memcpy(&a, instr->args[0], get_sizeof(instr->types[0]));
    memcpy(&b, instr->args[1], get_sizeof(instr->types[1]));
    log_debug(logger, "SUB Args[0]: %d, Args[1]: %d", a, b);
    if(icode == SUM) //SUM (Reg Destino, Reg. Origen) Reg Destino = Reg. Destino + Reg. Origen;
        result = a + b;
    if(icode == SUB) //SUB (Reg Destino, Reg. Origen) Reg Destino = Reg. Destino - Reg. Origen;
        result = a - b;
    log_debug(logger, "RESULT: %d",result);
    if(result <= 0) //Los uint son enteros sin signar por lo tanto no puede ser negativo o tendrá overflow (su valor sería abismal)
        result = 0; //Independiente de si es uint32_t o int ambos tienen el mismo tamaño de tipo de dato
    log_info(logger,"Resultado: %d, %d = %d",a,b, result);
    memcpy(instr->args[0], &result, get_sizeof(instr->types[0]));*/
    //log_debug(logger, "RESULT On Argument: %d",result);
}

#endif