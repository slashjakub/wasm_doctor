#define UNOP(NAME, I_OR_F, BITS, OP)                                          \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                POP_VAL(TYPE_##I_OR_F##BITS, a);                              \
                struct val val_c;                                             \
                if (EXECUTING) {                                              \
                        val_c.u.I_OR_F##BITS = OP(val_a.u.I_OR_F##BITS);      \
                }                                                             \
                PUSH_VAL(TYPE_##I_OR_F##BITS, c);                             \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define BINOP(NAME, I_OR_F, BITS, OP)                                         \
        BINOP2(NAME, I_OR_F, BITS, OP, false, false, false)

#define BINOP2(NAME, I_OR_F, BITS, OP, IS_DIV_OR_REM, IS_DIV_S_OR_REM_S,      \
               IS_DIV)                                                        \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                POP_VAL(TYPE_##I_OR_F##BITS, b);                              \
                POP_VAL(TYPE_##I_OR_F##BITS, a);                              \
                struct val val_c;                                             \
                if (EXECUTING) {                                              \
                        if (IS_DIV_OR_REM && val_b.u.i##BITS == 0) {          \
                                TRAP(TRAP_DIV_BY_ZERO, "division by zero");   \
                        }                                                     \
                        if (IS_DIV_S_OR_REM_S &&                              \
                            val_a.u.I_OR_F##BITS == INT##BITS##_MIN &&        \
                            val_b.u.I_OR_F##BITS == -1) {                     \
                                if (IS_DIV) {                                 \
                                        TRAP(TRAP_INTEGER_OVERFLOW,           \
                                             "integer overflow");             \
                                } else {                                      \
                                        val_c.u.I_OR_F##BITS = 0;             \
                                }                                             \
                        } else {                                              \
                                val_c.u.I_OR_F##BITS =                        \
                                        OP(BITS, val_a.u.I_OR_F##BITS,        \
                                           val_b.u.I_OR_F##BITS);             \
                        }                                                     \
                }                                                             \
                PUSH_VAL(TYPE_##I_OR_F##BITS, c);                             \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define TESTOP(NAME, I_OR_F, BITS, OP)                                        \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                POP_VAL(TYPE_##I_OR_F##BITS, a);                              \
                struct val val_b;                                             \
                if (EXECUTING) {                                              \
                        val_b.u.i32 = OP(val_a.u.I_OR_F##BITS);               \
                }                                                             \
                PUSH_VAL(TYPE_i32, b);                                        \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define CMPOP(NAME, BITS, CTYPE, OP) CMPOP2(NAME, BITS, (CTYPE), OP, i)
#define CMPOP_F(NAME, BITS, OP) CMPOP2(NAME, BITS, , OP, f)
#define CMPOP2(NAME, BITS, CAST, OP, I_OR_F)                                  \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                POP_VAL(TYPE_##I_OR_F##BITS, b);                              \
                POP_VAL(TYPE_##I_OR_F##BITS, a);                              \
                struct val val_c;                                             \
                if (EXECUTING) {                                              \
                        val_c.u.i32 = CAST val_a.u.I_OR_F##BITS OP CAST       \
                                              val_b.u.I_OR_F##BITS;           \
                }                                                             \
                PUSH_VAL(TYPE_i32, c);                                        \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define EXTENDOP(NAME, FROM_WTYPE, TO_WTYPE, CCAST)                           \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                POP_VAL(TYPE_##FROM_WTYPE, a);                                \
                struct val val_b;                                             \
                if (EXECUTING) {                                              \
                        val_b.u.TO_WTYPE = CCAST val_a.u.FROM_WTYPE;          \
                }                                                             \
                PUSH_VAL(TYPE_##TO_WTYPE, b);                                 \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define REINTERPRETOP(NAME, FROM_WTYPE, TO_WTYPE)                             \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                POP_VAL(TYPE_##FROM_WTYPE, a);                                \
                PUSH_VAL(TYPE_##TO_WTYPE, a);                                 \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define LOADOP(NAME, MEM, STACK, CAST) LOADOP2(NAME, MEM, STACK, CAST, i)
#define LOADOP_F(NAME, MEM, STACK, CAST) LOADOP2(NAME, MEM, STACK, CAST, f)
#define LOADOP2(NAME, MEM, STACK, CAST, I_OR_F)                               \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                const struct module *m = MODULE;                              \
                struct memarg memarg;                                         \
                int ret;                                                      \
                LOAD_PC;                                                      \
                READ_MEMARG(&memarg);                                         \
                CHECK(memarg.memidx < m->nimportedmems + m->nmems);           \
                CHECK(1 <= (MEM / 8) >>                                       \
                      memarg.align); /* 2 ** align <= N / 8 */                \
                POP_VAL(TYPE_i32, i);                                         \
                struct val val_c;                                             \
                if (EXECUTING) {                                              \
                        void *datap;                                          \
                        ret = memory_getptr(ECTX, memarg.memidx, val_i.u.i32, \
                                            memarg.offset, MEM / 8, &datap);  \
                        if (ret != 0) {                                       \
                                goto fail;                                    \
                        }                                                     \
                        val_c.u.i##STACK = CAST le##MEM##_decode(datap);      \
                                                                              \
                        printf("loaded (valid: %d) %u at addr %d (%x)\n",     \
                               validate_load(val_i.u.i32), val_c.u.i32,       \
                               val_i.u.i32, val_i.u.i32);                     \
                }                                                             \
                PUSH_VAL(TYPE_##I_OR_F##STACK, c);                            \
                SAVE_PC;                                                      \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define STOREOP(NAME, MEM, STACK, CAST) STOREOP2(NAME, MEM, STACK, CAST, i)
#define STOREOP_F(NAME, MEM, STACK, CAST) STOREOP2(NAME, MEM, STACK, CAST, f)
#define STOREOP2(NAME, MEM, STACK, CAST, I_OR_F)                              \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                const struct module *m = MODULE;                              \
                struct memarg memarg;                                         \
                int ret;                                                      \
                LOAD_PC;                                                      \
                READ_MEMARG(&memarg);                                         \
                CHECK(memarg.memidx < m->nimportedmems + m->nmems);           \
                CHECK(1 <= (MEM / 8) >>                                       \
                      memarg.align); /* 2 ** align <= N / 8 */                \
                POP_VAL(TYPE_##I_OR_F##STACK, v);                             \
                POP_VAL(TYPE_i32, i);                                         \
                if (EXECUTING) {                                              \
                        void *datap;                                          \
                        ret = memory_getptr(ECTX, memarg.memidx, val_i.u.i32, \
                                            memarg.offset, MEM / 8, &datap);  \
                        if (ret != 0) {                                       \
                                goto fail;                                    \
                        }                                                     \
                        le##MEM##_encode(datap, CAST val_v.u.i##STACK);       \
                                                                              \
                        printf("stored %u at addr %u (%x)\n", val_v.u.i32,    \
                               val_i.u.i32, val_i.u.i32);                     \
                        register_store(val_i.u.i32, 1);                       \
                }                                                             \
                SAVE_PC;                                                      \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define BITCOUNTOP(NAME, TYPE, OPE)                                           \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                POP_VAL(TYPE_##TYPE, a);                                      \
                struct val val_b;                                             \
                if (EXECUTING) {                                              \
                        val_b.u.TYPE = OPE(val_a.u.TYPE);                     \
                }                                                             \
                PUSH_VAL(TYPE_##TYPE, b);                                     \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

/* https://webassembly.github.io/spec/core/exec/instructions.html#exec-cvtop */
#define CVTOP(NAME, T1, T2, OPE)                                              \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                POP_VAL(TYPE_##T1, a);                                        \
                struct val val_b;                                             \
                if (EXECUTING) {                                              \
                        OPE(val_b.u.T2, val_a.u.T1);                          \
                }                                                             \
                PUSH_VAL(TYPE_##T2, b);                                       \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define CONSTOP(NAME, BITS, WTYPE)                                            \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                int ret;                                                      \
                LOAD_PC;                                                      \
                READ_LEB_I##BITS(v);                                          \
                struct val val_c;                                             \
                if (EXECUTING) {                                              \
                        val_c.u.i##BITS = v;                                  \
                }                                                             \
                PUSH_VAL(TYPE_##WTYPE, c);                                    \
                SAVE_PC;                                                      \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }

#define CONSTOP_F(NAME, BITS, WTYPE)                                          \
        INSN_IMPL(NAME)                                                       \
        {                                                                     \
                uint##BITS##_t v;                                             \
                int ret;                                                      \
                LOAD_PC;                                                      \
                ret = read_u##BITS(&p, ep, &v);                               \
                CHECK_RET(ret);                                               \
                struct val val_c;                                             \
                if (EXECUTING) {                                              \
                        val_c.u.i##BITS = v;                                  \
                }                                                             \
                PUSH_VAL(TYPE_##WTYPE, c);                                    \
                SAVE_PC;                                                      \
                INSN_SUCCESS;                                                 \
fail:                                                                         \
                INSN_FAIL;                                                    \
        }
