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
}

int main( void ) {
    test_failed_init();
    test_mm();
    printf( "Passed: %u\nFailed: %u\n", pass_counter, fail_counter );
    return ( fail_counter == 0 ) ? 0 : 1;
}
