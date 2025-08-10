#ifndef ENUMS_EXT_H
#define ENUMS_EXT_H

#ifndef UTILS_STRUCTS_H
#include "../utils/structs.h"
int is_value_in_range(int enum_value, int from, int to){
    return enum_value >= from && to >= enum_value;
}
int is_response(int value){
    return is_value_in_range(value, REQUEST_EXECUTE_PROCESS, REQUEST_KNOW);
    /*if(value < REQUEST_EXECUTE_PROCESS)
        return 0;
    if(value > REQUEST_KNOW)
        return 0;
    return 1;*/
}

#endif