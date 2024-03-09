#include <stdint.h>

#include "wasm_doctor.h"

uint64_t idx;
struct valid_block valid_blocks[100000] = {0};

void
register_store(uintptr_t address, uint64_t size)
{
        struct valid_block new_valid_block = {
                .addr = address,
                .size = size,
        };

        valid_blocks[idx++] = new_valid_block;
}

uint8_t
validate_load(uintptr_t address)
{
        for (uint64_t i = 0; i < idx; ++i) {
                if (valid_blocks[i].addr <= address &&
                    address <=
                            valid_blocks[i].addr +
                                    sizeof(uintptr_t) * valid_blocks[i].size) {
                        return 1;
                }
        }

        return 0;
}
