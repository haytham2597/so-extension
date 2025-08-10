#include "main.h"
static int secure_idx=0;
int main(int argc, char* argv[]) {
    load_config("memoria.config");
    cm = load_config_memory();

    create_log("memoria", cm.log_level);    
    instance_signal_handler();
    
    /*list_process_memory = list_create();
    partition_occupied = list_create();
    list_add_range_int(partition_occupied, 0, list_size(cm.particiones)); */
    /*int visoccupied =0;
    memcpy(&visoccupied, list_get(partition_occupied, 2), sizeof(int));
    //log_debug(logger, "Partición %d ocupada: %d", 2, visoccupied);
    log_debug(logger, "Partición %d ocupada: %d", 2, visoccupied);
    return 0;*/
    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 
    int sock_server = server_connection(cm.puerto_escucha);
    /*sock_filesystem = client_connection(cm.ip_filesystem, cm.puerto_filesystem);
    log_info(logger, "Sock filesystem %d", sock_filesystem);
    if(handshake(sock_filesystem, 0) != 0)
    {
        log_error(logger, "No se pudo conectar al servidor de filesystem: %s:%d", cm.ip_filesystem, cm.puerto_filesystem);
    }
    else{
        if(sock_filesystem > 0)
            send_john_snow_packet(MODULE_MEMORY, sock_filesystem);
    }*/
    data_memory = malloc(cm.tam_memoria);
    if(data_memory == NULL) {
        log_error(logger, "Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    
    //instance_partition(); 
    void* param = malloc(sizeof(int)*2);
    memcpy(param, &sock_server, sizeof(int));
    memcpy(param+sizeof(int), &cm.puerto_escucha, sizeof(int));
    attend_multiple_clients(param);
    pthread_mutex_destroy(&lock);
    
    return 0;
}

void* attend_multiple_clients(void* params)
{
    int sock=0, port=0;
    memcpy(&sock, params, sizeof(int));
    memcpy(&port, params+sizeof(int), sizeof(int));
    log_debug(logger, "Sock: %d, Port %d", sock,port);
    free(params);
    if(sock <= 0) {
        log_error(logger, "Socket inválido en attend_multiple_clients");
        return NULL;
    }

    for(;;){
        //pthread_mutex_lock(&lock);
        log_debug(logger, "Esperando cliente en socket %d, Puerto: %d", sock, port);
        int sock_client = wait_client(sock, port);
        log_debug(logger, "Socket cliente: %d", sock_client);
        
        if(handshake(sock_client, 1) != 0)
        {
            log_error(logger, "No pudo hacer handshake con socket %d error %d [%s:%d]", sock_client, errno, __func__, __LINE__);
            close(sock_client);
            continue;  // Continuamos esperando más conexiones en lugar de retornar
        }
        t_list* l = recv_operation_packet(sock_client);
        op_code_module ocm;
        memcpy(&ocm, list_get(l, 0), sizeof(op_code_module));
        list_destroy_and_destroy_elements(l, free_element);
        if(ocm == MODULE_KERNEL){
            log_info(logger, "Kernel Conectado - FD del socket: %d", sock_client);
            t_list* li = recv_operation_packet(sock_client);
            log_debug(logger, "Tamaño de lista del MODULE_KERNEL: %d %s:%d",list_size(li), __func__, __LINE__);
            int meth = 0;
            memcpy(&meth, list_get(li, 0), sizeof(int));
            if(meth == 105725)
            {
                char* v = buffer_to_string(list_get(li, 1));
                log_info(logger, "Recibi mensajito de test de kernel valor: %s", v);
                
                t_packet* replyback = create_packet();
                char* repl = string_from_format("Envio el mensaje que me envio el kernel valor: %s", v);
                add_int_to_packet(replyback, meth);
                add_string_to_packet(replyback, repl);
                send_and_free_packet(replyback, sock_client);
                free(v);
                free(repl);
                close(sock_client);
                list_destroy_and_destroy_elements(li, free_element);
                continue;
            }
            //kernel_work(li, sock_client);
            close(sock_client);
            list_destroy_and_destroy_elements(li, free_element);
            continue;  // Continuamos esperando más conexiones en lugar de retornar
        }
        
        if(ocm == MODULE_CPU_DISPATCH) {
            log_info(logger, "CPU Conectado - FD del socket: %d", sock_client);
            sock_cpu_dispatch = sock_client;  // Guardamos el socket de CPU
        }

        add_socket_structure_by_name_ocm_sock_server(ocm_to_string(ocm), ocm, sock_client, 1);
        threads* th = add_thread_by_int(secure_idx); 
        get_pthread_by_enum(th->threads_idx);
        void* parameter = malloc(sizeof(int)*3);
        int offset = 0;
        memcpy(parameter, &sock_client, sizeof(int));
        offset+=sizeof(int);
        memcpy(parameter+offset, &ocm, sizeof(int));
        offset+=sizeof(int);
        memcpy(parameter+offset, &sock, sizeof(int));
        int res_create = pthread_create(th->pth, NULL,go_loop_net, parameter);
        int res = pthread_detach(*th->pth);
                 
        log_info(logger, "Res: %d, Res create pthread: %d", res, res_create);
            
        //sleep(1);
    }
    return NULL;
}

void* go_loop_net(void* params){
    int len=sizeof(int)*3;
    void* data = malloc(len);
    memcpy(data, params, len);
    int sock_client = 0;
    memcpy(&sock_client, data, sizeof(int));
    free(params);
    loop_network(
        sock_client,
        packet_callback,
        data
    );
    free(data);
    
    return NULL;
}

void packet_callback(void* params){
    log_info(logger, "Inside here");

    int sock_client = 0;
    op_code_module ocm= 0;
    int sock_server=0;
    int offset= 0;
    memcpy(&sock_client, params+offset, sizeof(int));
    offset+=sizeof(int);
    memcpy(&ocm, params+offset, sizeof(int));
    offset+=sizeof(int);
    memcpy(&sock_server, params+offset, sizeof(int));
    log_info(logger, "En el packet callback sock_client: %d, ocm: %d, sock_server:%d", sock_client, ocm, sock_server);
    t_list* pack = recv_packet(sock_client);
    
    if(pack == NULL) {
        log_error(logger, "Error recibiendo paquete");
        return;
    }
    
    if(ocm == MODULE_CPU_DISPATCH) {
        log_warning(logger, "Procesando solicitud de CPU");
        //cpu_work(pack);
        list_destroy_and_destroy_elements(pack, free_element);
        return;
    }
}

void instance_signal_handler(){
    if(signal(SIGINT, catch_handler_termination) == SIG_ERR){
        log_error(logger, "Problema seteando un handler para señales SIGINT");
    }
    if(signal(SIGTERM, catch_handler_termination) == SIG_ERR){
        log_error(logger, "Problema seteando un handler para señales SIGTERM");
    }
    if(signal(SIGABRT, catch_handler_termination) == SIG_ERR){
        log_error(logger, "Problema seteando un handler para señales SIGABORT");
    }
}

