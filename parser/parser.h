#include <stdbool.h>
#include <stddef.h>
#include <malloc.h>
#include <memory.h>
#include "../properties/errors.h"
#include "../properties/messages.h"
#include "../properties/settings.h"

#ifndef BUDDY_MM_PARSER_H
#define BUDDY_MM_PARSER_H

typedef struct {
    bool is_error;
    void *info; // error_info_t or command_info_t
} parser_response_t;

parser_response_t *parse(char *raw_text);

#endif //BUDDY_MM_PARSER_H
