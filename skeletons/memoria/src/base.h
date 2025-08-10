#ifndef MEMORY_BASE_H
#define MEMORY_BASE_H

#include "inc/common.h"
#include "modules/sockets/network.h"
#include "modules/managers/socket_manager.h"
#include "modules/managers/threads_manager.h"
#include "modules/managers/pseudocode_manager.h"

#ifndef MEMORY_OCM_CPU_WORK_H
#include "ocm_worker/cpu_work.h"
#endif
#ifndef MEMORY_OCM_KERNEL_WORK_H
#include "ocm_worker/kernel_work.h"
#endif

#ifndef SYSTEM_MEMORY_H
#include "space_memory/system_memory.h"
#endif
#ifndef USER_MEMORY_H
#include "space_memory/user_memory.h"
#endif

#include "inc/libs.h"
#include "exts/array_ext.h"
#include "exts/list_ext.h"
#define MAXCLIENTMEMORY 15



#endif