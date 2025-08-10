#ifndef EXTS_COMMON_EXT
#define EXTS_COMMON_EXT

#ifndef UTILS_STRUCTS_H
#include "../utils/structs.h"
#endif
#include "sys/stat.h"
#include "time.h"
#define MIN(x,y) (((x)<(y)) ? (x) : (y))
#define MAX(x,y) (((x)>(y)) ? (x) : (y))

void free_element(void* elem){
    if(elem)
        free(elem);
}

/// @brief Opcode module to string
/// @param ocm Op code module
/// @return a char* of opcode module
char* ocm_to_string(int ocm){
    if((int)ocm == MODULE_KERNEL)
        return "MODULE_KERNEL";
    if(ocm == MODULE_CPU_DISPATCH)
        return "MODULE_CPU_DISPATCH";
    if(ocm == MODULE_CPU_INTERRUPT)
        return "MODULE_CPU_INTERRUPT";
    if(ocm == MODULE_MEMORY)
        return "MODULE_MEMORY";
    if(ocm == MODULE_IO)
        return "MODULE_IO";
    return string_new();
}

char* instr_to_string(instr_code icode){
    if(icode == INIT_PROC)
        return "INIT_PROC";
    if(icode == DUMP_MEMORY)
        return "DUMP_MEMORY";
    if(icode == IO)
        return "IO";
    if(icode == EXIT)
        return "EXIT";
    if(icode == NOOP)
        return "NOOP";
    if(icode == WRITE)
        return "WRITE";
    if(icode == READ)
        return "READ";
    if(icode == GOTO)
        return "GOTO";
    if(icode == SUM)
        return "SUM";
    if(icode == SUB)
        return "SUB";
    if(icode == MUL)
        return "MUL";
    if(icode == DIV)
        return "DIV";
    if(icode == INVALID_INSTRUCTION)
        return "INVALID_INSTRUCTION";
    return string_new();
}

char* state_to_string(state_process sp){
    if(sp == STATE_NEW)
        return "STATE_NEW";
    if(sp == STATE_READY)
        return "STATE_READY";
    if(sp == STATE_EXEC)
        return "STATE_EXEC";
    if(sp == STATE_BLOCKED)
        return "STATE_BLOCKED";
    if(sp == STATE_EXIT)
        return "STATE_EXIT";
    if(sp == STATE_SUSPEND_BLOCKED)
        return "STATE_SUSPEND_BLOCKED";
    if(sp == STATE_SUSPPEND_READY)
        return "STATE_SUSPEND_READY";
    return string_new();
}
char* scheduler_mode_to_string(scheduler_mode mode){
    if(mode == SHORT_SCHEDULER)
        return "SHORT_SCHEDULER";
    if(mode == MEDIUM_SCHEDULER)
        return "MEDIUM_SCHEDULER";
    if(mode == LONG_SCHEDULER)
        return "LONG_SCHEDULER";
    return string_new();
}
int file_exists(char* filename){
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? 1 : 0;
}

int file_create(char* filename){
    FILE* f = fopen(filename, "w+b"); //Quiero crear o abrir el archivo como binario
    return fclose(f);
}

/// @brief WARNING el archivo está abierto. Si no se usa más se debe cerrar
/// @param filename 
/// @return 
FILE* file_create_without_close(char* filename){
    FILE* f = fopen(filename, "w+b"); //Quiero crear o abrir el archivo como binario
    return f;
}

FILE* write_empty_seek(FILE* f, int sz, int seek){
    fseek(f, seek, SEEK_SET);
    char* buffer = (char*)malloc(sz*sizeof(char));
    for(int i=0;i<sz;i++)
        buffer[i] = '\0';
    if(fwrite(buffer, sizeof(char), sz, f) != sz){
        log_error(logger, "No se pudo escribir satisfactoriamente el archivo con tamaño: %d", sz);
        return NULL;
    }
    return f;
}

FILE* write_empty(FILE* f, int sz)
{
    return write_empty_seek(f, sz, 0);
}

/// @brief WARNING el archivo está abierto. Si no se usa más se debe cerrar
/// @param filename 
/// @return 
FILE* file_create_size_without_close(char* filename, int sz){
    FILE* f = fopen(filename, "w+b"); //Quiero crear o abrir el archivo como binario
    return write_empty(f, sz);
}


/*int file_write(char* filename, void* data, int len){
    FILE* f = fopen(filename, "wb+");
    //fwrite()
}*/

/// @brief 
/// @param f 
/// @return this NOT CLOSE THE FILE 
long int file_size(FILE* f){
    fseek(f, 0L, SEEK_END);
    long int res=  ftell(f);
    fseek(f, 0L, SEEK_SET);
    return res;
}

long int file_size_read(char* file){
    FILE* f = fopen(file, "r");
    long int res = file_size(f);
    fclose(f);
    return res;
}

long int file_size_and_close(FILE* f)
{
    long int res= file_size(f);
    fclose(f);
    return res;
}

int file_write(char* filepath, void* data, int len){
    FILE* f = fopen(filepath, "w+b");
    fwrite(data, len, 1, f);
    return fclose(f);
}

/// @brief Duerme el subproceso en tiempo de milisegundos
/// @param msec 
/// @return 
int msleep(long msec){
    struct timespec ts;
    if(msec  < 0){
        return -1;
    }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    nanosleep(&ts, NULL);
    return EXIT_SUCCESS;
}

#endif