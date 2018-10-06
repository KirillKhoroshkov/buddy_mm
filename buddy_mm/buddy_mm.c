#include "buddy_mm.h"

struct block_header_t {
    size_t size;
    block_header_t *previous_block;
    block_header_t *next_block;
};

struct buddy_mm_t {
    block_header_t **avail_blocks;
    void *space; // for uninit
    size_t size_of_space;
    size_t header_and_align_size;
};

void *get_shift(buddy_mm_t *mm) {
    return mm->space;
}

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
    while (current_size >= min_size_of_block) {
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

void buddy_mm_uninit( buddy_mm_t *mm ) {
    free(mm->space);
    free(mm->avail_blocks);
    free(mm);
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
        block_header_t *right = (block_header_t*) (current_place + get_size_by_index(current_index + 1, buddy_mm->size_of_space));
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

void *buddy_mm_malloc(buddy_mm_t *buddy_mm, size_t required_size) {
    size_t max_align = _Alignof(max_align_t);
    size_t align_size = ceil_to_multiple(required_size, max_align);
    size_t needed_size = ceil_to_multiple(sizeof(size_t), max_align) + align_size;
    size_t appropriate_size = buddy_mm->size_of_space;
    if (needed_size > appropriate_size) {
        return NULL;
    }
    size_t index = 0;
    while (true) {
        if (!((appropriate_size >= (buddy_mm->header_and_align_size)) && (appropriate_size >= needed_size))) {
            index--;
            break;
        }
        index++;
        appropriate_size /= 2;
    }
    void *appropriate_place = alloc_block(buddy_mm, index);
    if (appropriate_place == NULL) {
        return NULL;
    } else {
        *((size_t*) appropriate_place) = index;
        return appropriate_place + ceil_to_multiple(sizeof(size_t), max_align);
    }
}

void *get_buddy_address(void *address, size_t size, void *start_address) {
    void *relative_address = (void *) (address - start_address); //тут возможен косяк
    bool is_buddy_on_right = (bool) ((((size_t)relative_address + size) / size) % 2);
    if (is_buddy_on_right) {
        return address + size;
    } else {
        return address - size;
    }
}

void merge_first_with_buddy (buddy_mm_t *mm, size_t block_size) {
    if (block_size == 0) {
        return;
    }
    block_header_t *first_block = mm->avail_blocks[block_size];
    size_t size = get_size_by_index(block_size, mm->size_of_space);
    void *buddy_address = get_buddy_address(first_block, size, mm->space);
    block_header_t *current_address = first_block->next_block;
    while (current_address != NULL) {
        if (current_address == buddy_address) {
            block_header_t *prev = current_address->previous_block;
            prev->next_block = current_address->next_block;
            block_header_t *next = current_address->next_block;
            if (next != NULL) {
                next->previous_block = prev;
            }
            mm->avail_blocks[block_size] = first_block->next_block;
            if (mm->avail_blocks[block_size] != NULL) {
                mm->avail_blocks[block_size]->previous_block = NULL;
            }
            block_header_t *new_block;
            if (first_block < current_address) {
                new_block = first_block;
            } else {
                new_block = current_address;
            }
            new_block->next_block = mm->avail_blocks[block_size - 1];
            if (mm->avail_blocks[block_size - 1] != NULL) {
                mm->avail_blocks[block_size - 1]->previous_block = new_block;
            }
            mm->avail_blocks[block_size - 1] = new_block;
            merge_first_with_buddy(mm, block_size - 1);
            return;
        } else {
            current_address = current_address->next_block;
        }
    }
}

void buddy_mm_free(buddy_mm_t *mm, void *ptr) {
    size_t max_align = _Alignof(max_align_t);
    size_t header_size = ceil_to_multiple(sizeof(size_t), max_align);
    void *start_of_block = ptr - header_size;
    size_t block_size = *((size_t*) start_of_block);
    block_header_t *header = start_of_block;
    header->size = 0;
    header->previous_block = NULL;
    header->next_block = mm->avail_blocks[block_size];
    if (mm->avail_blocks[block_size] != NULL) {
        mm->avail_blocks[block_size]->previous_block = header;
    }
    mm->avail_blocks[block_size] = header;
    merge_first_with_buddy(mm, block_size);
}