#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "type.h"

/*
 * Note: resulttype_alloc/resulttype_free allocates the structure
 * differently from module.c.
 */
int
resulttype_alloc(uint32_t ntypes, const enum valtype *types,
                 struct resulttype **resultp)
{
        struct resulttype *p;
        uint32_t i;
        size_t bytesize1;
        size_t bytesize;

        bytesize1 = ntypes * sizeof(*p->types);
        if (bytesize1 / ntypes != sizeof(*p->types)) {
                return EOVERFLOW;
        }
        bytesize = sizeof(*p) + bytesize1;
        if (bytesize <= bytesize1) {
                return EOVERFLOW;
        }
        p = malloc(bytesize);
        if (p == NULL) {
                return ENOMEM;
        }
        p->ntypes = ntypes;
        p->types = (void *)p + sizeof(*p);
        for (i = 0; i < ntypes; i++) {
                p->types[i] = types[i];
        }
        p->is_static = false;
        *resultp = p;
        return 0;
}

void
resulttype_free(struct resulttype *p)
{
        if (p == NULL) {
                return;
        }
        if (p->is_static) {
                return;
        }
        free(p);
}

uint32_t
ptr2pc(struct module *m, const uint8_t *p)
{
        assert(p >= m->bin);
        assert(p - m->bin <= UINT32_MAX);
        return p - m->bin;
}

const uint8_t *
pc2ptr(struct module *m, uint32_t pc)
{
        return m->bin + pc;
}
