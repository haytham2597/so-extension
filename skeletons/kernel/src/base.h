#ifndef KERNEL_BASE_H
#define KERNEL_BASE_H

#define PRINT_CALL_FUNCTION

#include "utils/macros.h"

#ifndef _UNISTD_H
#include "unistd.h"
#endif

typedef int tid_t;
#include "semaphore.h"

#ifndef UTILS_STRUCTS_H
#include "utils/structs.h"
#endif

#ifndef EXTS_LIST_EXT
#include "exts/list_ext.h"
#endif
#ifndef KERNEL_MANAGER_SCHEDULER_H
#include "manager/scheduler.h"
#endif

#ifndef SYSCALLS_MUTEXS_H
#include "syscalls/mutexs.h"
#endif
#ifndef SYSCALLS_PROCESS_H
#include "syscalls/process.h"
#endif
#ifndef SYSCALLS_THREADS_H
#include "syscalls/threads.h"
#endif
#ifndef SYSCALLS_COMMON_H
#include "syscalls/common.h"
#endif
#include "modules/managers/threads_manager.h"
#include "modules/managers/socket_manager.h"

op_code_module itself_ocm;
char* file;
int process_size;

void packet_callback(void* params);

#endif