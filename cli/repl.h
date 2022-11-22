#include <stdbool.h>

#include "options.h"
#include "type.h"

struct repl_options {
        const char *prompt;
        struct repl_state *state;
        bool print_stats;
        struct load_options load_options;
};

struct repl_module_state {
        char *name;
        uint8_t *buf;
        size_t bufsize;
        bool buf_mapped;
        struct module *module;
        struct instance *inst;
};

/* eg. const.wast has 366 modules */
#define MAX_MODULES 500

struct repl_state {
        struct repl_module_state modules[MAX_MODULES];
        unsigned int nmodules;
        struct import_object *imports;
        unsigned int nregister;
        struct name registered_names[MAX_MODULES];
        struct val *param;
        struct val *result;
        struct wasi_instance *wasi;
        struct repl_options opts;
};

void repl_state_init(struct repl_state *state);
int repl(struct repl_state *state);

void repl_reset(struct repl_state *state);
int repl_load(struct repl_state *state, const char *modname,
              const char *filename);
int repl_register(struct repl_state *state, const char *modname,
                  const char *register_name);
int repl_invoke(struct repl_state *state, const char *moodname,
                const char *cmd, uint32_t *exitcodep, bool print_result);
void repl_print_version(void);

int repl_load_wasi(struct repl_state *state);
int repl_set_wasi_args(struct repl_state *state, int argc, char *const *argv);
int repl_set_wasi_environ(struct repl_state *state, int nenvs,
                          char *const *envs);
int repl_set_wasi_prestat(struct repl_state *state, const char *path);
