#include "zero_address_access_validator.h"
#include "error_reporter.h"

void
check_zero_address(struct zero_address_access_validator *validator, size_t address)
{
        if (address == 0) {
                add_zero_address_access(validator->reporter);
        }
}

void
zero_address_access_validator_init(struct zero_address_access_validator *validator, struct error_reporter *reporter)
{
        validator->reporter = reporter;
}
