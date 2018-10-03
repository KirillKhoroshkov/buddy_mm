#include "buddy_mm.h"

struct block_header_t {
    size_t size;
    struct block_header_t *previous_block;
    struct block_header_t *next_block;
};

struct buddy_mm_t {
    block_header_t **avail_blocks;
    void *space; // for uninit
    size_t size_of_space;
    size_t header_and_align_size;
};

size_t calculate_power_of_two (size_t degree) {
    size_t rez = 1;
    for (int i = 0; i < degree; i++) {
        rez *= 2;
    }
    return rez;
}

size_t ceil_to_multiple(size_t size, size_t max_align) {
    if ((size % max_align) == 0) {
        return size;
    } else {
        size_t rez = size / max_align;
        rez += 1;
        return rez * max_align;
    }
}

buddy_mm_t *buddy_mm_init(size_t degree) {
    size_t size_of_space = calculate_power_of_two(degree);
    size_t max_align = _Alignof(max_align_t);
    size_t min_size_of_block = ceil_to_multiple(sizeof(block_header_t), max_align);
    if (size_of_space < min_size_of_block) {
        return NULL;
    }
    void *space = malloc(size_of_space);
    if (space == NULL) {
        return NULL;
    }
    block_header_t **avail_blocks;
    size_t current_size = size_of_space;
    size_t number_of_lists_of_available = 0;
    while (current_size > min_size_of_block) {
        number_of_lists_of_available += 1;
        current_size /= 2;
    }
    avail_blocks = malloc(sizeof(block_header_t*) * number_of_lists_of_available);
    if (avail_blocks == NULL) {
        return NULL;
    }
    for (int i = 1; i < number_of_lists_of_available; i++) {
        avail_blocks[i] = NULL;
    }
    block_header_t *first_block = space;
    first_block->previous_block = NULL;
    first_block->next_block = NULL;
    first_block->size = 0;
    avail_blocks[0] = first_block;
    buddy_mm_t *buddy_mm = malloc(sizeof(buddy_mm_t));
    if (buddy_mm == NULL) {
        return NULL;
    }
    buddy_mm->space = space;
    buddy_mm->header_and_align_size = min_size_of_block;
    buddy_mm->size_of_space = size_of_space;
    buddy_mm->avail_blocks = avail_blocks;
    return buddy_mm;
}

size_t get_size_by_index(size_t index, size_t size) {
    while (index > 0) {
        size /= 2;
        index--;
    }
    return size;
}

void split_block(buddy_mm_t *buddy_mm, size_t from, size_t to) {
    size_t current_index = from;
    while (current_index < to) {
        void *current_place = buddy_mm->avail_blocks[current_index];
        buddy_mm->avail_blocks[current_index] = buddy_mm->avail_blocks[current_index]->next_block;
        block_header_t *right = (block_header_t*) current_place + get_size_by_index(buddy_mm->size_of_space, current_index -1 );
        right->next_block = buddy_mm->avail_blocks[current_index + 1];
        block_header_t *left = (block_header_t*) current_place;
        left->next_block = right;
        buddy_mm->avail_blocks[current_index + 1] = left;
        current_index++;
    }
}

void *alloc_block(buddy_mm_t *buddy_mm, size_t index) {
    size_t current_index = index;
    while (buddy_mm->avail_blocks[current_index] == NULL) {
        if (current_index == 0) {
            return NULL;
        } else {
            current_index--;
        }
    }
    block_header_t *appropriate_block;
    if (current_index != index) {
        split_block(buddy_mm, current_index, index);
    }
    appropriate_block = buddy_mm->avail_blocks[index];
    buddy_mm->avail_blocks[index] = buddy_mm->avail_blocks[index]->next_block;
    return appropriate_block;
}

void *buddymm_malloc(buddy_mm_t *buddy_mm, size_t required_size) {
    size_t max_align = _Alignof(max_align_t);
    size_t align_size = ceil_to_multiple(required_size, max_align);
    size_t size = ceil_to_multiple(sizeof(size_t), max_align) + align_size;
    size_t appropriate_size = buddy_mm->size_of_space;
    if (size > appropriate_size) {
        return NULL;
    }
    size_t index = 0;
    while (true) {
        appropriate_size /= 2;
        if (appropriate_size > buddy_mm->header_and_align_size && appropriate_size > size) {
            index++;
        } else {
            break;
        }
    }
    void *appropriate_place = alloc_block(buddy_mm, index);
    if (appropriate_place == NULL) {
        return NULL;
    } else {
        return appropriate_place + ceil_to_multiple(sizeof(size_t), max_align);
    }
}