#ifndef UTILS_ENUMS_H
#define UTILS_ENUMS_H


typedef enum{
    STATE_NEW,
    STATE_READY,
    STATE_SUSPPEND_READY,
    STATE_SUSPEND_BLOCKED,
    STATE_BLOCKED,
    STATE_EXEC,
    STATE_EXIT
}state_process;

typedef enum{
    PACKET = STATE_EXIT+1,
    INSTRUCTIONS,
    STATUS,
    REQUEST
}op_code;
typedef enum{
    MODULE_KERNEL=REQUEST+1,
    MODULE_IO,
    MODULE_CPU_DISPATCH,
    MODULE_CPU_INTERRUPT,
    MODULE_MEMORY,
}op_code_module;
typedef enum{
    DT_STRING=MODULE_MEMORY+1,
    DT_INT,
    DT_UNSIGNEDINT,
    DT_SHORT,
    DT_BYTE
}datatype;

typedef enum{
    SHORT_SCHEDULER=DT_BYTE+1,
    MEDIUM_SCHEDULER,
    LONG_SCHEDULER
}scheduler_mode;

typedef enum{
    /// @brief First in First Out
    FIFO=LONG_SCHEDULER+1,
    /// @brief Planificador SJF con desalojo
    SJF,
    /// @brief Planificador SJF sin desalojo
    SRT,
    /// @brief Highest Response Ratio Next
    HRRN
}scheduler_algorithm;

typedef enum{
    AIR_FIFO=HRRN+1,
    AIR_PCMP
}algorithm_insert_ready;


typedef enum{
    /// @brief OK
    SUCCESS = AIR_PCMP+1,
    /// @brief EOL o Fuera de línea cuando no se puede recibir más líneas de un archivo. Típicamente usado en Pseudocódigos    
    ENDOFLINE,
    /// @brief Informa desde IO al Kernel que terminó la solicitud IO
    END_OF_IO,
    /// @brief Interrupción
    INTERRUPT,
    SEGMENTATION_FAULT,
    PROCESS_CANT_INITIALIZED,
    /// @brief No existe
    NOTEXISTS,
    /// @brief Replanificación
    RESCHEDULE,
    /// @brief El proceso terminó de ejecutarse
    PROCESS_FINISHED,
    /// @brief Error general
    ERROR,
}response;

typedef enum {
    SA_FIRST = ERROR+1,
    SA_BEST,
    SA_WORST
}search_algorithm;

typedef enum{
	//BEGIN Comunicación KERNEL <-> MEMORIA
    //[PID, Tamaño de Proceso, Archivo Pseudocódigo] -> [SUCCESS | ERROR]
    REQUEST_EXECUTE_PROCESS= SA_WORST+1,
    //[PID] -> [SUCCESS | ERROR]
    REQUEST_END_PROCESS,
    //[PID] -> [SUCCESS | ERROR]
    REQUEST_SUSPEND_PROCESS,
    //[PID] -> [SUCCESS | ERROR]
    REQUEST_DES_SUSPEND_PROCESS,
    //[PID] -> [t_pcb (void*) serializado]
    REQUEST_CONTEXT_EXECUTION,
    //[NONE] -> [Listas]
    REQUEST_LIST_INSTRUCTIONS,
    //[Process Size (int)] -> [SUCCESS | ERROR]
    REQUEST_CHECK_SPACE_MEMORY,
    //END Comunicacion KERNEL <-> MEMORIA
        //BEGIN Comunicación CPU <-> Memoria
    /// @brief La CPU solicita la siguiente instrucción alojada en memoria
    //[PID, PC] -> [Línea (char*)]
	REQUEST_INSTRUCTIONS_MEMORY,
    //[PID, Dirección Lógica (int)] -> [Dirección Física, Marco]
    REQUEST_PAGE,
    //[¿sería dirección lógica?] -> [SUCCESS | ERROR]
    REQUEST_UPDATE_COMPLETE_PAGE,
    //[Dirección Física] -> [Contenido correspondiente de la página a partir del byte enviado (o sea la dirección física ) char*]
    REQUEST_READ_COMPLETE_PAGE,
    //Acceso a tabla de páginas
    //Acceso a tabla de páginas
    //[Dirección Lógica/Física {INVESTIGAR} (int)] -> [El número de tabla del siguiente nivel (si es tabla intermedia) | El número de marco correspondiente si es tabla de último nivel]
    REQUEST_ACCESS_TABLE_PAGE,
    //[Dirección, Mode(Escritura o Lectura) (int)] -> [Si es lectura; Valor de la posición pedida, Si es escritura: Escribir en memoria y devolver (SUCCESS | ERROR)]
    REQUEST_ACCESS_USER_SPACE,
    //[¿Dirección?] -> [Página]
    REQUEST_ACCESS_COMPLETE_PAGE,
    REQUEST_FRAME_BY_ENTRIES,
    //[N/A] -> [Cantidad de niveles (int)]
    REQUEST_NUMBER_OF_LEVEL,
    REQUEST_WRITE,
    REQUEST_READ,
    REQUEST_UPDATE_CONTEXT_EXECUTION,
	REQUEST_INFO,
	REQUEST_ACTION,
	REQUEST_KNOW
}request;

typedef enum{
    //WARNING: lo siguientes son Syscalls
    /// @brief Recibirá 2 parámetros de la CPU, el primero será el nombre del archivo de pseudocódigo que deberá ejecutar el proceso y el segundo parámetro es el tamaño del proceso en Memoria. El Kernel creará un nuevo PCB y lo dejará en estado NEW.
    /// (Archivo de instrucciones, Tamaño)
    INIT_PROC=REQUEST_KNOW+1,
    /// @brief 
    //[PID] -> [SUCCESS | ERROR] en Memoria
    DUMP_MEMORY,
    /// @brief (Tiempo)
    IO,
    /// @brief No Operation, sólo va a consumir tiempo
    NOOP,
    /// @brief (Dirección, Datos)
    WRITE,
    /// @brief (Dirección, Tamaño)
    READ,
    /// @brief (Valor) Actualiza el PC del Proceso
    GOTO,
    SUM,
    SUB,
    INVALID_INSTRUCTION,
}instr_code;

typedef enum{

}instr_syscalls;

typedef enum{

    R_TLB=INVALID_INSTRUCTION+1, 
    R_FIFO,
    /// @brief Least Recently Used
    R_LRU
}replace_tlb;

typedef enum{
    CLOCK=R_LRU+1,
    CLOCK_M
}replace_cache;

#endif