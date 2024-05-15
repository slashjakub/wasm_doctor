#ifndef LINEAR_STACK_VALIDATOR
#define LINEAR_STACK_VALIDATOR

#include <stddef.h>

#include "shadow_memory_validator.h"

// red zone explained in WebAssembly C ABI
// https://github.com/WebAssembly/tool-conventions/blob/main/BasicCABI.md#the-linear-stack
#define RED_ZONE_SIZE 128

/**
 * Linear stack validator is responsible for maintenance of the state of C ABI linear stack in WebAssembly linear
 * memory and the invalidation of its regions as they are being deallocated.
 */
struct linear_stack_validator {
        size_t linear_stack_pointer_base;
        size_t linear_stack_pointer;
};

void set_linear_stack_pointer_base(struct linear_stack_validator *validator, size_t address);
void move_linear_stack_pointer(struct linear_stack_validator *validator,
                               struct shadow_memory_validator *shadow_memory_validator, size_t address);
void linear_stack_validator_init(struct linear_stack_validator *validator);

#endif /* LINEAR_STACK_VALIDATOR */
