#include "inc/common.h"
#include "inc/libs.h"
#include "readline/readline.h"
#include "modules/sockets/network.h"
#include "exts/common_ext.h"
#include "exts/array_ext.h"
#include "commons/collections/queue.h"

#ifndef KERNEL_BASE_H
#include "base.h"
#endif

#include <signal.h>

//void* console();

volatile sig_atomic_t status=0;
static void catch_handler_termination(int sign){
    log_warning(logger, "Handle termination");
    close_all_sockets();
    exit(EXIT_SUCCESS);
}
void instance_create_process();
void instance_signal_handler();
void free_up_sockets();
void* connect_to_server(void* params);