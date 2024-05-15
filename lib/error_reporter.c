#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error_reporter.h"
#include "wasm_state.h"

static bool
is_dlmalloc(char *function_name)
{
        if (!strcmp(function_name, "dlmalloc")) {
                return true;
        }

        return false;
}

static bool
is_dlfree(char *function_name)
{
        if (!strcmp(function_name, "dlfree")) {
                return true;
        }

        return false;
}

static bool
is_start(char *function_name)
{
        if (!strcmp(function_name, "_start")) {
                return true;
        }

        return false;
}

static bool
is_stdio_exit(char *function_name)
{
        if (!strcmp(function_name, "__stdio_exit")) {
                return true;
        }

        return false;
}

static bool
is_isatty(char *function_name)
{
        if (!strcmp(function_name, "__isatty")) {
                return true;
        }

        return false;
}

static bool
is_writev(char *function_name)
{
        if (!strcmp(function_name, "writev")) {
                return true;
        }

        return false;
}

static bool
is_read(char *function_name)
{
        if (!strcmp(function_name, "read")) {
                return true;
        }

        return false;
}

static bool
is_stdio_read(char *function_name)
{
        if (!strcmp(function_name, "__stdio_read")) {
                return true;
        }

        return false;
}

static bool
is_vfscanf(char *function_name)
{
        if (!strcmp(function_name, "vfscanf")) {
                return true;
        }

        return false;
}

static bool
is_lseek(char *function_name)
{
        if (!strcmp(function_name, "__lseek")) {
                return true;
        }

        return false;
}

static bool
is_result_of_fwritex(struct error_reporter *reporter)
{
        for (size_t i = 1; i <= reporter->state->function_names_size; i++) {
                char *function_name = reporter->state->function_names[reporter->state->function_names_size - i];
                if (!strcmp(function_name, "__fwritex")) {
                        return true;
                }
        }

        return false;
}

static bool
is_blacklisted(char *function_name)
{
        return is_dlmalloc(function_name) || is_dlfree(function_name) || is_start(function_name) ||
               is_stdio_exit(function_name) || is_isatty(function_name) || is_writev(function_name);
}

bool
is_undefined_memory_use_blacklisted(struct error_reporter *reporter)
{
        char *function_name = reporter->state->function_names[reporter->state->function_names_size - 1];
        return is_blacklisted(function_name) || is_read(function_name) || is_stdio_read(function_name) ||
               is_vfscanf(function_name);
}

bool
is_undefined_local_use_blacklisted(struct error_reporter *reporter)
{
        char *function_name = reporter->state->function_names[reporter->state->function_names_size - 1];
        return is_blacklisted(function_name) || true;
}

bool
is_use_after_free_blacklisted(struct error_reporter *reporter)
{
        char *function_name = reporter->state->function_names[reporter->state->function_names_size - 1];
        return is_blacklisted(function_name);
}

bool
is_memory_leak_blacklisted(char *function_name)
{
        return is_blacklisted(function_name);
}

bool
is_double_free_blacklisted(struct error_reporter *reporter)
{
        char *function_name = reporter->state->function_names[reporter->state->function_names_size - 1];
        return is_blacklisted(function_name);
}

bool
is_invalid_free_blacklisted(struct error_reporter *reporter)
{
        char *function_name = reporter->state->function_names[reporter->state->function_names_size - 1];
        return is_blacklisted(function_name);
}

bool
is_invalid_read_blacklisted(struct error_reporter *reporter)
{
        char *function_name = reporter->state->function_names[reporter->state->function_names_size - 1];
        return is_blacklisted(function_name) || is_stdio_read(function_name) || is_vfscanf(function_name);
}

bool
is_invalid_write_blacklisted(struct error_reporter *reporter)
{
        char *function_name = reporter->state->function_names[reporter->state->function_names_size - 1];
        return is_blacklisted(function_name) || is_result_of_fwritex(reporter) || is_lseek(function_name);
}

static void
print_stack_trace(struct error_reporter *reporter)
{
        if (reporter->state->function_names_size >= 1) {
                printf("==Wasm Doctor== ");
                for (size_t i = reporter->state->function_names_size - 1; i > 0; --i) {
                        printf("%s <- ", reporter->state->function_names[i]);
                }
                printf("%s\n", reporter->state->function_names[0]);
                printf("==Wasm Doctor== \n");
        }
}

void
add_undefined_memory_use(struct error_reporter *reporter, size_t address, bool *validity)
{
        struct undefined_memory_use **errors = &reporter->undefined_memory_use_errors;
        size_t *errors_size = &reporter->undefined_memory_use_errors_size;

        *errors = (struct undefined_memory_use *)realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = reporter->state->size_in_bytes;
        (*errors)[*errors_size - 1].validity =
                malloc(reporter->state->size_in_bytes * 8 * sizeof(*((*errors)[*errors_size - 1].validity)));
        memcpy((*errors)[*errors_size - 1].validity, validity, reporter->state->size_in_bytes * 8);

        if (reporter->report && !is_undefined_memory_use_blacklisted(reporter)) {
                printf("==Wasm Doctor== Undefined value of size %zu bytes read from address "
                       "%zu.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address);

                printf("==Wasm Doctor== validity: ");
                for (size_t j = 0; j < (*errors)[*errors_size - 1].size * 8; ++j) {
                        printf("%u", (*errors)[*errors_size - 1].validity[j]);
                }
                printf("\n");

                print_stack_trace(reporter);
        }
}

void
add_undefined_local_use(struct error_reporter *reporter, size_t idx)
{
        struct undefined_local_use **errors = &reporter->undefined_local_use_errors;
        size_t *errors_size = &reporter->undefined_local_use_errors_size;

        *errors = (struct undefined_local_use *)realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].idx = idx;

        if (reporter->report && !is_undefined_local_use_blacklisted(reporter)) {
                printf("==Wasm Doctor== Undefined local with index %zu read.\n", (*errors)[*errors_size - 1].idx);
                print_stack_trace(reporter);
        }
}

void
add_use_after_free(struct error_reporter *reporter, size_t address)
{
        struct use_after_free **errors = &reporter->use_after_free_errors;
        size_t *errors_size = &reporter->use_after_free_errors_size;

        *errors = (struct use_after_free *)realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = reporter->state->size_in_bytes;

        if (reporter->report && !is_use_after_free_blacklisted(reporter)) {
                printf("==Wasm Doctor== Use after free of size %zu bytes detected at address "
                       "%zu.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address * 8);
                print_stack_trace(reporter);
        }
}

void
add_memory_leak(struct error_reporter *reporter, size_t address, uint8_t size_in_bytes, char *function_name)
{
        struct memory_leak **errors = &reporter->memory_leak_errors;
        size_t *errors_size = &reporter->memory_leak_errors_size;

        *errors = (struct memory_leak *)realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = size_in_bytes;

        if (reporter->report && !is_memory_leak_blacklisted(function_name)) {
                printf("==Wasm Doctor== Memory leak of size %zu bytes detected at address "
                       "%zu.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address * 8);
                print_stack_trace(reporter);
        }
}

void
add_double_free(struct error_reporter *reporter, size_t address)
{
        struct double_free **errors = &reporter->double_free_errors;
        size_t *errors_size = &reporter->double_free_errors_size;

        *errors = (struct double_free *)realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;

        if (reporter->report && !is_double_free_blacklisted(reporter)) {
                printf("==Wasm Doctor== Double free detected at address %zu.\n",
                       (*errors)[*errors_size - 1].address * 8);
                print_stack_trace(reporter);
        }
}

void
add_invalid_free(struct error_reporter *reporter, size_t address)
{
        struct invalid_free **errors = &reporter->invalid_free_errors;
        size_t *errors_size = &reporter->invalid_free_errors_size;

        *errors = (struct invalid_free *)realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;

        if (reporter->report && !is_invalid_free_blacklisted(reporter)) {
                printf("==Wasm Doctor== Invalid free detected at address %zu.\n",
                       (*errors)[*errors_size - 1].address * 8);
                print_stack_trace(reporter);
        }
}

void
add_invalid_read(struct error_reporter *reporter, size_t address)
{
        struct invalid_read **errors = &reporter->invalid_read_errors;
        size_t *errors_size = &reporter->invalid_read_errors_size;

        *errors = (struct invalid_read *)realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = reporter->state->size_in_bytes;

        if (reporter->report && !is_invalid_read_blacklisted(reporter)) {
                printf("==Wasm Doctor== Invalid read of size %zu bytes detected at address %zu.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address);
                print_stack_trace(reporter);
        }
}

void
add_invalid_write(struct error_reporter *reporter, size_t address)
{
        struct invalid_write **errors = &reporter->invalid_write_errors;
        size_t *errors_size = &reporter->invalid_write_errors_size;

        *errors = (struct invalid_write *)realloc(*errors, ++(*errors_size) * sizeof(**errors));
        (*errors)[*errors_size - 1].address = address;
        (*errors)[*errors_size - 1].size = reporter->state->size_in_bytes;

        if (reporter->report && !is_invalid_write_blacklisted(reporter)) {
                printf("==Wasm Doctor== Invalid write of size %zu bytes detected at address %zu.\n",
                       (*errors)[*errors_size - 1].size, (*errors)[*errors_size - 1].address);
                print_stack_trace(reporter);
        }
}

void
add_zero_address_access(struct error_reporter *reporter)
{
        struct zero_address_access **errors = &reporter->zero_address_access_errors;
        size_t *errors_size = &reporter->zero_address_access_errors_size;

        *errors = (struct zero_address_access *)realloc(*errors, ++(*errors_size) * sizeof(**errors));

        if (reporter->report) {
                printf("==Wasm Doctor== Address zero access detected.\n");
                print_stack_trace(reporter);
        }
}

void
reporter_init(struct error_reporter *reporter, struct wasm_state *state, bool report)
{
        reporter->undefined_memory_use_errors_size = 0;
        reporter->undefined_memory_use_errors = NULL;

        reporter->undefined_local_use_errors_size = 0;
        reporter->undefined_local_use_errors = NULL;

        reporter->use_after_free_errors_size = 0;
        reporter->use_after_free_errors = NULL;

        reporter->memory_leak_errors_size = 0;
        reporter->memory_leak_errors = NULL;

        reporter->double_free_errors_size = 0;
        reporter->double_free_errors = NULL;

        reporter->invalid_free_errors_size = 0;
        reporter->invalid_free_errors = NULL;

        reporter->invalid_read_errors_size = 0;
        reporter->invalid_read_errors = NULL;

        reporter->invalid_write_errors_size = 0;
        reporter->invalid_write_errors = NULL;

        reporter->zero_address_access_errors_size = 0;
        reporter->zero_address_access_errors = NULL;

        reporter->state = state;

        reporter->report = report;
}

void
reporter_exit(struct error_reporter *reporter)
{
        for (size_t i = 0; i < reporter->undefined_memory_use_errors_size; ++i) {
                free(reporter->undefined_memory_use_errors[i].validity);
        }

        free(reporter->undefined_memory_use_errors);
        free(reporter->undefined_local_use_errors);
        free(reporter->use_after_free_errors);
        free(reporter->memory_leak_errors);
        free(reporter->double_free_errors);
        free(reporter->invalid_free_errors);
        free(reporter->invalid_read_errors);
        free(reporter->invalid_write_errors);
        free(reporter->zero_address_access_errors);

        wasm_state_exit(reporter->state);
}
