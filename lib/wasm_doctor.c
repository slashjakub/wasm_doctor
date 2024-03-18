#include <stdint.h>

#include "wasm_doctor.h"

uint32_t idx = 0;
struct valid_block valid_blocks[100000] = {0};

uint32_t valid_stack = 0;

void
update_sp(uint32_t sp)
{
        valid_stack = sp;
}

void
register_store(uint32_t address, uint32_t size)
{
        struct valid_block new_valid_block = {
                .addr = address,
                .size = size,
        };

        valid_blocks[idx++] = new_valid_block;
}

uint8_t
validate_load(uint32_t address)
{
        if (address <= valid_stack) {
                return 1;
        }

        for (uint32_t i = 0; i < idx; ++i) {
                if (valid_blocks[i].addr <= address &&
                    address <= valid_blocks[i].addr + valid_blocks[i].size) {
                        return 1;
                }
        }

        return 0;
}
