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
    MULTILEVEL_SCHEDULER,
    LONG_SCHEDULER,
}scheduler_mode;

typedef enum{
    /// @brief First in First Out
    FIFO=LONG_SCHEDULER+1,
    /// @brief Planificador SJF con desalojo
    SJF,
    /// @brief Planificador SJF sin desalojo
    SRT,
    PRIORITY,
    /// @brief Highest Response Ratio Next
    HRRN,
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
    SUCCESS_SYSCALL,
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
	REQUEST_RESOURCES,
    REQUEST_BLOCKING_REASON,
    REQUEST_INTERRUPT,
	REQUEST_DIR,
    REQUEST_CREATE_FILE,
    REQUEST_UPDATE_TCB_FIELD,
	REQUEST_EXECUTE_SCRIPT_KERNEL,
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
    EXIT,
    /// @brief No Operation, sólo va a consumir tiempo
    NOOP,
    /// @brief (Dirección, Datos)
    WRITE,
    /// @brief (Dirección, Tamaño)
    READ,
    /// @brief (Valor) Actualiza el PC del Proceso
    GOTO,
    SET,
    JNZ,
    LOG,
    SUM,
    SUB,
    MUL,
    DIV,
    INVALID_INSTRUCTION,
    PROCESS_CREATE,
    PROCESS_EXIT,
    /// @brief (Nombre del archivo Pseudocódigo, Prioridad) esta syscall recibirá como parámetro de la CPU el nombre del archivo de pseudocódigo que deberá ejecutar el hilo a crear y su prioridad. 
    /// Al momento de crear el nuevo hilo, deberá generar el nuevo TCB con un TID autoincremental y poner al mismo en el estado READY.
    THREAD_CREATE,
    /// @brief (TID) esta syscall recibe como parámetro un TID, mueve el hilo que la invocó al estado 
    /// BLOCK hasta que el TID pasado por parámetro finalice. En caso de que el TID pasado por parámetro 
    /// no exista o ya haya finalizado, esta syscall no hace nada y el hilo que la invocó continuará su ejecución.
    THREAD_JOIN,
    /// @brief (TID) esta syscall recibe como parámetro un TID con el objetivo de finalizarlo pasando al mismo al estado EXIT. 
    /// Se deberá indicar a la Memoria la finalización de dicho hilo. En caso de que el TID pasado por parámetro no exista o ya haya finalizado, 
    /// esta syscall no hace nada. Finalmente, el hilo que la invocó continuará su ejecución.
    THREAD_CANCEL,
    /// @brief (Recurso) esta syscall recibirá como parámetro de la CPU el nombre del archivo de pseudocódigo 
    /// que deberá ejecutar el hilo a crear y su prioridad. Al momento de crear el nuevo hilo, deberá generar 
    /// el nuevo TCB con un TID autoincremental y poner al mismo en el estado READY.
    MUTEX_CREATE,
    /// @brief (Recurso) se deberá verificar primero que exista el mutex solicitado y en caso de que exista y el mismo no se encuentre tomado se deberá asignar dicho mutex al hilo correspondiente. 
    /// En caso de que el mutex se encuentre tomado, el hilo que realizó MUTEX_LOCK se bloqueará en la cola de bloqueados correspondiente a dicho mutex.
    MUTEX_LOCK,
    /// @brief (Recurso) se deberá verificar primero que exista el mutex solicitado y esté tomado por el hilo que realizó la syscall. 
    /// En caso de que corresponda, se deberá desbloquear al primer hilo de la cola de bloqueados de ese mutex y le asignará el mutex 
    /// al hilo recién desbloqueado. 
    /// Una vez hecho esto, se devuelve la ejecución al hilo que realizó la syscall MUTEX_UNLOCK. 
    /// En caso de que el hilo que realiza la syscall no tenga asignado el mutex, no realizará ningún desbloqueo.
    MUTEX_UNLOCK,
    /// @brief esta syscall finaliza al hilo que lo invocó, pasando el mismo al estado EXIT. Se deberá indicar a la Memoria la finalización de dicho hilo.
    THREAD_EXIT,
}instr_code;


typedef enum{
    /// @brief No se asignó ningún bloqueo
    BLOCK_NONE=REQUEST_CONTEXT_EXECUTION+1,
    /// @brief Bloqueo de IO
    BLOCK_IO,
    /// @brief Bloqueo por Mutex
    BLOCK_MUTEX,
    /// @brief Bloqueo por THREAD_JOIN
    BLOCK_JOIN,
    /// @brief Bloqueo desconocido (esto no debe suceder)
    BLOCK_UNKOWN
}blocking_reason;


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