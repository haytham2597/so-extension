#ifndef BASE_INSTRUCTIONS_H
#define BASE_INSTRUCTIONS_H

#ifndef MODULES_SOCKETS_NETWORK_H
#include <modules/sockets/network.h>
#endif
#include "utils/structs.h"
#ifndef EXTS_COMMON_EXT
#include "exts/common_ext.h"
#endif

#ifndef EXTS_CAST_EXT
#include "exts/cast_ext.h"
#endif

#ifndef BASE_MMU_H
#include "../mmu/base_mmu.h"
#endif
#ifndef MEMORY_MANAGEMENT_UNIT_H
#include "../mmu/memory_management_unit.h"
#endif

config_cpu cc;

instruction* create_instruction(char* instrstr){
    instruction* instr = malloc(sizeof(instruction));
    instr->resource = malloc(MAX_INPUT);
    instr->filename = malloc(MAX_INPUT);
    char** spl = string_split(instrstr, " ");
    instr->icode = cast_code(spl[0]);
    int len = string_array_size(spl)-1;
    instr->args = malloc(sizeof(void*)*len);
    instr->types = malloc(sizeof(datatype)*len);
    string_array_destroy(spl);
    return instr;
}

char* instructions_to_params_string(instruction* instr){
    //TODO: Test this and fix
    char* result = string_new();
    for(int i=0;i<instr->sz_args;i++)
    {
        datatype dt = instr->types[i];
        //char* with = string_new();
        if(dt == DT_UNSIGNEDINT)
        {
            uint32_t r=0;
            memcpy(&r, instr->args[i], sizeof(uint32_t));
            string_append_with_format(&result, "%d", r);
            //with = string_from_format("%d", r);
            //with = string_itoa(r);
        }
        if(dt == DT_STRING){
            
            string_append_with_format(&result, "%s", instr->resource);
            //with = string_from_format("%s", instr->resource);
            /*with = malloc(strlen(instr->resource)+1);
            strcpy(with, instr->resource);*/
            //with = 
            //with = buffer_to_string(&instr->args[i]);
        }
        /*if(with == NULL)
            continue;
        if(string_is_empty(with)){
            free(with);
            continue;
        }*/
        /*string_append(&result, with);
        free(with);*/
        if(i!=instr->sz_args-1)
            string_append(&result, ",");
    }
    return result;
}

/// @brief Obtiene cantidad de argumento en base al código de instrucción
/// @param icode 
/// @return 
int len_args(instr_code icode){
    /*if(!is_syscalls(icode))
        return (icode >= SET && icode <= JNZ) ? 2 : 1;
    if(icode == PROCESS_CREATE)
        return 3;
    if(icode >= THREAD_CREATE && icode <= JNZ)
        return 2;
    if(icode >= LOG && icode <= MUTEX_UNLOCK)
        return 1;
    return 0;*/
    return 0;
}
#endif