#include "main.h"

sem_t sem_thread;
int main(int argc, char* argv[]) {
    
    load_config("kernel.config");
    ck = load_config_kernel();
    itself_ocm = MODULE_KERNEL;
    create_log("kernel", ck.log_level);
    log_debug(logger, "Hola soy el kernel");
    if(argc != 3){
        log_error(logger, "La cantidad de argumentos no coincide tiene que especificar archivo pseudocódigo (char*) y tamaño de proceso (int)");
        return 1;
    } 
    pthread_mutex_t locker;
    pthread_mutex_init(&locker, NULL);
    process_size=0;
    
    file = malloc(MAX_INPUT);
    memcpy(file, argv[1], MAX_INPUT);
    char* nm = malloc(MAX_INPUT);
    memcpy(nm, argv[2], MAX_INPUT);
    
    process_size = atoi(nm);
    free(nm);
    log_debug(logger, "File: %s, sz: %d", file, process_size);
    add_thread_by_int(hash((unsigned char*)"console"));
    int arr[] = {SHORT_SCHEDULER,LONG_SCHEDULER,MODULE_CPU_DISPATCH,MODULE_CPU_INTERRUPT};
    for(int i=0;i<4;i++){
        pthread_mutex_lock(&locker);
        int v = arr[i];
        add_thread_by_enum(v);
        pthread_mutex_unlock(&locker);
    }
    for(int i=MODULE_CPU_DISPATCH;i<=MODULE_CPU_INTERRUPT;i++){
        pthread_mutex_lock(&locker);
        pthread_t* pth = get_pthread_by_enum(i);
        int res= 0;
        void* parameters = malloc(sizeof(int));
        memcpy(parameters, &i, sizeof(int));
        res = pthread_create(
            pth,
            NULL, 
            connect_to_server, 
            parameters
        );
        log_debug(logger, "Res pthread_create: %d", res);
        res = pthread_detach(*pth);
        pthread_mutex_unlock(&locker);
        sem_wait(&sem_thread);
        //log_debug(logger, "Res detach: %d", res);
    }
    pthread_mutex_destroy(&locker);
    
    /*sem_wait(&sem_thread);
    run_scheduler();
    sem_wait(&sem_thread);*/
    
    //instance_create_process();
    //console();
    return 0;
}

void* connect_to_server(void* params){
    sem_post(&sem_thread);
    op_code_module ocm;
    memcpy(&ocm, params, sizeof(op_code_module));
    free(params);
    log_debug(logger, "Connectando al servidor: %s", ocm_to_string(ocm)); 
    int wcl = 0; 
    if(ocm == MODULE_CPU_DISPATCH){
        wcl = client_connection(ck.ip_cpu,ck.puerto_escucha_dispatch);
        sock_cpu_dispatch = wcl;
    }
    if(ocm == MODULE_CPU_INTERRUPT){
        wcl = client_connection(ck.ip_cpu,ck.puerto_escucha_interrupt);
        sock_cpu_interrupt = wcl;
    }
    if(handshake(wcl, 0) != 0)
    {
        log_error(logger, "No pudo hacer handshake con el socket %d del modulo %s exit(1) is invoked", wcl, ocm_to_string(ocm));
        exit(EXIT_FAILURE);
    }
    send_john_snow_packet(itself_ocm, wcl);
    add_socket_structure_by_name_ocm_sock_server(ocm_to_string(ocm), ocm, wcl, 0);
    
    void* parameters = malloc(sizeof(int)*3);
    int len_args = 2;
    int offset = 0;
    memcpy(parameters, &len_args, sizeof(int));
    offset+=sizeof(int);
    memcpy(parameters+offset, &ocm, sizeof(int));
    offset+=sizeof(int);
    memcpy(parameters+offset, &wcl, sizeof(int));

    sem_post(&sem_thread);

    loop_network(wcl, packet_callback, parameters);
    free(parameters);
    return NULL;
}

void packet_callback(void* params){
    int cntargs = 0;
    int sock = -1;
    op_code_module ocm=0;

    int offset = 0;
    memcpy(&cntargs, params+offset, sizeof(int));
    offset+=sizeof(int);
    memcpy(&ocm, params+offset, sizeof(int));
    offset+=sizeof(int);
    memcpy(&sock, params+offset, sizeof(int));
    //free(params);
    log_debug(logger, "OCM: %s", ocm_to_string(ocm));
    t_list* packet = recv_packet(sock);
    int icode = 0;
    memcpy(&icode, list_get(packet, 0), sizeof(int));
    
    if(ocm == MODULE_CPU_DISPATCH) //Me llego la respusta del CPUDISPATCH
    {
        
    }
    if(ocm == MODULE_CPU_INTERRUPT)
    {
        
    }
    list_destroy_and_destroy_elements(packet, free_element);
}

void free_up_sockets()
{
    t_list* sockets_fd = get_sockets_fds();
    for(int i=0;i<list_size(sockets_fd);i++){
        void* datafd = list_get(sockets_fd, i);
        int fd = -1;
        memcpy(&fd, datafd, sizeof(int));
        if(fd == -1){
            continue;
        }
        close(fd); //cierra todos los sockets creados y existentes.
    }
}

void instance_signal_handler(){
    if(signal(SIGINT, catch_handler_termination) == SIG_ERR){
        log_error(logger, "Problema seteando un handler para señales");
    }
    if(signal(SIGTERM, catch_handler_termination) == SIG_ERR){
        log_error(logger, "Problema seteando un handler para señales");
    }
    if(signal(SIGABRT, catch_handler_termination) == SIG_ERR){
        log_error(logger, "Problema seteando un handler para señales");
    }
}