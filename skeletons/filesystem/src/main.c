#include "main.h"

int main(int argc, char* argv[]) {
    load_config("filesystem.config");
    cf = load_config_filesystem();

    create_log("filesystem", cf.log_level);    
    instance_signal_handler();
    /*instance_bitmap();
    instance_block();*/
    
    sock_server = server_connection(cf.puerto_escucha);
    add_thread_by_enum(MODULE_FILESYSTEM);
    void* parameters = malloc(sizeof(int)*2);
    memcpy(parameters, &sock_server,sizeof(int));
    memcpy(parameters+sizeof(int), &cf.puerto_escucha,sizeof(int));
    pthread_create(
        get_pthread_by_enum(MODULE_FILESYSTEM), 
        NULL, 
        attend_multiple_clients, 
        parameters
    );
    pthread_join(*get_pthread_by_enum(MODULE_FILESYSTEM), NULL);
    return 0;
}

void* attend_multiple_clients(void* params)
{
    int sock=0, port=0;
    memcpy(&sock, params, sizeof(int));
    memcpy(&port, params+sizeof(int), sizeof(int));
    log_debug(logger, "Sock: %d, Port: %d",sock, port);
    for(;;){
        log_debug(logger, "Esperando cliente en socket %d, Puerto: %d", sock, port);
        int sock_client = wait_client(sock, port);
        log_debug(logger, "Socket cliente: %d", sock_client);
        if(handshake(sock_client, 1) != 0)
        {
            log_error(logger, "No pudo hacer handshake con socket %d error %d [%s:%d]", sock_client, errno, __func__, __LINE__);
            continue;
        }
        t_list* l = recv_operation_packet(sock_client);
        op_code_module ocm;
        memcpy(&ocm, list_get(l, 0), sizeof(op_code_module));

        //Deberia ser siempre memoria y al igual que la memoria con kernel, se inmola
        if(ocm == MODULE_MEMORY){
            log_debug(logger, "Es memoria");
            t_list* li = recv_operation_packet(sock_client);
            log_debug(logger, "Recibi proveniente de memoria cant: %d", list_size(li));
            memory_work(li, sock_client);
            close(sock_client);
            list_destroy_and_destroy_elements(li, free_element);
            continue;
        }
        add_socket_structure_by_name_ocm_sock_server(ocm_to_string(ocm), ocm, sock_client, 1);
        
        //TODO: Improve this
        threads* th = add_thread(); 
        int thr_id = 0;
        memcpy(&thr_id, &th->threads_idx, sizeof(int));
        void* parameters = malloc(sizeof(int)*3);
        int offset =0;
        memcpy(parameters+offset, &sock_client, sizeof(int));
        offset +=sizeof(int);
        memcpy(parameters+offset, &ocm, sizeof(int));
        offset +=sizeof(int);
        memcpy(parameters+offset, &sock, sizeof(int));
        int res = pthread_create(th->pth, NULL, go_loop_net, parameters);
        res = pthread_detach(*th->pth);
        log_info(logger, "Res: %d", res);
        //free(parameters);
        list_destroy_and_destroy_elements(l, free_element);
    }
    return NULL;
}

void* go_loop_net(void* params){
    void* safe_copy = malloc(sizeof(int)*3);
    memcpy(safe_copy, params, sizeof(int)*3);
    int sock_client = 0;
    memcpy(&sock_client, safe_copy, sizeof(int));
    /*int* par = (int*)params;
    int sock_client = (int)par[0];*/
    loop_network(
        sock_client,
        &packet_callback,
        safe_copy
    );
    free(safe_copy);
    return NULL;
}

void packet_callback(void* params){
    //TODO: Improve, se supone que sólo recibe de Memoria y nada más. No haría falta determinar quién es. 
    int sock_client =0, sock_server=0;
    op_code_module ocm;
    int offset = 0;
    memcpy(&sock_client, params, sizeof(int));
    offset+=sizeof(int);
    memcpy(&ocm, params+offset, sizeof(int));
    offset+=sizeof(int);
    memcpy(&sock_server, params+offset, sizeof(int));
    t_list* pack = recv_packet(sock_client);
    log_info(logger, "Recibo paquete del modulo %s", ocm_to_string(ocm));
    char* str = buffer_to_string(list_get(pack, 0));
    log_info(logger, "Mensaje: %s", str);
    free(pack);
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