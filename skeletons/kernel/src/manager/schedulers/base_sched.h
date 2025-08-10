#ifndef KERNEL_SCHEDULERS_BASE_SCHEDULER_H
#define KERNEL_SCHEDULERS_BASE_SCHEDULER_H

#ifndef MODULES_THREADS_MANAGER_H
#include "modules/managers/threads_manager.h"
#endif

config_kernel ck;

#include "../pcb_tcb.h"

//HERE
int scheduler_stop = 0;
scheduler_algorithm type_sched;
int sock_cpu_interrupt=0, sock_cpu_dispatch=0;

#endif