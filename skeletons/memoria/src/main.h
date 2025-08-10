#include "base.h"

#include "signal.h"
volatile sig_atomic_t status=0;

// Declaraciones de funciones
void* handle_kernel_request(void* params);
void* go_loop_net(void* params);
void packet_callback(void* params);
void* attend_multiple_clients(void* params);
void instance_signal_handler(void);
static void catch_handler_termination(int sign)
{
    log_warning(logger, "Handle Termination");
    close_all_sockets();
    exit(EXIT_SUCCESS);
}

pthread_mutex_t lock;