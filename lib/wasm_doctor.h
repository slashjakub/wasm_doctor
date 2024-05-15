#ifndef WASM_DOCTOR
#define WASM_DOCTOR

#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"
#include "heap_use_validator.h"
#include "linear_stack_validator.h"
#include "local_validator.h"
#include "shadow_memory_validator.h"
#include "wasm_state.h"
#include "zero_address_access_validator.h"

struct wasm_doctor {
        struct error_reporter reporter;
        struct wasm_state state;

        struct linear_stack_validator linear_stack_validator;
        struct shadow_memory_validator shadow_memory_validator;
        struct heap_use_validator heap_validator;
        struct local_validator local_validator;
        struct zero_address_access_validator zero_validator;
};

void doctor_set_linear_stack_pointer_base(size_t address);
void doctor_move_linear_stack_pointer(size_t address);
void doctor_global_data_validate(size_t address, size_t size_in_bytes);
void doctor_store(size_t address, uint8_t size_in_bytes);
void doctor_load(size_t address, uint8_t size_in_bytes);
void doctor_register_malloc(size_t block_start, size_t size_in_bytes);
void doctor_register_free(size_t block_start);
void doctor_local_set(uint32_t idx);
void doctor_local_get(uint32_t idx);
void doctor_frame_enter(uint32_t locals_size, char *function_name);
void doctor_frame_exit(void);
void doctor_init(struct wasm_doctor *wasm_doctor, uint32_t size_in_pages, bool report);
void doctor_reporter_exit(void);
void doctor_exit(bool exit_reporter);

#endif /* WASM_DOCTOR */
