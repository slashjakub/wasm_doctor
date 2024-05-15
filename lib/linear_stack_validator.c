#include "linear_stack_validator.h"
#include "shadow_memory_validator.h"

void
set_linear_stack_pointer_base(struct linear_stack_validator *validator, size_t address)
{
        validator->linear_stack_pointer_base = address;
        validator->linear_stack_pointer = address;
}

void
move_linear_stack_pointer(struct linear_stack_validator *validator,
                          struct shadow_memory_validator *shadow_memory_validator, size_t address)
{
        if (validator->linear_stack_pointer_base == UINT32_MAX) {
                validator->linear_stack_pointer_base = address;
        }

        if (address > validator->linear_stack_pointer) {
                invalidate_region(shadow_memory_validator, validator->linear_stack_pointer - RED_ZONE_SIZE,
                                  address - validator->linear_stack_pointer + RED_ZONE_SIZE);
        }

        validator->linear_stack_pointer = address;
}

void
linear_stack_validator_init(struct linear_stack_validator *validator)
{
        validator->linear_stack_pointer_base = UINT32_MAX;
        validator->linear_stack_pointer = UINT32_MAX;
}
