#include "../parser/parser.c"

static unsigned pass_counter = 0;
static unsigned fail_counter = 0;

static void pass() {
    pass_counter += 1;
}

static void fail_( const char *test_name, int lineno ) {
    fprintf( stderr, "Test %s failed at %d\n", test_name, lineno );
    fail_counter += 1;
}

#define fail()  fail_( __func__, __LINE__ )  /*точки с запятой в конце нет, ее поставят при использовании макроса! */

static void test_init( void ) {
    char *str = malloc(sizeof(char) * 24);
    strcpy(str, "start 32");
    parser_response_t *response = parse(str);
    free(str);
    if( (response->is_error) != false ) {
        fail();
    } else {
        pass();
    }
    if (((command_info_t*) response->info)->command_code != init_command_code) {
        fail();
    } else {
        pass();
    }
    if (((init_parameters_t*)((command_info_t*) response->info)->parameters)->size != 32) {
        fail();
    } else {
        pass();
    }
    free_command_info(((command_info_t*) response->info));
    free(response);
}

static void test_unknown_command(void) {
    char *str = malloc(sizeof(char) * 24);
    strcpy(str, "star 32");
    parser_response_t *response = parse(str);
    free(str);
    if( (response->is_error) != true ) {
        fail();
    } else {
        pass();
    }
    if (((error_info_t*) response->info)->code != unknown_command_error) {
        fail();
    } else {
        pass();
    }
    if (strcmp(((error_info_t*) response->info)->message, unknown_command_error_message) != 0) {
        fail();
    } else {
        pass();
    }
    free_error_info(((error_info_t*) response->info));
    free(response);
}

static void test_save( void ) {
    char *str = malloc(sizeof(char) * 24);
    strcpy(str, "save ddd -r");
    parser_response_t *response = parse(str);
    free(str);
    if( (response->is_error) != false ) {
        fail();
    } else {
        pass();
    }
    if (((command_info_t*) response->info)->command_code != save_command_code) {
        fail();
    } else {
        pass();
    }
    if (strcmp(((save_parameters_t*)((command_info_t*) response->info)->parameters)->file_path, "ddd") != 0) {
        fail();
    } else {
        pass();
    }
    if (!((save_parameters_t*)((command_info_t*) response->info)->parameters)->rewrite_it) {
        fail();
    } else {
        pass();
    }
    free_command_info(((command_info_t*) response->info));
    free(response);
}

static void test_set( void ) {
    char *str = malloc(sizeof(char) * 24);
    strcpy(str, "set 1000 24 c");
    parser_response_t *response = parse(str);
    free(str);
    if( (response->is_error) != false ) {
        fail();
    } else {
        pass();
    }
    if (((command_info_t*) response->info)->command_code != set_command_code) {
        fail();
    } else {
        pass();
    }
    if (((set_parameters_t*)((command_info_t*) response->info)->parameters)->address != (void *) 0x1000) {
        fail();
    } else {
        pass();
    }
    if (((set_parameters_t*)((command_info_t*) response->info)->parameters)->index != 24) {
        fail();
    } else {
        pass();
    }
    if (((set_parameters_t*)((command_info_t*) response->info)->parameters)->new_value != 'c') {
        fail();
    } else {
        pass();
    }
    free_command_info(((command_info_t*) response->info));
    free(response);
}

static void test_help( void ) {
    char *str = malloc(sizeof(char) * 24);
    strcpy(str, "help");
    parser_response_t *response = parse(str);
    free(str);
    if( (response->is_error) != false ) {
        fail();
    } else {
        pass();
    }
    if (((command_info_t*) response->info)->command_code != help_command_code) {
        fail();
    } else {
        pass();
    }
    if (((command_info_t*) response->info)->parameters != NULL) {
        fail();
    } else {
        pass();
    }
    free_command_info(((command_info_t*) response->info));
    free(response);
}

int main( void ) {
    test_init();
    test_unknown_command();
    test_save();
    test_set();
    test_help();
    printf( "Passed: %u\nFailed: %u\n", pass_counter, fail_counter );
    return ( fail_counter == 0 ) ? 0 : 1;
}
