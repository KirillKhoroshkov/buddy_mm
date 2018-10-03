#include "commands.h"

#ifndef BUDDY_MM_MESSAGES_H
#define BUDDY_MM_MESSAGES_H

#define program_name "mm"
#define start_message "Memory manager \"The buddy system\". For help, type \"" help_command "\"."
#define help_message "Commands:\n\
1) " init_command " <degree> \n\
2) " allocate_command " <size>\n\
3) " reallocate_command " <address> <new_size>\n\
4) " free_command " <address>\n\
5) " set_command " <address> <index> <new_value>\n\
6) " get_command " <address> <index>\n\
7) " save_command " <file_path> [-r]\n\
8) " open_command " <file_path>\n\
9) " read_command " <file_path>"

#define unknown_command_error_message "Unknown command"
#define invalid_args_error_message "Invalid arguments"
#define not_configured_error_message "Memory space is not configured"
#define not_enough_space_error_message "Not enough space"
#define invalid_configuration_error_message "Invalid configuration"
#define out_of_memory_error_message "Out of memory"
#define out_of_allocated_memory_error_message "Out of allocated memory"
#define invalid_file_path_error_message "Invalid file path"
#define invalid_file_error_message "Invalid file"
#define file_already_consist_error_message "File already consist"
#define reading_while_reading_error_message "Сan not execute " read_command " command when reading commands from a file"
#define unknown_error_message "Unknown error"

#endif //BUDDY_MM_MESSAGES_H
