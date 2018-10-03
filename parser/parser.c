#include "parser.h"

parser_response_t *parse(char *raw_text) {
    char entered_command[max_string_length];
    parser_response_t *response = malloc(sizeof(parser_response_t));
    int args_number = sscanf(raw_text, "%s", entered_command);
    if (args_number != 1) { // if string is empty
        response->is_error = true;
        response->info = get_error_info_by_code(invalid_args_error);
        return response;
    }
    char *args = raw_text + strlen(entered_command);
    if (strcmp(entered_command, get_command) == 0) {
        void *address;
        size_t index;
        args_number = sscanf(args, "%p %zu", &address, &index);
        if (args_number != 2) {
            response->is_error = true;
            response->info = get_error_info_by_code(invalid_args_error);
        } else {
            response->is_error = false;
            command_info_t *command_info = malloc(sizeof(command_info_t));
            get_parameters_t *parameters = malloc(sizeof(get_parameters_t));
            parameters->address = address;
            parameters->index = index;
            command_info->command_code = get_command_code;
            command_info->parameters = parameters;
            response->info = command_info;
        }
    } else if (strcmp(entered_command, set_command) == 0) {
        void *address;
        size_t index;
        char new_value;
        args_number = sscanf(args, "%p %zu %c", &address, &index, &new_value);
        if (args_number != 3) {
            response->is_error = true;
            response->info = get_error_info_by_code(invalid_args_error);
        } else {
            response->is_error = false;
            command_info_t *command_info = malloc(sizeof(command_info_t));
            set_parameters_t *parameters = malloc(sizeof(set_parameters_t));
            parameters->address = address;
            parameters->index = index;
            parameters->new_value = new_value;
            command_info->command_code = set_command_code;
            command_info->parameters = parameters;
            response->info = command_info;
        }
    } else if (strcmp(entered_command, reallocate_command) == 0) {
        void *address;
        size_t new_size;
        args_number = sscanf(args, "%p %zu", &address, &new_size);
        if (args_number != 2) {
            response->is_error = true;
            response->info = get_error_info_by_code(invalid_args_error);
        } else {
            response->is_error = false;
            command_info_t *command_info = malloc(sizeof(command_info_t));
            reallocate_parameters_t *parameters = malloc(sizeof(reallocate_parameters_t));
            parameters->address = address;
            parameters->new_size = new_size;
            command_info->command_code = reallocate_command_code;
            command_info->parameters = parameters;
            response->info = command_info;
        }
    } else if (strcmp(entered_command, allocate_command) == 0) {
        size_t size;
        args_number = sscanf(args, "%zu", &size);
        if (args_number != 1) {
            response->is_error = true;
            response->info = get_error_info_by_code(invalid_args_error);
        } else {
            response->is_error = false;
            command_info_t *command_info = malloc(sizeof(command_info_t));
            allocate_parameters_t *parameters = malloc(sizeof(allocate_parameters_t));
            parameters->size = size;
            command_info->command_code = allocate_command_code;
            command_info->parameters = parameters;
            response->info = command_info;
        }
    } else if (strcmp(entered_command, free_command) == 0) {
        void *address;
        args_number = sscanf(args, "%p", &address);
        if (args_number != 1) {
            response->is_error = true;
            response->info = get_error_info_by_code(invalid_args_error);
        } else {
            response->is_error = false;
            command_info_t *command_info = malloc(sizeof(command_info_t));
            free_parameters_t *parameters = malloc(sizeof(free_parameters_t));
            parameters->address = address;
            command_info->command_code = free_command_code;
            command_info->parameters = parameters;
            response->info = command_info;
        }
    } else if (strcmp(entered_command, print_command) == 0) {
        response->is_error = false;
        command_info_t *command_info = malloc(sizeof(command_info_t));
        command_info->command_code = print_command_code;
        command_info->parameters = NULL;
        response->info = command_info;
    } else if (strcmp(entered_command, save_command) == 0) {
        char *file_path;
        file_path = malloc(sizeof(char) * (max_string_length));
        file_path[0] = '\0';
        args_number = sscanf(args, "%s", file_path);
        if (args_number != 1) {
            response->is_error = true;
            response->info = get_error_info_by_code(invalid_args_error);
        } else {
            bool rewrite_it;
            char *flag_pointer = strstr((args + strlen(file_path)), rewrite_flag);
            if (flag_pointer != NULL) {
                rewrite_it = true;
            } else {
                rewrite_it = false;
            }
            response->is_error = false;
            command_info_t *command_info = malloc(sizeof(command_info_t));
            save_parameters_t *parameters = malloc(sizeof(save_parameters_t));
            parameters->file_path = file_path;
            parameters->rewrite_it = rewrite_it;
            command_info->command_code = save_command_code;
            command_info->parameters = parameters;
            response->info = command_info;
        }
    } else if (strcmp(entered_command, open_command) == 0) {
        char *file_path;
        file_path = malloc(sizeof(char) * (max_string_length));
        file_path[0] = '\0';
        args_number = sscanf(args, "%s", file_path);
        if (args_number != 1) {
            response->is_error = true;
            response->info = get_error_info_by_code(invalid_args_error);
        } else {
            response->is_error = false;
            command_info_t *command_info = malloc(sizeof(command_info_t));
            open_parameters_t *parameters = malloc(sizeof(open_parameters_t));
            parameters->file_path = file_path;
            command_info->command_code = open_command_code;
            command_info->parameters = parameters;
            response->info = command_info;
        }
    } else if (strcmp(entered_command, init_command) == 0) {
        size_t degree;
        if (sscanf(args, "%zu", &degree) == 0) {
            degree = default_space_size;
        }
        response->is_error = false;
        command_info_t *command_info = malloc(sizeof(command_info_t));
        init_parameters_t *parameters = malloc(sizeof(init_parameters_t));
        parameters->degree = degree;
        command_info->command_code = init_command_code;
        command_info->parameters = parameters;
        response->info = command_info;
    } else if (strcmp(entered_command, help_command) == 0) {
        response->is_error = false;
        command_info_t *command_info = malloc(sizeof(command_info_t));
        command_info->command_code = help_command_code;
        command_info->parameters = NULL;
        response->info = command_info;
    } else if (strcmp(entered_command, read_command) == 0) {
        char *file_path;
        file_path = malloc(sizeof(char) * (max_string_length));
        file_path[0] = '\0';
        args_number = sscanf(args, "%s", file_path);
        if (args_number != 1) {
            response->is_error = true;
            response->info = get_error_info_by_code(invalid_args_error);
        } else {
            response->is_error = false;
            command_info_t *command_info = malloc(sizeof(command_info_t));
            read_parameters_t *parameters = malloc(sizeof(read_parameters_t));
            parameters->file_path = file_path;
            command_info->command_code = read_command_code;
            command_info->parameters = parameters;
            response->info = command_info;
        }
    } else if (strcmp(entered_command, exit_command) == 0) {
        response->is_error = false;
        command_info_t *command_info = malloc(sizeof(command_info_t));
        command_info->command_code = exit_command_code;
        command_info->parameters = NULL;
        response->info = command_info;
    } else {
        response->is_error = true;
        response->info = get_error_info_by_code(unknown_command_error);
    }
    return response;
}