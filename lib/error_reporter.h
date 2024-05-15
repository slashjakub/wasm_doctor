#ifndef ERROR_REPORTER
#define ERROR_REPORTER

#include <stdbool.h>
#include <stddef.h>

#include "wasm_state.h"

struct undefined_memory_use {
        size_t address;
        size_t size;
        bool *validity;
};

struct undefined_local_use {
        size_t idx;
        size_t size;
};

struct use_after_free {
        size_t address;
        size_t size;
};

struct memory_leak {
        size_t address;
        size_t size;
};

struct double_free {
        size_t address;
};

struct invalid_free {
        size_t address;
};

struct invalid_read {
        size_t address;
        size_t size;
};

struct invalid_write {
        size_t address;
        size_t size;
};

struct zero_address_access {
        size_t address;
        size_t size;
};

struct error_reporter {
        struct wasm_state *state;
        bool report;

        size_t undefined_memory_use_errors_size;
        struct undefined_memory_use *undefined_memory_use_errors;

        size_t undefined_local_use_errors_size;
        struct undefined_local_use *undefined_local_use_errors;

        size_t use_after_free_errors_size;
        struct use_after_free *use_after_free_errors;

        size_t memory_leak_errors_size;
        struct memory_leak *memory_leak_errors;

        size_t double_free_errors_size;
        struct double_free *double_free_errors;

        size_t invalid_free_errors_size;
        struct invalid_free *invalid_free_errors;

        size_t invalid_read_errors_size;
        struct invalid_read *invalid_read_errors;

        size_t invalid_write_errors_size;
        struct invalid_write *invalid_write_errors;

        size_t zero_address_access_errors_size;
        struct zero_address_access *zero_address_access_errors;
};

bool is_undefined_memory_use_blacklisted(struct error_reporter *reporter);
bool is_undefined_local_use_blacklisted(struct error_reporter *reporter);
bool is_use_after_free_blacklisted(struct error_reporter *reporter);
bool is_memory_leak_blacklisted(char *function_name);
bool is_double_free_blacklisted(struct error_reporter *reporter);
bool is_invalid_free_blacklisted(struct error_reporter *reporter);
bool is_invalid_read_blacklisted(struct error_reporter *reporter);
bool is_invalid_write_blacklisted(struct error_reporter *reporter);

void add_undefined_memory_use(struct error_reporter *reporter, size_t address, bool *validity);
void add_undefined_local_use(struct error_reporter *reporter, size_t idx);
void add_use_after_free(struct error_reporter *reporter, size_t address);
void add_memory_leak(struct error_reporter *reporter, size_t address, uint8_t size_in_bytes, char *function_name);
void add_double_free(struct error_reporter *reporter, size_t address);
void add_invalid_free(struct error_reporter *reporter, size_t address);
void add_invalid_read(struct error_reporter *reporter, size_t address);
void add_invalid_write(struct error_reporter *reporter, size_t address);
void add_zero_address_access(struct error_reporter *reporter);
void reporter_init(struct error_reporter *reporter, struct wasm_state *state, bool report);
void reporter_exit(struct error_reporter *reporter);

#endif /* ERROR_REPORTER */
