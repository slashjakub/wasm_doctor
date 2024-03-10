#ifndef WASM_DOCTOR
#define WASM_DOCTOR

#include <stdint.h>

struct valid_block {
        uint32_t addr;
        uint32_t size;
};

void register_store(uint32_t address, uint32_t size);
uint8_t validate_load(uint32_t address);

#endif /* WASM_DOCTOR */
