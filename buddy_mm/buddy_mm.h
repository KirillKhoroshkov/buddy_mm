#include <stdbool.h>
#include <malloc.h>
#include <memory.h>

#ifndef BUDDY_MM_BUDDY_MM_H

struct buddy_mm_t;
typedef struct buddy_mm_t buddy_mm_t;
struct block_header_t;
typedef struct block_header_t block_header_t;

extern buddy_mm_t *buddy_mm_init( size_t degree );
extern void buddy_mm_uninit( buddy_mm_t *mm );

extern void *buddymm_malloc( buddy_mm_t *mm, size_t size );
extern void buddymm_free( buddy_mm_t *mm, void *ptr );

#define BUDDY_MM_BUDDY_MM_H

#endif //BUDDY_MM_BUDDY_MM_H
