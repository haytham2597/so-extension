#include "base.h"

#define MAXCLIENTFILESYSTEM 15
#include "signal.h"

volatile sig_atomic_t status=0;
static void catch_handler_termination(int sign){
    log_warning(logger, "Handle termination");
    close(sock_server);
    //close_all_sockets();
    exit(EXIT_SUCCESS);
}

void instance_signal_handler();
void* go_loop_net(void* params);
void* attend_multiple_clients(void* params);
void packet_callback(void* params);