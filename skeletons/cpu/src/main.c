#include "main.h"

int main(int argc, char *argv[])
{
    create_log("cpu", LOG_LEVEL_TRACE);
    log_info(logger, "%s","Hola mundo");
}