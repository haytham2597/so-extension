#ifndef LIBS_PACKET_H
#define LIBS_PACKET_H

#ifndef UTILS_STRUCTS_H
#include "../utils/structs.h"
#endif

#include <commons/collections/list.h>

#include "../exts/string_ext.h"

#ifndef LIBS_SERIALIZATION_H
#include "serialization.h"
#endif

#include "inc/common.h"
#ifndef EXTS_COMMON_EXT
#include "../exts/common_ext.h"
#endif


/// @brief [PRIVATE]
/// @param packet 
void create_buffer(t_packet* packet){
    packet->buffer = malloc(sizeof(t_buffer_dima));
    packet->buffer->size = 0;
    packet->buffer->stream = NULL;
}

/// @brief Crea el t_packet
/// @return t_packet*
t_packet* create_packet(){
    t_packet* packet = malloc(sizeof(t_packet));
    packet->opcode = PACKET;
    create_buffer(packet);
    return packet;
}

/// @brief Crea el t_packet con opcode específico
/// @return t_packet*
t_packet* create_packet_opcode(op_code opcode){
    t_packet* packet = malloc(sizeof(t_packet));
    packet->opcode = opcode;
    create_buffer(packet);
    return packet;
}
/// @brief [PRIVATE]
/// Serializa el paquete
/// @param packet Paquete
/// @param bytes Cantidad de bytes
/// @return void*
void* serialize_packet(t_packet* packet, int bytes){
    void*magic = malloc(bytes);
    int offset=0;
    memcpy(magic+offset, &(packet->opcode), sizeof(int));
    offset+=sizeof(int);
    memcpy(magic+offset, &(packet->buffer->size), sizeof(int));
    offset+=sizeof(int);
    memcpy(magic+offset, packet->buffer->stream, packet->buffer->size);
    offset+= packet->buffer->size;
    return magic;
}
/// @brief Limpia el paquete, se debe invocar cuando no se usa más. 
/// Nota: Recordar que el t_packet* es un PUNTERO hay que tener mucho cuidado para invocar este método
/// @param packet Paquete
void free_packet(t_packet* packet){
    free(packet->buffer->stream);
    free(packet->buffer);
    free(packet);
}

void free_instruction(instruction* instrs){
    free(instrs->str);
    for(int i=0;i<instrs->sz_args;i++)
        free(instrs->args[i]);
    if(instrs->args)
        free(instrs->args);
    if(instrs->types)
        free(instrs->types);
    free(instrs);
}

/// @brief [PRIVATE]
/// @param packet Packete
/// @param value Valor
/// @param size Tamaño
void add_packet(t_packet* packet, void* value, int size){
    packet->buffer->stream = realloc(packet->buffer->stream, packet->buffer->size+size+sizeof(int));
    memcpy(packet->buffer->stream + packet->buffer->size, &size, sizeof(int));
    memcpy(packet->buffer->stream + packet->buffer->size+sizeof(int),value, size);
    packet->buffer->size += size + sizeof(int);
}

/// @brief Agrega cadena al paquete
/// @param packet Packete
/// @param str Cadena a agregar
void add_string_to_packet(t_packet* packet, char* str){
    int sz = strlen(str)+1;
    void* buf = malloc(sz);
    memcpy(buf, str, sz);
    /*int sz = 0;
    void* buf = string_to_buffersize(str, &sz);*/
    add_packet(packet, buf, sz);
    free(buf);
}
/// @brief Agrega entero al paquete
/// @param packet Paquete
/// @param v El número a agregar.
void add_int_to_packet(t_packet* packet, int v){
    add_packet(packet, &v, sizeof(int));
}

void set_opcode_to_packet(t_packet* packet, int opcode){
    packet->opcode = opcode;
}

void add_pcb_to_packet(t_packet* packet, t_pcb* pcb)
{
    int len = 0;
    void* data = serialize_pcb(pcb, &len);
    add_packet(packet, data, len);
    free(data);
}


#endif