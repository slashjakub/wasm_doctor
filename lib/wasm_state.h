#ifndef WASM_STATE
#define WASM_STATE

#include <stdint.h>

struct wasm_state {
        uint32_t function_names_size;
        char **function_names;
        uint8_t size_in_bytes;
};

void enter_function(struct wasm_state *state, char *function_name);
void exit_function(struct wasm_state *state);
void set_byte_size(struct wasm_state *state, uint8_t size_in_bytes);
void wasm_state_init(struct wasm_state *state);
void wasm_state_exit(struct wasm_state *state);

#endif /* WASM_STATE */
