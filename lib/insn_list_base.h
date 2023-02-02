/* clang-format off */

INSTRUCTION(0x00, "unreachable", unreachable, 0)
INSTRUCTION(0x01, "nop", nop, 0)
INSTRUCTION(0x02, "block", block, 0)
INSTRUCTION(0x03, "loop", loop, 0)
INSTRUCTION(0x04, "if", if, 0)
INSTRUCTION(0x05, "else", else, 0)
INSTRUCTION(0x0b, "end", end, INSN_FLAG_CONST)
INSTRUCTION(0x0c, "br", br, 0)
INSTRUCTION(0x0d, "br_if", br_if, 0)
INSTRUCTION(0x0e, "br_table", br_table, 0)
INSTRUCTION(0x0f, "return", return, 0)
INSTRUCTION(0x10, "call", call, 0)
INSTRUCTION(0x11, "call_indirect", call_indirect, 0)
INSTRUCTION(0x1a, "drop", drop, 0)
INSTRUCTION(0x1b, "select", select, 0)
INSTRUCTION(0x1c, "select_t", select_t, 0)
INSTRUCTION(0x20, "local.get", local_get, 0)
INSTRUCTION(0x21, "local.set", local_set, 0)
INSTRUCTION(0x22, "local.tee", local_tee, 0)
INSTRUCTION(0x23, "global.get", global_get, INSN_FLAG_CONST)
INSTRUCTION(0x24, "global.set", global_set, 0)
INSTRUCTION(0x25, "table_get", table_get, 0)
INSTRUCTION(0x26, "table_set", table_set, 0)

INSTRUCTION(0x28, "i32.load", i32_load, 0)
INSTRUCTION(0x29, "i64.load", i64_load, 0)
INSTRUCTION(0x2a, "f32.load", f32_load, 0)
INSTRUCTION(0x2b, "f64.load", f64_load, 0)

INSTRUCTION(0x2c, "i32.load8_s", i32_load8_s, 0)
INSTRUCTION(0x2d, "i32.load8_u", i32_load8_u, 0)
INSTRUCTION(0x2e, "i32.load16_s", i32_load16_s, 0)
INSTRUCTION(0x2f, "i32.load16_u", i32_load16_u, 0)

INSTRUCTION(0x30, "i64.load8_s", i64_load8_s, 0)
INSTRUCTION(0x31, "i64.load8_u", i64_load8_u, 0)
INSTRUCTION(0x32, "i64.load16_s", i64_load16_s, 0)
INSTRUCTION(0x33, "i64.load16_u", i64_load16_u, 0)
INSTRUCTION(0x34, "i64.load32_s", i64_load32_s, 0)
INSTRUCTION(0x35, "i64.load32_u", i64_load32_u, 0)

INSTRUCTION(0x36, "i32.store", i32_store, 0)
INSTRUCTION(0x37, "i64.store", i64_store, 0)
INSTRUCTION(0x38, "f32.store", f32_store, 0)
INSTRUCTION(0x39, "f64.store", f64_store, 0)
INSTRUCTION(0x3a, "i32.store8", i32_store8, 0)
INSTRUCTION(0x3b, "i32.store16", i32_store16, 0)
INSTRUCTION(0x3c, "i64.store8", i64_store8, 0)
INSTRUCTION(0x3d, "i64.store16", i64_store16, 0)
INSTRUCTION(0x3e, "i64.store32", i64_store32, 0)

INSTRUCTION(0x3f, "memory.size", memory_size, 0)
INSTRUCTION(0x40, "memory.grow", memory_grow, 0)

INSTRUCTION(0x41, "i32.const", i32_const, INSN_FLAG_CONST)
INSTRUCTION(0x42, "i64.const", i64_const, INSN_FLAG_CONST)
INSTRUCTION(0x43, "f32.const", f32_const, INSN_FLAG_CONST)
INSTRUCTION(0x44, "f64.const", f64_const, INSN_FLAG_CONST)

INSTRUCTION(0x45, "i32.eqz", i32_eqz, 0)
INSTRUCTION(0x46, "i32.eq", i32_eq, 0)
INSTRUCTION(0x47, "i32.ne", i32_ne, 0)
INSTRUCTION(0x48, "i32.lt_s", i32_lt_s, 0)
INSTRUCTION(0x49, "i32.lt_u", i32_lt_u, 0)
INSTRUCTION(0x4a, "i32.gt_s", i32_gt_s, 0)
INSTRUCTION(0x4b, "i32.gt_u", i32_gt_u, 0)
INSTRUCTION(0x4c, "i32.le_s", i32_le_s, 0)
INSTRUCTION(0x4d, "i32.le_u", i32_le_u, 0)
INSTRUCTION(0x4e, "i32.ge_s", i32_ge_s, 0)
INSTRUCTION(0x4f, "i32.ge_u", i32_ge_u, 0)

INSTRUCTION(0x50, "i64.eqz", i64_eqz, 0)
INSTRUCTION(0x51, "i64.eq", i64_eq, 0)
INSTRUCTION(0x52, "i64.ne", i64_ne, 0)
INSTRUCTION(0x53, "i64.lt_s", i64_lt_s, 0)
INSTRUCTION(0x54, "i64.lt_u", i64_lt_u, 0)
INSTRUCTION(0x55, "i64.gt_s", i64_gt_s, 0)
INSTRUCTION(0x56, "i64.gt_u", i64_gt_u, 0)
INSTRUCTION(0x57, "i64.le_s", i64_le_s, 0)
INSTRUCTION(0x58, "i64.le_u", i64_le_u, 0)
INSTRUCTION(0x59, "i64.ge_s", i64_ge_s, 0)
INSTRUCTION(0x5a, "i64.ge_u", i64_ge_u, 0)

INSTRUCTION(0x5b, "f32.eq", f32_eq, 0)
INSTRUCTION(0x5c, "f32.ne", f32_ne, 0)
INSTRUCTION(0x5d, "f32.lt", f32_lt, 0)
INSTRUCTION(0x5e, "f32.gt", f32_gt, 0)
INSTRUCTION(0x5f, "f32.le", f32_le, 0)
INSTRUCTION(0x60, "f32.ge", f32_ge, 0)

INSTRUCTION(0x61, "f64.eq", f64_eq, 0)
INSTRUCTION(0x62, "f64.ne", f64_ne, 0)
INSTRUCTION(0x63, "f64.lt", f64_lt, 0)
INSTRUCTION(0x64, "f64.gt", f64_gt, 0)
INSTRUCTION(0x65, "f64.le", f64_le, 0)
INSTRUCTION(0x66, "f64.ge", f64_ge, 0)

INSTRUCTION(0x67, "i32.clz", i32_clz, 0)
INSTRUCTION(0x68, "i32.ctz", i32_ctz, 0)
INSTRUCTION(0x69, "i32.popcnt", i32_popcnt, 0)
INSTRUCTION(0x6a, "i32.add", i32_add, INSN_FLAG_EXTENDED_CONST)
INSTRUCTION(0x6b, "i32.sub", i32_sub, INSN_FLAG_EXTENDED_CONST)
INSTRUCTION(0x6c, "i32.mul", i32_mul, INSN_FLAG_EXTENDED_CONST)
INSTRUCTION(0x6d, "i32.div_s", i32_div_s, 0)
INSTRUCTION(0x6e, "i32.div_u", i32_div_u, 0)
INSTRUCTION(0x6f, "i32.rem_s", i32_rem_s, 0)
INSTRUCTION(0x70, "i32.rem_u", i32_rem_u, 0)
INSTRUCTION(0x71, "i32.and", i32_and, 0)
INSTRUCTION(0x72, "i32.or", i32_or, 0)
INSTRUCTION(0x73, "i32.xor", i32_xor, 0)
INSTRUCTION(0x74, "i32.shl", i32_shl, 0)
INSTRUCTION(0x75, "i32.shr_s", i32_shr_s, 0)
INSTRUCTION(0x76, "i32.shr_u", i32_shr_u, 0)
INSTRUCTION(0x77, "i32.rotl", i32_rotl, 0)
INSTRUCTION(0x78, "i32.rotr", i32_rotr, 0)

INSTRUCTION(0x79, "i64.clz", i64_clz, 0)
INSTRUCTION(0x7a, "i64.ctz", i64_ctz, 0)
INSTRUCTION(0x7b, "i64.popcnt", i64_popcnt, 0)
INSTRUCTION(0x7c, "i64.add", i64_add, INSN_FLAG_EXTENDED_CONST)
INSTRUCTION(0x7d, "i64.sub", i64_sub, INSN_FLAG_EXTENDED_CONST)
INSTRUCTION(0x7e, "i64.mul", i64_mul, INSN_FLAG_EXTENDED_CONST)
INSTRUCTION(0x7f, "i64.div_s", i64_div_s, 0)
INSTRUCTION(0x80, "i64.div_u", i64_div_u, 0)
INSTRUCTION(0x81, "i64.rem_s", i64_rem_s, 0)
INSTRUCTION(0x82, "i64.rem_u", i64_rem_u, 0)
INSTRUCTION(0x83, "i64.and", i64_and, 0)
INSTRUCTION(0x84, "i64.or", i64_or, 0)
INSTRUCTION(0x85, "i64.xor", i64_xor, 0)
INSTRUCTION(0x86, "i64.shl", i64_shl, 0)
INSTRUCTION(0x87, "i64.shr_s", i64_shr_s, 0)
INSTRUCTION(0x88, "i64.shr_u", i64_shr_u, 0)
INSTRUCTION(0x89, "i64.rotl", i64_rotl, 0)
INSTRUCTION(0x8a, "i64.rotr", i64_rotr, 0)

INSTRUCTION(0x8b, "f32.abs", f32_abs, 0)
INSTRUCTION(0x8c, "f32.neg", f32_neg, 0)
INSTRUCTION(0x8d, "f32.ceil", f32_ceil, 0)
INSTRUCTION(0x8e, "f32.floor", f32_floor, 0)
INSTRUCTION(0x8f, "f32.trunc", f32_trunc, 0)
INSTRUCTION(0x90, "f32.nearest", f32_nearest, 0)
INSTRUCTION(0x91, "f32.sqrt", f32_sqrt, 0)

INSTRUCTION(0x92, "f32.add", f32_add, 0)
INSTRUCTION(0x93, "f32.sub", f32_sub, 0)
INSTRUCTION(0x94, "f32.mul", f32_mul, 0)
INSTRUCTION(0x95, "f32.div", f32_div, 0)
INSTRUCTION(0x96, "f32.min", f32_min, 0)
INSTRUCTION(0x97, "f32.max", f32_max, 0)
INSTRUCTION(0x98, "f32.copysign", f32_copysign, 0)

INSTRUCTION(0x99, "f64.abs", f64_abs, 0)
INSTRUCTION(0x9a, "f64.neg", f64_neg, 0)
INSTRUCTION(0x9b, "f64.ceil", f64_ceil, 0)
INSTRUCTION(0x9c, "f64.floor", f64_floor, 0)
INSTRUCTION(0x9d, "f64.trunc", f64_trunc, 0)
INSTRUCTION(0x9e, "f64.nearest", f64_nearest, 0)
INSTRUCTION(0x9f, "f64.sqrt", f64_sqrt, 0)

INSTRUCTION(0xa0, "f64.add", f64_add, 0)
INSTRUCTION(0xa1, "f64.sub", f64_sub, 0)
INSTRUCTION(0xa2, "f64.mul", f64_mul, 0)
INSTRUCTION(0xa3, "f64.div", f64_div, 0)
INSTRUCTION(0xa4, "f64.min", f64_min, 0)
INSTRUCTION(0xa5, "f64.max", f64_max, 0)
INSTRUCTION(0xa6, "f64.copysign", f64_copysign, 0)

INSTRUCTION(0xa7, "i32.wrap_i64", i32_wrap_i64, 0)

INSTRUCTION(0xa8, "i32.trunc_f32_s", i32_trunc_f32_s, 0)
INSTRUCTION(0xa9, "i32.trunc_f32_u", i32_trunc_f32_u, 0)
INSTRUCTION(0xaa, "i32.trunc_f64_s", i32_trunc_f64_s, 0)
INSTRUCTION(0xab, "i32.trunc_f64_u", i32_trunc_f64_u, 0)

INSTRUCTION(0xac, "i64.extend_i32_s", i64_extend_i32_s, 0)
INSTRUCTION(0xad, "i64.extend_i32_u", i64_extend_i32_u, 0)

INSTRUCTION(0xae, "i64.trunc_f32_s", i64_trunc_f32_s, 0)
INSTRUCTION(0xaf, "i64.trunc_f32_u", i64_trunc_f32_u, 0)
INSTRUCTION(0xb0, "i64.trunc_f64_s", i64_trunc_f64_s, 0)
INSTRUCTION(0xb1, "i64.trunc_f64_u", i64_trunc_f64_u, 0)

INSTRUCTION(0xb2, "f32.convert_i32_s", f32_convert_i32_s, 0)
INSTRUCTION(0xb3, "f32.convert_i32_u", f32_convert_i32_u, 0)
INSTRUCTION(0xb4, "f32.convert_i64_s", f32_convert_i64_s, 0)
INSTRUCTION(0xb5, "f32.convert_i64_u", f32_convert_i64_u, 0)
INSTRUCTION(0xb6, "f32.demote_f64", f32_demote_f64, 0)

INSTRUCTION(0xb7, "f64.convert_i32_s", f64_convert_i32_s, 0)
INSTRUCTION(0xb8, "f64.convert_i32_u", f64_convert_i32_u, 0)
INSTRUCTION(0xb9, "f64.convert_i64_s", f64_convert_i64_s, 0)
INSTRUCTION(0xba, "f64.convert_i64_u", f64_convert_i64_u, 0)
INSTRUCTION(0xbb, "f64.promote_f32", f64_promote_f32, 0)

INSTRUCTION(0xbc, "i32.reinterpret_f32", i32_reinterpret_f32, 0)
INSTRUCTION(0xbd, "i64.reinterpret_f64", i64_reinterpret_f64, 0)
INSTRUCTION(0xbe, "f32.reinterpret_i32", f32_reinterpret_i32, 0)
INSTRUCTION(0xbf, "f64.reinterpret_i64", f64_reinterpret_i64, 0)

INSTRUCTION(0xc0, "i32.extend8_s", i32_extend8_s, 0)
INSTRUCTION(0xc1, "i32.extend16_s", i32_extend16_s, 0)
INSTRUCTION(0xc2, "i64.extend8_s", i64_extend8_s, 0)
INSTRUCTION(0xc3, "i64.extend16_s", i64_extend16_s, 0)
INSTRUCTION(0xc4, "i64.extend32_s", i64_extend32_s, 0)

INSTRUCTION(0xd0, "ref.null", ref_null, INSN_FLAG_CONST)
INSTRUCTION(0xd1, "ref.is_null", ref_is_null, 0)
INSTRUCTION(0xd2, "ref.func", ref_func, INSN_FLAG_CONST)

INSTRUCTION_INDIRECT(0xfc, fc)
#if defined(TOYWASM_ENABLE_WASM_THREADS)
INSTRUCTION_INDIRECT(0xfe, fe)
#endif

/* clang-format on */
