#include "main.h"

int main(int argc, char *argv[])
{
    load_config("cpu.config");
    cc = load_config_cpu();

    create_log("cpu", LOG_LEVEL_TRACE);
    log_info(logger, "%s","Hola mundo");
    int sock_server_listen_dispatch = server_connection(cc.puerto_kernel_dispatch);
    int sock_server_listen_interrupt = server_connection(cc.puerto_kernel_interrupt);
    sock_memory = client_connection(cc.ip_memoria, cc.puerto_memoria);
    log_orange(logger, "Sock Memory %d", sock_memory);
    
    if (handshake(sock_memory, 0) != 0)
    {
        log_error(logger, "No se pudo conectar al servidor de memoria: %s:%d", cc.ip_memoria, cc.puerto_memoria);
        exit(EXIT_FAILURE);
    }
    else
    {
        send_john_snow_packet(MODULE_CPU_DISPATCH, sock_memory);
    }

    add_thread_by_enum(MODULE_CPU_DISPATCH);
    add_thread_by_enum(MODULE_CPU_INTERRUPT);

    pthread_mutex_t locker;
    pthread_mutex_init(&locker, NULL);
    pthread_mutex_lock(&locker);

    void* parameters = malloc(sizeof(int)*3);
    int offset = 0;
    int mcd = MODULE_CPU_DISPATCH;
    memcpy(parameters, &mcd, sizeof(int));
    offset+=sizeof(int);
    memcpy(parameters+offset, &sock_server_listen_dispatch, sizeof(int));
    offset+=sizeof(int);
    memcpy(parameters+offset, &cc.puerto_kernel_dispatch, sizeof(int));
    pthread_create(get_pthread_by_enum(MODULE_CPU_DISPATCH), NULL, attend_client, parameters);
    int res_detach = pthread_detach(*get_pthread_by_enum(MODULE_CPU_DISPATCH));
    if (res_detach != 0)
    {
        log_error(logger, "Error en Pthread Detach [%s:%d]", __func__, __LINE__);
        exit(EXIT_FAILURE);
    }
    pthread_mutex_unlock(&locker);
    //sleep(1);
    pthread_mutex_lock(&locker);
    
    mcd = MODULE_CPU_INTERRUPT;
    parameters = malloc(sizeof(int)*3);
    offset=0;
    memcpy(parameters, &mcd, sizeof(int));
    offset+=sizeof(int);
    memcpy(parameters+offset, &sock_server_listen_interrupt, sizeof(int));
    offset+=sizeof(int);
    memcpy(parameters+offset, &cc.puerto_kernel_interrupt, sizeof(int));
    pthread_create(get_pthread_by_enum(MODULE_CPU_INTERRUPT), NULL, attend_client, parameters);

    res_detach = pthread_detach(*get_pthread_by_enum(MODULE_CPU_INTERRUPT));
    if (res_detach != 0)
    {
        log_error(logger, "Error en Pthread Detach [%s:%d]", __func__, __LINE__);
        exit(EXIT_FAILURE);
    }
    pthread_mutex_unlock(&locker);
    
    pthread_mutex_destroy(&locker);
}

void* attend_client(void *params)
{
    op_code_module ocm;
    int sock_server_listen = 0, port = 0;
    int offset = 0;
    memcpy(&ocm, params + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&sock_server_listen, params + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&port, params + offset, sizeof(int));
    free(params);
    log_debug(logger, "Atendiendo cliente en puerto %d del servidor %d del modulo %s", port, sock_server_listen, ocm_to_string(ocm));

    int sock_client = wait_client(sock_server_listen, port);
    if (ocm == MODULE_CPU_DISPATCH)
    {
        sock_client_dispatch = sock_client;
    }
    else if (ocm == MODULE_CPU_INTERRUPT){
        sock_client_interrupt = sock_client;
    }
    log_debug(logger, "Socket cliente %d del (%s)", sock_client, ocm_to_string(ocm));

    if (handshake(sock_client, 1) != 0)
        return NULL;
    t_list *list = recv_operation_packet(sock_client);
    log_debug(logger, "Cantidad de elementos en list %d, en %s:%d", list_size(list), __func__, __LINE__);
    op_code_module ocmfrom;
    memcpy(&ocmfrom, list_get(list, 0), sizeof(op_code_module));
    log_debug(logger, "Socket conectado ocm %s", ocm_to_string(ocmfrom));
    list_destroy_and_destroy_elements(list, free_element);
    void *parameter = malloc(sizeof(int) * 4);
    offset = 0;
    memcpy(parameter, &ocmfrom, sizeof(int));
    offset += sizeof(int);
    memcpy(parameter + offset, &sock_client, sizeof(int));
    offset += sizeof(int);
    memcpy(parameter + offset, &sock_server_listen, sizeof(int));
    offset += sizeof(int);
    memcpy(parameter + offset, &ocm, sizeof(int));
    loop_network(sock_client, packet_callback, parameter);
    free(parameter);
    return NULL;
}


void packet_callback(void *params)
{
    log_warning(logger, "[PACKET_CALLBACK] Iniciando procesamiento de paquete");
    packet_callback_event_args pcea = parse_packet_event_arg(params);
    int sock_client = 0, sock_server_listen = 0;
    op_code_module ocm_from, ocm_internal;
    sock_client = pcea.sock_client;
    sock_server_listen = pcea.sock_server_listen;
    ocm_from = pcea.ocm_from;
    ocm_internal = pcea.ocm_internal;
    /*int offset = 0;
    memcpy(&ocm_from, params + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&sock_client, params + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&sock_server_listen, params + offset, sizeof(int));
    offset += sizeof(int);
    memcpy(&ocm_internal, params + offset, sizeof(int));*/
    log_debug(logger, "[IGNORAR] Sockets - Client: %d, Server Listen: %d, Memory: %d,OCMFROM: %s, OCMInternal %s", sock_client, sock_server_listen, sock_memory, ocm_to_string(ocm_from), ocm_to_string(ocm_internal));
    t_list* packet = recv_packet(sock_client);
    if (packet == NULL)
    {
        log_error(logger, "Error al recibir el paquete desde el socket %d", sock_client);
        return;
    }
    log_debug(logger, "Paquete recibido con tamanio: %d", list_size(packet));

    if (ocm_from == MODULE_KERNEL)
    {    
        int method = 0;
        memcpy(&method, list_get(packet, 0), sizeof(int));
        log_warning(logger, "[PACKET_CALLBACK] Procesando paquete desde KERNEL - Tipo: %s", ocm_internal == MODULE_CPU_DISPATCH ? "DISPATCH" : "INTERRUPT");

        if (method == SUCCESS_SYSCALL || method == ERROR) {
            // Es una respuesta de syscall
            int icode = 0;
            memcpy(&icode, list_get(packet, 1), sizeof(int));
            if(icode == PROCESS_EXIT || icode == THREAD_EXIT){
                log_debug(logger, "SE RECIBIO PROCESS EXIT EXITOSO");
                sem_post(&sem_syscall_response);
                //notify_kernel_process_end(current_tcb, NETX_TCB);

                sem_post(&sem_tcb_ready);
                is_running = 0;
                isOnFetch = 0;
                syscall_in_progress = 0;
                /*free_tcb(current_tcb); // Libera el TCB actual
                current_tcb = NULL; // Evita usar un TCB inválido*/
                list_destroy_and_destroy_elements(packet, free_element);
                return;
            }
            //handle_syscall_response(method);
            list_destroy_and_destroy_elements(packet, free_element);
            return;
        }
        
        /*if (ocm_internal == MODULE_CPU_DISPATCH)
        {
            kernel_work_of_dispatch(packet, sock_client, sock_server_listen);
        }
        if (ocm_internal == MODULE_CPU_INTERRUPT)
        {
          
            log_info(logger, "## Llega interrupcion al puerto interrupt");
            check_interrupt(packet, sock_client);
        }*/
        list_destroy_and_destroy_elements(packet, free_element);
        return;
    }

    if (ocm_from == MODULE_MEMORY) {
        log_debug(logger, "Me llego algo de MODULE_MEMORY en %s:%d", __func__, __LINE__);
        int method = 0;
        memcpy(&method, list_get(packet, 0), sizeof(int));
    
        if (method == SEGMENTATION_FAULT) {
            //DANGER: PROCESS_EXIT NO ES UN instruction*
            //syscalls_instrs(PROCESS_EXIT, current_tcb);
        }
    }
    list_destroy_and_destroy_elements(packet, free_element);
    //list_destroy_and_destroy_elements(packet, free_element);
    // free(packet);
}