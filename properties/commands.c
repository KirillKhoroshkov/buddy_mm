#include <malloc.h>
#include "commands.h"

void free_command_info(command_info_t *command_info) {
    switch( command_info->command_code ) {
        case exit_command_code:
            break;
        case print_command_code:
            break;
        case init_command_code:
            free((init_parameters_t*) command_info->parameters);
            break;
        case allocate_command_code:
            free((allocate_parameters_t*) command_info->parameters);
            break;
        case reallocate_command_code:
            free((reallocate_parameters_t*) command_info->parameters);
            break;
        case save_command_code:
            free(((save_parameters_t*) command_info->parameters)->file_path);
            free((save_parameters_t*) command_info->parameters);
            break;
        case open_command_code:
            free(((open_parameters_t*) command_info->parameters)->file_path);
            free((open_parameters_t*) command_info->parameters);
            break;
        case help_command_code:
            break;
        case read_command_code:
            free(((read_parameters_t*) command_info->parameters)->file_path);
            free((read_parameters_t*) command_info->parameters);
            break;
        case free_command_code:
            free((free_parameters_t*) command_info->parameters);
            break;
        case get_command_code:
            free((get_parameters_t*) command_info->parameters);
            break;
        case set_command_code:
            free((set_parameters_t*) command_info->parameters);
            break;
    }
    free(command_info);
}