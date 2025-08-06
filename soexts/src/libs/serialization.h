#ifndef LIBS_SERIALIZATION_H
#define LIBS_SERIALIZATION_H

#ifndef LIBS_LOGGER_H
#include "../libs/logger.h"
#endif

void* serialize_pcb(t_pcb* pcb, int* len){
    void* data = malloc(sizeof(int)*4+MAX_INPUT);
    int offset=0;
    memcpy(data, &pcb->pid, sizeof(int));
    offset+=sizeof(int);
    memcpy(data+offset, &pcb->pc, sizeof(int));
    offset+=sizeof(int);
    memcpy(data+offset, &pcb->process_size, sizeof(int));
    offset+=sizeof(int);
    memcpy(data+offset, &pcb->sp, sizeof(int));
    offset+=sizeof(int);
    memcpy(data+offset, &pcb->cpu_id, MAX_INPUT);
    offset+=MAX_INPUT;
    if(len != NULL)
        *len = offset;
    return data;
}

t_pcb* deserialize_pcb(void* data){
    t_pcb* pcb = malloc(sizeof(t_pcb));
    pcb->cpu_id = malloc(MAX_INPUT);
    int offset = 0;
    memcpy(&pcb->pid, data, sizeof(int));
    offset +=sizeof(int);
    memcpy(&pcb->pc, data+offset, sizeof(int));
    offset +=sizeof(int);
    memcpy(&pcb->process_size, data+offset, sizeof(int));
    offset +=sizeof(int);
    memcpy(&pcb->sp, data+offset, sizeof(int));
    offset +=sizeof(int);
    memcpy(&pcb->cpu_id, data+offset, MAX_INPUT);

    return pcb;
}

//TODO: Agregar funciones de serializaciones/deserializaciones
void print_pcb(t_pcb* pcb){
    char* str = string_new();
    string_append_with_format(&str, "Registro TCB estado: %d", pcb->sp);
    string_append_with_format(&str, "-PID: %d", pcb->pid);
    string_append_with_format(&str, "-PC: %d", pcb->pc);
    string_append_with_format(&str, "-Tamaño proceso: %d", pcb->process_size);
    log_debug(logger, "TCB: %s", str);
    free(str);
}
#endif