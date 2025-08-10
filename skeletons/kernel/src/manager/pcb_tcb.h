#ifndef KERNEL_MANAGER_PCB_TCB_H
#define KERNEL_MANAGER_PCB_TCB_H

#ifndef UTILS_STRUCTS_H
#include "utils/structs.h"
#endif
#ifndef EXTS_TEMPORAL_H
#include "exts/temporal_ext.h"
#endif
#include "commons/collections/queue.h"
/// @brief Lista de PCB
t_list* pcbs;
/// @brief Lista de TCB
t_list* tcbs;
//t_pcb* current_process=NULL;
t_tcb* current_tcb=NULL;
t_dictionary* tcb_dict=NULL;

#ifndef KERNEL_BASE_H
#include "../base.h"
#endif

//HERE

#endif