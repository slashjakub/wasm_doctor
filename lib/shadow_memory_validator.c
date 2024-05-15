#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "error_reporter.h"
#include "shadow_memory_validator.h"

#define BYTES_PER_WORD sizeof(word_t)

void
validate(struct shadow_memory_validator *validator, uint64_t bit_idx)
{
        /* printf("%u %lu %lu\n", bit_idx, BYTES_PER_WORD, */
        /* bit_idx / BYTES_PER_WORD); */
        validator->words[bit_idx / BYTES_PER_WORD] |= 1 << bit_idx % BYTES_PER_WORD;
}

void
invalidate(struct shadow_memory_validator *validator, uint64_t bit_idx)
{
        validator->words[bit_idx / BYTES_PER_WORD] &= ~(1 << bit_idx % BYTES_PER_WORD);
}

void
validate_region(struct shadow_memory_validator *validator, size_t address, size_t size_in_bytes)
{
        for (size_t i = address * 8; i < (address + size_in_bytes) * 8 - 1; ++i) {
                validate(validator, i);
        }
}

void
invalidate_region(struct shadow_memory_validator *validator, size_t address, size_t size_in_bytes)
{

        for (size_t i = address * 8; i < (address + size_in_bytes) * 8 - 1; ++i) {
                invalidate(validator, i);
        }
}

void
check_region_access(struct shadow_memory_validator *validator, size_t address, size_t size_in_bytes)
{
        bool validity[size_in_bytes * 8];

        bool is_valid = true;
        for (size_t i = address * 8; i < (address + size_in_bytes) * 8 - 1; ++i) {
                if ((validator->words[i / BYTES_PER_WORD] & (1 << i % BYTES_PER_WORD)) == 0) {
                        is_valid = false;
                        validity[i - address * 8] = false;
                } else {
                        validity[i - address * 8] = true;
                }
        }

        if (!is_valid) {
                add_undefined_memory_use(validator->reporter, address, validity);
        }
}

void
shadow_memory_validator_init(struct shadow_memory_validator *validator, size_t mem_size,
                             struct error_reporter *reporter)
{
        validator->words = (word_t *)malloc(mem_size * 8);
        invalidate_region(validator, 0, mem_size);

        validator->reporter = reporter;
}

void
shadow_memory_validator_exit(struct shadow_memory_validator *validator)
{
        free(validator->words);
}
