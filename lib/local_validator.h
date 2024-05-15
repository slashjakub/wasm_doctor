#ifndef LOCAL_VALIDATOR
#define LOCAL_VALIDATOR

#include <stdbool.h>
#include <stdint.h>

#include "error_reporter.h"

struct local_validator {
        struct error_reporter *reporter;
        uint32_t frames_size;
        bool **locals;
};

void register_set(struct local_validator *validator, uint32_t local_idx);
void validate_get(struct local_validator *validator, uint32_t local_idx);
void local_validator_frame_enter(struct local_validator *validator, uint32_t locals_size);
void local_validator_frame_exit(struct local_validator *validator);
void local_validator_init(struct local_validator *validator, struct error_reporter *reporter);
void local_validator_exit(struct local_validator *validator);

#endif /* LOCAL_VALIDATOR */
