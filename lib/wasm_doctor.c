#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"
#include "heap_use_validator.h"
#include "linear_stack_validator.h"
#include "local_validator.h"
#include "shadow_memory_validator.h"
#include "wasm_doctor.h"
#include "wasm_state.h"

#include "zero_address_access_validator.h"

#define WASM_PAGE_SIZE 65536

struct wasm_doctor *doctor;

/**
 * Set the linear stack pointer base to position defined in WebAssembly module.
 * This function is Clang/LLVM linear stack pointer specific.
 * It is meant to be used with the $__stack_pointer global in WebAssembly.
 *
 * @param[in] address Address of the linear stack pointer base.
 */
void
doctor_set_linear_stack_pointer_base(size_t address)
{
        set_linear_stack_pointer_base(&doctor->linear_stack_validator, address);
}

/**
 * Move the linear stack pointer to its new position.
 * If the stack size decreases invalidate values in the region that is no longer part of the linear stack.
 * This function is Clang/LLVM linear stack pointer specific.
 * It is meant to be used with the $__stack_pointer global in WebAssembly.
 *
 * @param[in] address New address of the linear stack pointer.
 */
void
doctor_move_linear_stack_pointer(size_t address)
{
        move_linear_stack_pointer(&doctor->linear_stack_validator, &doctor->shadow_memory_validator, address);
}

/**
 * @param[in] address Address of the global data to be validated.
 * @param[in] size_in_bytes Size of the global data to be validated in bytes.
 */
void
doctor_global_data_validate(size_t address, size_t size_in_bytes)
{
        validate_region(&doctor->shadow_memory_validator, address, size_in_bytes);
        register_global_data(&doctor->heap_validator, address, size_in_bytes);
}

/**
 * @param[in] address Address of the store.
 * @param[in] size_in_bytes Size of the store in bytes.
 */
void
doctor_store(size_t address, uint8_t size_in_bytes)
{
        set_byte_size(doctor->reporter.state, size_in_bytes);
        check_use_after_free(&doctor->heap_validator, address, size_in_bytes);
        check_write_validity(&doctor->heap_validator, address, size_in_bytes);
        validate_region(&doctor->shadow_memory_validator, address, size_in_bytes);
        check_zero_address(&doctor->zero_validator, address);
}

/**
 * @param[in] address Address of the load.
 * @param[in] size_in_bytes Size of the load in bytes.
 */
void
doctor_load(size_t address, uint8_t size_in_bytes)
{
        set_byte_size(doctor->reporter.state, size_in_bytes);
        check_use_after_free(&doctor->heap_validator, address, size_in_bytes);
        check_read_validity(&doctor->heap_validator, address, size_in_bytes);
        check_region_access(&doctor->shadow_memory_validator, address, size_in_bytes);
        check_zero_address(&doctor->zero_validator, address);
}

/**
 * @param[in] block_start Start address of the allocated block.
 * @param[in] size_in_bytes Size of the allocated block.
 */
void
doctor_register_malloc(size_t block_start, size_t size_in_bytes)
{
        register_malloc(&doctor->heap_validator, block_start, size_in_bytes);
}

/**
 * @param[in] block_start Start address of the allocated block.
 */
void
doctor_register_free(size_t block_start)
{
        register_free(&doctor->heap_validator, block_start);
}

/**
 * @param[in] idx Index of the local which is being set by the local.set WebAssembly instruction.
 */
void
doctor_local_set(uint32_t idx)
{
        register_set(&doctor->local_validator, idx);
}

/**
 * @param[in] idx Index of the local which is accessed by the local.get WebAssembly instruction.
 */
void
doctor_local_get(uint32_t idx)
{
        validate_get(&doctor->local_validator, idx);
}

/**
 * @param[in] locals_size The number of locals a WebAssembly function (frame) has.
 * @param[in] function_name The name of function that is being entered.
 */
void
doctor_frame_enter(uint32_t locals_size, char *function_name)
{
        enter_function(doctor->reporter.state, function_name);
        local_validator_frame_enter(&doctor->local_validator, locals_size);
}

void
doctor_frame_exit(void)
{
        exit_function(doctor->reporter.state);
        local_validator_frame_exit(&doctor->local_validator);
}

/**
 * @param[in, out] wasm_doctor Wasm Doctor struct containing all validators and reports.
 * @param[in] size_in_pages Size of WebAssembly memory in pages.
 */
void
doctor_init(struct wasm_doctor *wasm_doctor, uint32_t size_in_pages, bool report)
{
        doctor = wasm_doctor;
        wasm_state_init(&doctor->state);
        reporter_init(&doctor->reporter, &doctor->state, report);
        local_validator_init(&doctor->local_validator, &doctor->reporter);
        heap_use_validator_init(&doctor->heap_validator, &doctor->linear_stack_validator, &doctor->reporter);
        shadow_memory_validator_init(&doctor->shadow_memory_validator, WASM_PAGE_SIZE * size_in_pages,
                                     &doctor->reporter);
        linear_stack_validator_init(&doctor->linear_stack_validator);
        zero_address_access_validator_init(&doctor->zero_validator, &doctor->reporter);
}

/**
 * Exit reporter if it was not previously done by doctor_exit. This may happens in case the reports are needed after the
 * analysis has already stopped.
 */
void
doctor_reporter_exit(void)
{
        reporter_exit(&doctor->reporter);
}

/**
 * @param[in] exit_reporter Boolean value decidig whether to call reporter_exit or keep the reports for further use
 * (i.e. testing).
 */
void
doctor_exit(bool exit_reporter)
{
        shadow_memory_validator_exit(&doctor->shadow_memory_validator);
        heap_use_validator_exit(&doctor->heap_validator);
        local_validator_exit(&doctor->local_validator);

        if (exit_reporter) {
                doctor_reporter_exit();
        }
}
