#include "../buddy_mm/buddy_mm.c" // с .c не пашет

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

static void test_failed_init(void) {
    buddy_mm_t *mm = buddy_mm_init(2);
    if (mm == NULL) {
        pass();
    } else {
        fail();
    }
}

static void test_mm(void) {
    buddy_mm_t *mm = buddy_mm_init(8);
    if (mm != NULL) {
        pass();
    } else {
        fail();
        return;
    }
    if (mm->size_of_space == 256) {
        pass();
    } else {
        fail();
    }
    if (mm->avail_blocks[1] == NULL) {
        pass();
    } else {
        fail();
    }
    if (mm->space == mm->avail_blocks[0]) {
        pass();
    } else {
        fail();
    }
    void *first = buddy_mm_malloc(mm, 256);
    if (first == NULL) {
        pass();
    } else {
        fail();
    }
    if (mm->avail_blocks[1] == NULL && mm->avail_blocks[0] != NULL) {
        pass();
    } else {
        fail();
    }
    size_t max_align = _Alignof(max_align_t);
    size_t align_size_header = ceil_to_multiple(sizeof(size_t), max_align);
    char *first_char = (char*) buddy_mm_malloc(mm, sizeof(char));
    if (first_char == (align_size_header + mm->space)) {
        pass();
    } else {
        fail();
    }
    char *second_char = (char*) buddy_mm_malloc(mm, sizeof(char));
    if (mm->avail_blocks[1] != NULL) {
        pass();
    } else {
        fail();
    }
    if (mm->avail_blocks[2] != NULL) {
        pass();
    } else {
        fail();
    }
    if (mm->avail_blocks[3] == NULL) {
        pass();
    } else {
        fail();
    }
    size_t first_block_size = get_size_by_index(*((size_t*) ((void*) first_char - align_size_header)), mm->size_of_space);
    if (second_char == (align_size_header + ((size_t) first_block_size) + mm->space)) {
        pass();
    } else {
        fail();
    }
    buddy_mm_free(mm, first_char);
    if (mm->avail_blocks[3] != NULL) {
        pass();
    } else {
        fail();
    }
    buddy_mm_free(mm, second_char);
    if (mm->avail_blocks[0] != NULL) {
        pass();
    } else {
        fail();
    }
    if (mm->avail_blocks[1] == NULL) {
        pass();
    } else {
        fail();
    }
    if (mm->avail_blocks[2] == NULL) {
        pass();
    } else {
        fail();
    }
    if (mm->avail_blocks[3] == NULL) {
        pass();
    } else {
        fail();
    }
}

int main( void ) {
    test_failed_init();
    test_mm();
    printf( "Passed: %u\nFailed: %u\n", pass_counter, fail_counter );
    return ( fail_counter == 0 ) ? 0 : 1;
}
