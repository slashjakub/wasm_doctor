#ifndef WASM_DOCTOR
#define WASM_DOCTOR

#include <stdint.h>

struct valid_block {
        uintptr_t addr;
        uint64_t size;
};

void register_store(uintptr_t address, uint64_t size);
uint8_t validate_load(uintptr_t address);

#endif /* WASM_DOCTOR */
