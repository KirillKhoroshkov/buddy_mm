#include <memory.h>
#include <stdbool.h>

#ifndef BUDDY_MM_COMMANDS_H
#define BUDDY_MM_COMMANDS_H

#define exit_command "exit"
#define exit_command_code 1

#define print_command "print"
#define print_command_code 2

#define init_command "start"
#define init_command_code 3
typedef struct {
    size_t degree;
} init_parameters_t;

#define allocate_command "allocate"
#define allocate_command_code 4
typedef struct {
    size_t size;
} allocate_parameters_t;

#define reallocate_command "reallocate"
#define reallocate_command_code 5
typedef struct {
    void *address;
    size_t new_size;
} reallocate_parameters_t;

#define save_command "save"
#define save_command_code 6
#define rewrite_flag "-r"
typedef struct {
    char *file_path;
    bool rewrite_it;
} save_parameters_t;

#define open_command "open"
#define open_command_code 7
typedef struct {
    char *file_path;
} open_parameters_t;

#define help_command "help"
#define help_command_code 8

#define read_command "read"
#define read_command_code 9
typedef struct {
    char *file_path;
} read_parameters_t;

#define free_command "free"
#define free_command_code 10
typedef struct {
    void *address;
} free_parameters_t;

#define get_command "get"
#define get_command_code 11
typedef struct {
    void *address;
    size_t index;
} get_parameters_t;

#define set_command "set"
#define set_command_code 12
typedef struct {
    void *address;
    size_t index;
    char new_value;
} set_parameters_t;

typedef struct {
    int command_code;
    void *parameters;
} command_info_t;

void free_command_info(command_info_t *command_info);

#endif //BUDDY_MM_COMMANDS_H