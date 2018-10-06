#include <stdio.h>
#include "properties/settings.h"
#include "properties/messages.h"
#include "properties/errors.h"
#include "parser/parser.h"
#include "buddy_mm/buddy_mm.h"

int main( ) {
    printf(start_message "\n");
    FILE *stream = stdin;
    char raw_text[max_string_length];
    error_info_t *error_info = NULL;
    char *message = NULL;
    buddy_mm_t *buddy_mm;
    void *shift; // for the relative addresses
    while (true) {
        printf("%s> ", program_name);
        if (fgets(raw_text, max_string_length, stream) == NULL && stream != stdin) {
            fclose(stream);
            stream = stdin;
            fgets(raw_text, max_string_length, stream);
        }
        if (stream != stdin) {
            printf("%s\n", raw_text);
        }
        parser_response_t *parser_response = parse(raw_text);
        if (parser_response->is_error) {
            error_info = (error_info_t *) parser_response->info;
        } else {
            command_info_t *command_info = (command_info_t *) parser_response->info;
            int command_code = command_info->command_code;
            void *parameters = command_info->parameters;
            if (command_code == exit_command_code) {
                break;
            } else if (command_code == help_command_code) {
                message = malloc((strlen(help_message) + 1) * sizeof(char));
                strcpy(message, help_message);
            } else if (command_code == init_command_code) {
                init_parameters_t *init_parameters = (init_parameters_t *) parameters;
                size_t degree = init_parameters->degree;
                if (buddy_mm != NULL) {
                    buddy_mm_uninit(buddy_mm);
                }
                buddy_mm = buddy_mm_init(degree);
                shift = get_shift(buddy_mm);
                message = malloc(32);
                sprintf(message, "%p", shift);
            } else if (command_code == allocate_command_code) {
                allocate_parameters_t *allocate_parameters = (allocate_parameters_t *) parameters;
                size_t size = allocate_parameters->size;
                if (buddy_mm == NULL) {
                    error_info = get_error_info_by_code(not_configured_error);
                } else {
                    void *address = buddy_mm_malloc(buddy_mm, size * sizeof(char));
                    void *relative_address = (void *) (address - shift);
                    message = malloc(32);
                    sprintf(message, "%p", relative_address);
                }
            } else if (command_code == print_command_code) {
                /**/
            } else if (command_code == reallocate_command_code) {
                reallocate_parameters_t *reallocate_parameters = (reallocate_parameters_t *) parameters;
                void *address = reallocate_parameters->address;
                size_t new_size = reallocate_parameters->new_size;
                /**/
            } else if (command_code == save_command_code) {
                save_parameters_t *save_parameters = (save_parameters_t *) parameters;
                char *file_path = save_parameters->file_path;
                bool rewrite_it = save_parameters->rewrite_it;
                /**/
            } else if (command_code == open_command_code) {
                open_parameters_t *open_parameters = (open_parameters_t *) parameters;
                char *file_path = open_parameters->file_path;
                /**/
            } else if (command_code == read_command_code) {
                read_parameters_t *read_parameters = (read_parameters_t *) parameters;
                char *file_path = read_parameters->file_path;
                if (stream != stdin) {
                    error_info = get_error_info_by_code(reading_while_reading_error);
                } else {
                    stream = fopen(file_path, "r");
                    if (stream == NULL) {
                        error_info = get_error_info_by_code(invalid_file_path_error);
                        stream = stdin;
                    } else {
                        message = malloc((strlen(file_path) + 1) * sizeof(char));
                        strcpy(message, file_path);
                    }
                }
            } else if (command_code == free_command_code) {
                free_parameters_t *free_parameters = (free_parameters_t *) parameters;
                void *relative_address = free_parameters->address;
                if (buddy_mm == NULL) {
                    error_info = get_error_info_by_code(not_configured_error);
                } else {
                    void *address = (void *) ((size_t) relative_address + (size_t) shift);
                    buddy_mm_free(buddy_mm, address);
                    message = malloc(32);
                    sprintf(message, "%p", relative_address);
                }
            } else if (command_code == get_command_code) {
                if (buddy_mm == NULL) {
                    error_info = get_error_info_by_code(not_configured_error);
                } else {
                    get_parameters_t *get_parameters = (get_parameters_t *) parameters;
                    void *relative_address = get_parameters->address;
                    size_t index = get_parameters->index;
                    void *address = (void *) ((size_t) relative_address + (size_t) shift);
                    char *ch = (char *) address + index;
                    message = malloc(sizeof(char) * 2);
                    sprintf(message, "%c", *ch);
                }
            } else if (command_code == set_command_code) {
                if (buddy_mm == NULL) {
                    error_info = get_error_info_by_code(not_configured_error);
                } else {
                    set_parameters_t *set_parameters = (set_parameters_t *) parameters;
                    void *relative_address = set_parameters->address;
                    void *address = (void *) ((size_t) relative_address + (size_t) shift);
                    size_t index = set_parameters->index;
                    char new_value = set_parameters->new_value;
                    char *ch = (char *) address + index;
                    char old_value = *ch;
                    *ch = new_value;
                    message = malloc(sizeof(char) * 2);
                    sprintf(message, "%c", old_value);
                }
            }
            free_command_info(command_info);
        }
        if (error_info != NULL) {
            printf("%d, ", error_info->code);
            printf("%s\n", error_info->message);
            free_error_info(error_info);
            error_info = NULL;
        } else {
            printf("0, %s\n", message);
            free(message);
            message = NULL;
        }
        free(parser_response);
        raw_text[0] = '\0';
    }
}
