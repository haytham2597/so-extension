#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

#include "../utils/enums.h"

#include "commons/collections/list.h"
#ifndef CONFIG_H_
#include <commons/config.h>
#endif
#ifndef LIBS_LOGGER_H
#include "logger.h"
#endif

#ifndef CAST_EXT_H
#include "../exts/cast_ext.h"
#endif
t_config* config;

typedef struct{
    char* ip_memoria;
    int puerto_memoria;
    int puerto_escucha_dispatch;
    int puerto_escucha_interrupt;
    int puerto_escucha_io;
    scheduler_algorithm algoritmo_corto_plazo;
    algorithm_insert_ready algoritmo_ingreso_a_ready;
    double alfa;
    int tiempo_suspension;
    t_log_level log_level;
}config_kernel;

typedef struct{
    char* ip_kernel;
    int puerto_kernel;
    t_log_level log_level;
}config_io;


typedef struct{
    char* ip_memoria;
    int puerto_memoria;
    char* ip_kernel;
    int puerto_kernel_dispatch;
    int puerto_kernel_interrupt;
    int entradas_tlb;
    int entradas_cache;
    int retardo_cache;
    replace_tlb reemplazo_tlb;
    replace_cache reemplazo_cache;
    t_log_level log_level;
}config_cpu;

typedef struct{
    int puerto_escucha;
    int tam_memoria;
    int tam_pagina;
    int entradas_por_tabla;
    int cantidad_niveles;
    int retardo_memoria;
    int retardo_swap;
    char* path_swapfile;
    char* dump_path;
    char* path_instrucciones;
    t_log_level log_level;
}config_memory;


void check_null_config(){
    if(config != NULL)
        return;
    if(logger != NULL){
        log_error(logger, "Config nulo");
    }
    else{
        printf("Error config nulo %s:%d", __func__, __LINE__);
    }
    exit(1);
}
/// @brief [PRIVATE]
/// @return N/A
t_log_level get_log_level(){
    char* str = config_get_string_value(config, "LOG_LEVEL");
    return log_level_from_string(str);
}

void non_exists_config(char* nonexists){
    if(logger != NULL){
        log_error(logger,"%s",nonexists);
    }else{
        printf("%s\n", nonexists);
    }
    exit(1);
}

scheduler_algorithm get_scheduler_algorithm()
{
    char* sa = config_get_string_value(config, "ALGORITMO_CORTO_PLAZO");
    if(string_equals_ignore_case(sa, "FIFO"))
        return FIFO;
    if(string_equals_ignore_case(sa, "SRT"))
        return SRT;
    if(string_equals_ignore_case(sa, "SJF"))
        return SJF;
    non_exists_config("No existe el algoritmo de planificacion");
    return FIFO;
}

algorithm_insert_ready get_air(){
    char* sa = config_get_string_value(config, "ALGORITMO_INGRESO_A_READY");
    if(string_equals_ignore_case(sa, "FIFO"))
        return AIR_FIFO;
    if(string_equals_ignore_case(sa, "PCMP"))
        return AIR_PCMP;
    non_exists_config("No existe el algoritmo de ingreso a ready");
    return AIR_FIFO;
}

/// @brief Carga los datos del kernel. 
/// Nota: previamente el método load_config(path) tiene que ser llamado
/// @return config_kernel
config_kernel load_config_kernel(){
    check_null_config();
    config_kernel ck;
    ck.ip_memoria=config_get_string_value(config, "IP_MEMORIA");
    ck.puerto_memoria=config_get_int_value(config, "PUERTO_MEMORIA");
    ck.puerto_escucha_dispatch=config_get_int_value(config, "PUERTO_ESCUCHA_DISPATCH");
    ck.puerto_escucha_interrupt=config_get_int_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    ck.puerto_escucha_io=config_get_int_value(config, "PUERTO_ESCUCHA_IO");
    ck.algoritmo_corto_plazo=get_scheduler_algorithm();
    ck.algoritmo_ingreso_a_ready=get_air();
    ck.alfa =config_get_double_value(config, "ALFA");
    ck.tiempo_suspension = config_get_int_value(config, "TIEMPO_SUSPENSION");
    ck.log_level = get_log_level();
    return ck;
}

/// @brief Carga los datos del filesystem. 
/// Nota: previamente el método load_config(path) tiene que ser llamado
/// @return config_filsystem
config_io load_config_io(){
    check_null_config();
    config_io ci;
    ci.ip_kernel=config_get_string_value(config, "IP_KERNEL");
    ci.puerto_kernel=config_get_int_value(config, "PUERTO_KERNEL");
    ci.log_level=get_log_level();
    return ci;
}

/// @brief Carga los datos del cpu. 
/// Nota: previamente el método load_config(path) tiene que ser llamado
/// @return config_cpu
config_cpu load_config_cpu(){
    check_null_config();
    config_cpu cc;
    cc.ip_memoria=config_get_string_value(config, "IP_MEMORIA");
    cc.puerto_memoria=config_get_int_value(config, "PUERTO_MEMORIA");
    cc.ip_kernel=config_get_string_value(config, "IP_KERNEL");
    cc.puerto_kernel_dispatch=config_get_int_value(config, "PUERTO_KERNEL_DISPATCH");
    cc.puerto_kernel_interrupt=config_get_int_value(config, "PUERTO_KERNEL_INTERRUPT");
    cc.entradas_tlb=config_get_int_value(config, "ENTRADAS_TLB");
    cc.entradas_cache=config_get_int_value(config, "ENTRADAS_CACHE");
    cc.retardo_cache=config_get_int_value(config, "RETARDO_CACHE");
    cc.reemplazo_tlb=cast_replace_tlb(config_get_string_value(config, "REEMPLAZO_TLB"));
    cc.reemplazo_cache=cast_replace_cache(config_get_string_value(config, "REEMPLAZO_CACHE"));
    cc.log_level = get_log_level();
    return cc;
}


/// @brief Carga los datos del memory. 
/// Nota: previamente el método load_config(path) tiene que ser llamado
/// @return config_memory
config_memory load_config_memory(){
    check_null_config();
    config_memory cm;
    cm.puerto_escucha=config_get_int_value(config, "PUERTO_ESCUCHA");
    cm.tam_memoria=config_get_int_value(config, "TAM_MEMORIA");
    cm.tam_pagina=config_get_int_value(config, "TAM_PAGINA");
    cm.entradas_por_tabla=config_get_int_value(config, "ENTRADAS_POR_TABLA");
    cm.cantidad_niveles =config_get_int_value(config, "CANTIDAD_NIVELES");
    cm.retardo_memoria=config_get_int_value(config, "RETARDO_MEMORIA");
    cm.retardo_swap=config_get_int_value(config, "RETARDO_SWAP");
    cm.path_swapfile=config_get_string_value(config, "PATH_SWAPFILE");
    cm.dump_path=config_get_string_value(config, "DUMP_PATH");
    cm.path_instrucciones=config_get_string_value(config, "PATH_INSTRUCCIONES");
    cm.log_level=get_log_level();
    return cm;
}
/// @brief Carga el archivo en variable global config
/// @param path Ruta del .config
/// @return retorna la variable  global config
t_config* load_config(char* path){
    config = config_create(path);
    if(config == NULL){
        printf("ERROR no se pudo cargar el config");
        exit(1);
    }
    return config;
}
#endif