#include <malloc.h>
#include <memory.h>
#include "messages.h"

#ifndef BUDDY_MM_ERRORS_H
#define BUDDY_MM_ERRORS_H

#define unknown_command_error 2
#define invalid_args_error 3
#define not_configured_error 4
#define not_enough_space_error 5
#define invalid_configuration_error 6
#define out_of_memory_error 7
#define out_of_allocated_memory_error 8
#define invalid_file_path_error 9
#define invalid_file_error 10
#define file_already_consist_error 11
#define reading_while_reading_error 12

typedef struct {
    int code;
    char *message;
} error_info_t;

error_info_t *get_error_info_by_code(int code);

void free_error_info(error_info_t *error_info);

#endif //BUDDY_MM_ERRORS_H
