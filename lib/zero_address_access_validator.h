#ifndef ZERO_ADDRESS_ACCESS_VALIDATOR
#define ZERO_ADDRESS_ACCESS_VALIDATOR

#include "error_reporter.h"

struct zero_address_access_validator {
        struct error_reporter *reporter;
};

void check_zero_address(struct zero_address_access_validator *validator, size_t address);
void zero_address_access_validator_init(struct zero_address_access_validator *validator,
                                        struct error_reporter *reporter);

#endif /* ZERO_ADDRESS_ACCESS_VALIDATOR */
