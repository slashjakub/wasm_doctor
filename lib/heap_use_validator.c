#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_reporter.h"
#include "heap_use_validator.h"
#include "linear_stack_validator.h"

void
register_global_data(struct heap_use_validator *validator, size_t address, size_t size_in_bytes)
{
        validator->global_blocks = (struct global_block *)realloc(
                validator->global_blocks, ++validator->global_blocks_size * sizeof(*validator->global_blocks));
        validator->global_blocks[validator->global_blocks_size - 1].block_start = address;
        validator->global_blocks[validator->global_blocks_size - 1].size_in_bytes = size_in_bytes;
}

void
register_malloc(struct heap_use_validator *validator, size_t block_start, size_t size_in_bytes)
{
        validator->blocks = (struct allocated_block *)realloc(validator->blocks,
                                                              ++validator->blocks_size * sizeof(*validator->blocks));
        validator->blocks[validator->blocks_size - 1].block_start = block_start;
        validator->blocks[validator->blocks_size - 1].size_in_bytes = size_in_bytes;
        validator->blocks[validator->blocks_size - 1].freed = false;
        validator->blocks[validator->blocks_size - 1].allocated_in_function_name =
                (char *)malloc(strlen(validator->reporter->state
                                              ->function_names[validator->reporter->state->function_names_size - 1]) +
                               1);

        strncpy(validator->blocks[validator->blocks_size - 1].allocated_in_function_name,
                validator->reporter->state->function_names[validator->reporter->state->function_names_size - 1],
                strlen(validator->reporter->state
                               ->function_names[validator->reporter->state->function_names_size - 1]) +
                        1);
}

void
register_free(struct heap_use_validator *validator, size_t block_start)
{
        for (size_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].block_start == block_start) {
                        if (validator->blocks[i].freed == false) {
                                validator->blocks[i].freed = true;
                                return;
                        } else {
                                add_double_free(validator->reporter, block_start);
                                return;
                        }
                }
        }

        add_invalid_free(validator->reporter, block_start);
}

void
check_use_after_free(struct heap_use_validator *validator, size_t address, uint8_t size_in_bytes)
{
        bool found_on_heap = false;
        bool found_allocated = false;

        for (size_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].block_start <= address + size_in_bytes &&
                    address < validator->blocks[i].block_start + validator->blocks[i].size_in_bytes) {
                        found_on_heap = true;

                        if (validator->blocks[i].freed == false) {
                                found_allocated = true;
                        }
                }
        }

        if (found_on_heap && !found_allocated) {
                add_use_after_free(validator->reporter, address);
        }
}

static bool
is_read_write_valid(struct heap_use_validator *validator, size_t address, uint8_t size_in_bytes)
{
        if (validator->linear_stack_validator->linear_stack_pointer - RED_ZONE_SIZE <= address &&
            address + size_in_bytes <= validator->linear_stack_validator->linear_stack_pointer_base) {
                return true;
        }

        for (size_t i = 0; i < validator->global_blocks_size; ++i) {
                if (validator->global_blocks[i].block_start <= address &&
                    address + size_in_bytes <=
                            validator->global_blocks[i].block_start + validator->global_blocks[i].size_in_bytes) {
                        return true;
                }
        }

        for (size_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].block_start <= address &&
                    address + size_in_bytes <= validator->blocks[i].block_start + validator->blocks[i].size_in_bytes) {
                        return true;
                }
        }

        return false;
}

void
check_read_validity(struct heap_use_validator *validator, size_t address, uint8_t size_in_bytes)
{
        if (!is_read_write_valid(validator, address, size_in_bytes)) {
                add_invalid_read(validator->reporter, address);
        }
}

void
check_write_validity(struct heap_use_validator *validator, size_t address, uint8_t size_in_bytes)
{
        if (!is_read_write_valid(validator, address, size_in_bytes)) {
                add_invalid_write(validator->reporter, address);
        }
}

void
heap_use_validator_init(struct heap_use_validator *validator, struct linear_stack_validator *linear_stack_validator,
                        struct error_reporter *reporter)
{
        validator->blocks_size = 0;
        validator->blocks = NULL;

        validator->global_blocks_size = 0;
        validator->global_blocks = NULL;

        validator->linear_stack_validator = linear_stack_validator;
        validator->reporter = reporter;
}

void
heap_use_validator_exit(struct heap_use_validator *validator)
{
        for (size_t i = 0; i < validator->blocks_size; ++i) {
                if (validator->blocks[i].freed == false) {

                        add_memory_leak(validator->reporter, validator->blocks[i].block_start,
                                        validator->blocks[i].size_in_bytes,
                                        validator->blocks[i].allocated_in_function_name);
                }

                free(validator->blocks[i].allocated_in_function_name);
        }

        free(validator->blocks);

        free(validator->global_blocks);
}
