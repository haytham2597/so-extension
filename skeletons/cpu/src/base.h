#ifndef CPU_BASE_H
#define CPU_BASE_H


#include <inc/libs.h>
#include <inc/common.h>
#include <pthread.h>
#include <stdlib.h>
#include "exts/array_ext.h"
#include "exts/string_ext.h"
#include "modules/sockets/network.h"
#include "utils/enums.h"
#include "utils/structs.h"
#ifndef BASE_MMU_H
#include "mmu/base_mmu.h"
#endif

#include "libs/event_args.h"

//#include "libs/config.h"

#ifndef BASE_INSTRUCTIONS_H
#include "instructions_cycle/base_instructions.h"
#endif
#include "instructions_cycle/fetch.h"
#include "instructions_cycle/decode.h"
#include "instructions_cycle/execute.h"

#include "commons/collections/dictionary.h"
#include "modules/sockets/network.h"
#include "modules/managers/threads_manager.h"

void packet_callback(void* params);

#endif