#ifndef BASE_MMU_H
#define BASE_MMU_H
#include "semaphore.h"

#include "inc/common.h"
#ifndef INC_LIBS_H
#include "inc/libs.h"
#endif
#ifndef EXTS_COMMON_EXT
#include "exts/common_ext.h"
#endif

//t_tcb* current_tcb;
int sock_memory, sock_client_dispatch, sock_client_interrupt;
sem_t sem_context_received;
sem_t sem_tcb_ready;
sem_t sem_syscall_response;
sem_t sem_reschedule;
sem_t sem_interrup_current;
int syscall_response_status = ERROR;
int syscall_in_progress = 0;

int is_running=0, isOnFetch=0,stop=0;

#endif