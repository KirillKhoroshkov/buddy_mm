#include "errors.h"

error_info_t *get_error_info_by_code(int code) {
    error_info_t *error_info;
    error_info = malloc(sizeof(error_info_t));
    (*error_info).code = code;
    switch (code) {
        case unknown_command_error:
            (*error_info).message = malloc((strlen(unknown_command_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, unknown_command_error_message);
            break;
        case invalid_args_error:
            (*error_info).message = malloc((strlen(invalid_args_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, invalid_args_error_message);
            break;
        case not_configured_error:
            (*error_info).message = malloc((strlen(not_configured_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, not_configured_error_message);
            break;
        case not_enough_space_error:
            (*error_info).message = malloc((strlen(not_enough_space_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, not_enough_space_error_message);
            break;
        case invalid_configuration_error:
            (*error_info).message = malloc((strlen(invalid_configuration_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, invalid_configuration_error_message);
            break;
        case out_of_memory_error:
            (*error_info).message = malloc((strlen(out_of_memory_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, out_of_memory_error_message);
            break;
        case out_of_allocated_memory_error:
            (*error_info).message = malloc((strlen(out_of_allocated_memory_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, out_of_allocated_memory_error_message);
            break;
        case invalid_file_path_error:
            (*error_info).message = malloc((strlen(invalid_file_path_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, invalid_file_path_error_message);
            break;
        case invalid_file_error:
            (*error_info).message = malloc((strlen(invalid_file_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, invalid_file_error_message);
            break;
        case file_already_consist_error:
            (*error_info).message = malloc((strlen(file_already_consist_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, file_already_consist_error_message);
            break;
        case reading_while_reading_error:
            (*error_info).message = malloc((strlen(reading_while_reading_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, reading_while_reading_error_message);
            break;
        default :
            (*error_info).message = malloc((strlen(unknown_error_message) + 1) * sizeof(char));
            strcpy((*error_info).message, unknown_error_message);
            break;
    }
    return error_info;
}

void free_error_info(error_info_t *error_info) {
    free((*error_info).message);
    free(error_info);
}