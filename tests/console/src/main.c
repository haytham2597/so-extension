#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../src/libs/logger.h"
#include "../../../src/exts/log_ext.h"
#include "commons/string.h"

int main(int argc, char* argv[]) 
{
    create_log("test", LOG_LEVEL_TRACE);
    log_info(logger, "Test log");
    log_with_fore_color(logger, 50, "%s", "Esto es un mensaje");
    log_with_fore_color(logger, 55, "%s", "Esto es otro mensaje");
    log_with_fore_color(logger, 60, "%s", "Esto es otro nuevo mensaje");
    log_with_fore_back_color(logger, 5,50, "Mensaje con fondo");
    log_violet(logger,"%s", "Mensaje copado");
    log_light_blue(logger,"%s", "Mensaje copado");
    log_light_green(logger,"%s", "Mensaje copado");
    log_pink(logger,"%s", "Mensaje copado");
    log_orange(logger,"%s", "Mensaje copado");
    log_white_background(logger,"%s", "White back");
    log_info(logger, "Esto es uno de catedra");
    log_trace(logger, "Esto es uno de catedra trace");
}