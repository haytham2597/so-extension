#ifndef UTILS_STRUCTS_H
#define UTILS_STRUCTS_H

#include <unistd.h>

#ifndef UTILS_ENUMS_H
#include "enums.h"
#endif

#include "commons/collections/list.h"
#include "commons/bitarray.h"
#include "commons/temporal.h"
#include "inc/typedef.h"
//#include "exts/temporal_ext.h"
#define MAX_INPUT 255



typedef struct{
    int pagina;
    int offset;
} datos_traduccion; // aca guardamos la pagina y el offset de la busqueda de la misma :)

typedef struct{
    int pagina;
    int direccion_fisica;
    int marco;

    //hacer free cuando se desaloje un proceso/ o se elimine la cache
    void* contenido;
    bool bit_uso;
    bool bit_modificado;
} t_cache;
typedef struct{
    int nro_pagina;
    int marco_fisico;
    bool valido;
    int pid;
} elemento_tlb;

typedef struct{
	int pid;
	state_process estado;
	t_temporal* tiempo_transcurrido;
} pid_y_estado;

typedef struct{ 
	int fd_dispatch;
	int fd_interrupt;
	int id;
    bool libre;
} id_cpu;

typedef struct{
	int pid;
	char* io_solicitado;
	char* timestamp;
    bool es_dump_memory;
    int tiempo_que_pide;
    bool estaSiendoAtendido;
} bloqueo;

typedef struct{
	char* nombre;
	bool ocupado;
    int fd;
} interfaz_io;

typedef struct{
    int pid;
    char* io_pedida;
    bool sigueBloqueado;
} suspension;

typedef struct{
    int pid;
    char* timestamp;
} args_thread_bloqueo;


typedef struct{
	int pid;
	int estimado;
} pid_y_estimado;

typedef struct
{
    int pid;
    int pc;
    t_list* MetricasDeEstados;
    t_list* MetricasDeTiempoPorEstado;

    char* archivo;
    int tamanio_de_archivo;
} pcb;

typedef struct
{
    int pid;
    int pc;

    char* archivo;
    int tamanio_de_archivo;
} mini_pcb;



/// @brief [PRIVATE]
typedef struct{
    int size;
    void* stream;
}t_buffer;

typedef struct{
    /// @brief Código de operación
    op_code opcode;
    /// @brief Buffer
    t_buffer* buffer;
}t_packet;

typedef struct{
    /// @brief Identificador del proceso
    pid_t pid;
    /// @brief Program Counter
    unsigned int pc;
    /// @brief Tamaño del proceso
    unsigned int process_size;
    /// @brief Estado del proceso
    state_process sp;
    /// @brief Este campo sólo lo va a tener la memoria, Métrica de Estado TODO: investigar si es del struct metric_per_process, no estoy seguro. 
    t_list* me;
    /// @brief Este campo sólo lo va a tener la memoria, Métrica de Tiempo por Estado.
    t_list* mt;
    /// @brief No estoy seguro de si esto iría pero lo coloqué para saber si tal PCB está en X cpus
    char* cpu_id;
}t_pcb;

typedef struct{
    instr_code icode;
    int sz_args;
    char* resource;
    char* filename;
    datatype* types;
    void** args;
}instruction;

typedef struct
{
    int page;
    int frame;
    int victim;
    replace_tlb replace_tlb;
}t_tlb;

typedef struct
{
    /// @brief Cantidad de accesos a Tablas de Páginas
    int cnt_page;
    /// @brief Cantidad de Instrucciones solicitadas
    int cnt_requested_instructions;
    /// @brief Cantidad de bajadas a Swap
    int cnt_down_swap;
    /// @brief Cantidad de subidas a Memoria Principal o al espacio contiguo de memoria
    int cnt_up_mp;
    /// @brief Cantidad de lectura de memoria
    int cnt_read_mem;
    /// @brief cantidad de escritura de memoria
    int cnt_write_mem;
}metric_per_process;

typedef struct
{
    t_bitarray bitmap_swap;
    char* swap_file;
    t_list* mp;
}t_swap_memory;

typedef struct{
    pid_t pid;
    char* fullpath;
    char* filename;
    /// @brief instruction
    t_list* instructions;
}pseudocode;

typedef struct
{
    char* cpu_id;
    int sock_cpu_dispatch;
    int sock_cpu_interrupt;
    int is_running;
}t_cpus;

typedef struct{
    char* identifier;
    int sock;
}t_io;

typedef struct
{
    void* contiguous_memory;
    int sz_occupied;
}t_memory;


/*typedef struct ttpe t_table_page_entry;
//Representaría una tabla completa
typedef struct
{
    //int idx;
    int frame_page;
    int valid_bit;
    int level;
    //Entradas. Cada tabla tiene por ejemplo 4 entradas y cada entrada apunta a sus respectivas páginas
    //Memoria sabe cuántas entradas tiene por lo tanto esto es fijo
    t_table_page_entry** tables_pages_entries; 
    //t_table_page* next;
}t_table_page;

typedef struct{
    int id;
    int frame;
}t_frame;

//TODO: Improve
typedef struct ttpe
{
    int frame;
    int present;
    int used;
    int dirty;
    //Si es NULL significaría que es MARCO y no tiene que apuntar ninguna tabla de página
    t_table_page* next;
}t_table_page_entry;

typedef struct{
    int dir;
    int offset;
}t_physical_dir;*/

typedef struct
{
    /// @brief Es la cantidad total de memoria física que tiene éste módulo
    /// Creo que aplicar long es una exageración
    long total_memory_bytes;
    /// @brief Tamaño de cada página en bytes 
    int page_size_byte;
    /// @brief Número total de marcos de páginas físicos
    int num_physical_frames;
    /// @brief Estado de los marcos físicos
    t_bitarray* physical_frames_status; //0: Libre, 1: Ocupado es básicamente un bit-array
    void* contiguous_memory;
    int sz_occupied;
}memory_manager;


typedef struct page_table* next_pt;
typedef struct
{
    /// @brief Número de marco de página físico
    int frame_number;
    /// @brief 1 si la entrada es válida, 0 caso contrario
    int is_valid;
    /// @brief 1 si la página está en memoria, 0 si está en disco. Si está en 0 significaría que fue swapeado
    int is_present;
    /// @brief El offset del swap igual usualmente con el pid podría saber dónde está
    int is_modified;
    int swap_offset;
    //next_pt* next_level_table;
}page_table_entry;

typedef struct{
    page_table_entry** entries;
    //DEPRECATED
    //int num_entries;
}page_table;

/// @brief Sería el outer table
typedef struct
{
    page_table* root_page;
    int num_levels;
    /// @brief Números de entrada en cada nivel
    int* entries_per_level;
    /// @brief Utilizarlo como puntero global
    memory_manager* mem_manager;
}paging_system;
/*
typedef struct{
    page_table_entry entry;
    int level;
    int index_in_table;
    long virtual_address_prefix;
}specific_level_entry;
*/
typedef struct
{
    pid_t pid;
    unsigned int base;
    unsigned int limit;
    int occupied;
    int process_sz;
    int virtual_memory_start;
    /*/// @brief Lista de direcciones físicas que tiene
    t_list* physicals_dirs;*/
    //t_table_page* pages;
    page_table* root_page;
    //paging_system* ps;
    /// @brief Lista de estructura metric_per_process
    metric_per_process* mpp;
}t_process_memory;
/*
typedef struct{
    page_table_entry* entry;
    int level_found;
    int index_found;
}translation_result;

typedef struct{
    pid_t pid;
    long virtual_address;
}frame_mapping_info;
*/
/// @brief Para el serializado y deserializado se toma primero el ancho de toda su estructura completa. 
///El tamaño del void* data es el ancho completo - sizeof(pid_t) que es 4 bytes - otro sizeof(int) que tamb es 4 bytes
///Lo que significa que si el proceso es de 150 bytes y almacenamos eso en void* serían un total de 158 bytes. Porque toda la estructura compone esa cantidad
///Obtener len le queda de resto 154 bytes y obtener el pid_t le queda de resto 150.
typedef struct{
    long len;
    pid_t pid;
    void* data;
}swap_content;

typedef struct{
    pid_t pid;
    int n_page;
    int slot;
}swap_reg;


typedef struct
{
    op_code_module ocm_from;
    int sock_client;
    int sock_server_listen;
    op_code_module ocm_internal;
}packet_callback_event_args;

typedef struct{
    uint32_t pc;
    uint32_t ax;
    uint32_t bx;
    uint32_t cx;
    uint32_t dx;
    uint32_t ex;
    uint32_t fx;
    uint32_t gx;
    uint32_t hx;
    /// @brief Base
    uint32_t base;
    /// @brief Desplazamiento
    uint32_t limit; 
}reg_cpu;


typedef struct {
    char* resource_name;        // Nombre del recurso protegido por el mutex
    int used;          // Indica si el mutex está tomado (1) o no (0)
    tid_t tid;      // TID del hilo que ha tomado el mutex (si está tomado)
    t_list* waiting_tids;
} t_mutex;
typedef struct{
    pid_t pid;
    /// @brief Identificador único de TCB
    tid_t tid;
    /// @brief Prioridad
    int priority;
    /// @brief Estado del hilo (STATE_NEW, STATE_READY, etc.)
    state_process state;            
    /// @brief Motivo de Bloqueo
    blocking_reason motivoBloqueo;
    /// @brief Registro de CPU
    reg_cpu* regcpu;
    // Referencia al pseudocódigo
    char* filepath;      
    t_mutex* mutex;
    t_pcb* pointer_pcb;
    tid_t blocked_by_tid;
    int desalojado;
    int arrival_time;
}t_tcb;

#endif