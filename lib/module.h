#include <stdint.h>

struct load_context;
struct module;
struct name;

int module_create(struct module **mp, const uint8_t *p, const uint8_t *ep,
                  struct load_context *ctx);
void module_destroy(struct module *m);
int module_find_export(const struct module *m, const struct name *name,
                       uint32_t type, uint32_t *idxp);
int module_find_export_func(const struct module *m, const struct name *name,
                            uint32_t *funcidxp);
void module_print_stats(const struct module *m);
