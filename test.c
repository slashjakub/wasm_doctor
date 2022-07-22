
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <cmocka.h>

#include "endian.h"
#include "leb128.h"

#define TEST_OK(type, encoded_bytes, expected_value)                          \
        p = encoded_bytes;                                                    \
        ep = p + sizeof(encoded_bytes);                                       \
        ret = read_leb_##type(&p, ep, &type);                                 \
        assert_int_equal(ret, 0);                                             \
        assert_ptr_equal(p, ep);                                              \
        assert_int_equal(type, expected_value)

#define TEST_E2BIG(type, encoded_bytes)                                       \
        p = op = encoded_bytes;                                               \
        ep = p + sizeof(encoded_bytes);                                       \
        ret = read_leb_##type(&p, ep, &type);                                 \
        assert_int_equal(ret, E2BIG);                                         \
        assert_ptr_equal(p, op)

#define TEST_BITS_OK(type, bits, encoded_bytes, expected_value)               \
        p = encoded_bytes;                                                    \
        ep = p + sizeof(encoded_bytes);                                       \
        ret = read_leb_##type(&p, ep, bits, &type##64);                       \
        assert_int_equal(ret, 0);                                             \
        assert_ptr_equal(p, ep);                                              \
        assert_int_equal(type##64, expected_value)

#define TEST_BITS_E2BIG(type, bits, encoded_bytes)                            \
        p = op = encoded_bytes;                                               \
        ep = p + sizeof(encoded_bytes);                                       \
        ret = read_leb_##type(&p, ep, bits, &type##64);                       \
        assert_int_equal(ret, E2BIG);                                         \
        assert_ptr_equal(p, op)

void
test_leb128(void **state)
{
        uint64_t u64;
        uint32_t u32;
        uint32_t i32;
        int32_t s32;
        int64_t s64;
        uint64_t i64;
        const uint8_t *p;
        const uint8_t *op;
        const uint8_t *ep;
        int ret;

        /* https://en.wikipedia.org/wiki/LEB128#Unsigned_LEB128 */
        const uint8_t u_624485[] = {
                0xe5,
                0x8e,
                0x26,
        };

        TEST_OK(u32, u_624485, 624485);
        TEST_OK(s32, u_624485, 624485);
        TEST_OK(u64, u_624485, 624485);
        TEST_OK(s64, u_624485, 624485);

        TEST_BITS_OK(u, 64, u_624485, 624485);
        TEST_BITS_OK(u, 20, u_624485, 624485);
        TEST_BITS_E2BIG(u, 19, u_624485);
        TEST_BITS_E2BIG(u, 1, u_624485);

        TEST_BITS_OK(s, 64, u_624485, 624485);
        TEST_BITS_OK(s, 21, u_624485, 624485);
        TEST_BITS_E2BIG(s, 20, u_624485);
        TEST_BITS_E2BIG(s, 1, u_624485);

        /* https://en.wikipedia.org/wiki/LEB128#Signed_LEB128 */
        const uint8_t s_minus123456[] = {
                0xc0,
                0xbb,
                0x78,
        };

        TEST_OK(u32, s_minus123456, 0x1e1dc0);
        TEST_OK(s32, s_minus123456, -123456);
        TEST_OK(u64, s_minus123456, 0x1e1dc0);
        TEST_OK(s64, s_minus123456, -123456);

        TEST_BITS_OK(s, 64, s_minus123456, -123456);
        TEST_BITS_OK(s, 18, s_minus123456, -123456);
        TEST_BITS_E2BIG(s, 17, s_minus123456);
        TEST_BITS_E2BIG(s, 1, s_minus123456);

        const uint8_t u_0x100000000[] = {
                0x80, 0x80, 0x80, 0x80, 0x10,
        };
        const uint8_t u_0xffffffff[] = {
                0xff, 0xff, 0xff, 0xff, 0x0f,
        };
        const uint8_t s_minus1[] = {
                0x7f,
        };
        const uint8_t s_minus0x80000000[] = {
                0x80, 0x80, 0x80, 0x80, 0x78,
        };
        const uint8_t s_minus0x80000001[] = {
                0xff, 0xff, 0xff, 0xff, 0x77,
        };
        const uint8_t u_0x7fffffff[] = {
                0xff, 0xff, 0xff, 0xff, 0x07,
        };
        const uint8_t u_0x80000000[] = {
                0x80, 0x80, 0x80, 0x80, 0x08,
        };

        TEST_OK(u32, u_0xffffffff, 0xffffffff);
        TEST_E2BIG(s32, u_0xffffffff);
        TEST_OK(u64, u_0xffffffff, 0xffffffff);
        TEST_OK(s64, u_0xffffffff, 0xffffffff);

        TEST_OK(u32, u_0x7fffffff, 0x7fffffff);
        TEST_OK(s32, u_0x7fffffff, 0x7fffffff);
        TEST_OK(u64, u_0x7fffffff, 0x7fffffff);
        TEST_OK(s64, u_0x7fffffff, 0x7fffffff);

        TEST_OK(u32, u_0x80000000, 0x80000000);
        TEST_E2BIG(s32, u_0x80000000);
        TEST_OK(u64, u_0x80000000, 0x80000000);
        TEST_OK(s64, u_0x80000000, 0x80000000);

        TEST_E2BIG(u32, u_0x100000000);
        TEST_E2BIG(s32, u_0x100000000);
        TEST_OK(u64, u_0x100000000, 0x100000000);
        TEST_OK(s64, u_0x100000000, 0x100000000);

        TEST_OK(u32, s_minus1, 0x7f);
        TEST_OK(s32, s_minus1, -1);
        TEST_OK(u64, s_minus1, 0x7f);
        TEST_OK(s64, s_minus1, -1);

        TEST_E2BIG(u32, s_minus0x80000000);
        TEST_OK(s32, s_minus0x80000000, -(int64_t)0x80000000);
        TEST_OK(u64, s_minus0x80000000, 0x780000000);
        TEST_OK(s64, s_minus0x80000000, -(int64_t)0x80000000);

        TEST_E2BIG(u32, s_minus0x80000001);
        TEST_E2BIG(s32, s_minus0x80000001);
        TEST_OK(u64, s_minus0x80000001, 0x77fffffff);
        TEST_OK(s64, s_minus0x80000001, -(int64_t)0x80000001);

        const uint8_t u_0x10000000000000000[] = {
                0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x02,
        };

        TEST_E2BIG(u32, u_0x10000000000000000);
        TEST_E2BIG(s32, u_0x10000000000000000);
        TEST_E2BIG(u64, u_0x10000000000000000);
        TEST_E2BIG(s64, u_0x10000000000000000);

        const uint8_t u_0xffffffffffffffff[] = {
                0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01,
        };

        TEST_E2BIG(u32, u_0xffffffffffffffff);
        TEST_E2BIG(s32, u_0xffffffffffffffff);
        TEST_OK(u64, u_0xffffffffffffffff, 0xffffffffffffffff);
        TEST_E2BIG(s64, u_0xffffffffffffffff);

        /*
         * Uninterpreted integers are encoded as signed.
         */
        const uint8_t i_2155905152_s[] = {
                0x80, 0x81, 0x82, 0x84, 0x78,
        };

        TEST_OK(i32, i_2155905152_s, 2155905152);
        TEST_E2BIG(u32, i_2155905152_s);
        TEST_OK(s32, i_2155905152_s, (int32_t)2155905152);

        const uint8_t i_2155905152_u[] = {
                0x80, 0x81, 0x82, 0x84, 0x08,
        };

        TEST_E2BIG(i32, i_2155905152_u);
        TEST_OK(u32, i_2155905152_u, 2155905152);
        TEST_E2BIG(s32, i_2155905152_u);

        /*
         * Redundant representations are quite common in
         * actual wasm files.
         */

        const uint8_t u_5_redundant[] = {
                0x85, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
        };

        TEST_OK(i32, u_5_redundant, 5);
        TEST_OK(u32, u_5_redundant, 5);
        TEST_OK(s32, u_5_redundant, 5);

        const uint8_t s_minus5_redundant[] = {
                0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f,
        };

        TEST_OK(i32, s_minus5_redundant, (uint32_t)-5);
        TEST_E2BIG(u32, s_minus5_redundant);
        TEST_OK(s32, s_minus5_redundant, -5);

        /* some special values for blocktype */
        const uint8_t s33_x40[] = {
                0x40,
        };
        const uint8_t s33_x7f[] = {
                0x7f,
        };
        const uint8_t s33_x6f[] = {
                0x6f,
        };
        TEST_BITS_OK(s, 33, s33_x40, -64);
        TEST_BITS_OK(s, 33, s33_x7f, -1);
        TEST_BITS_OK(s, 33, s33_x6f, -17);

        const uint8_t s64_min[] = {
                0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x7f,
        };
        TEST_OK(i64, s64_min, INT64_MIN);
}

void
test_endian(void **state)
{
        static const uint8_t v64[] = {
                0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
        };
        assert_int_equal(0xfedcba9876543210, le64_to_host(*(uint64_t *)v64));

        uint8_t buf64[8];
        *(uint64_t *)buf64 = host_to_le64(0xfedcba9876543210);
        assert_memory_equal(buf64, v64, 8);

        static const uint8_t v32[] = {
                0xaa,
                0xbb,
                0xcc,
                0xdd,
        };
        assert_int_equal(0xddccbbaa, le32_to_host(*(uint32_t *)v32));

        uint8_t buf32[4];
        *(uint32_t *)buf32 = host_to_le32(0xddccbbaa);
        assert_memory_equal(buf32, v32, 4);

        static const uint8_t v16[] = {
                0x11,
                0x92,
        };
        assert_int_equal(0x9211, le16_to_host(*(uint16_t *)v16));

        uint8_t buf16[2];
        *(uint16_t *)buf16 = host_to_le16(0x9211);
        assert_memory_equal(buf16, v16, 2);

        assert_int_equal(0x11, le8_to_host(0x11));
        assert_int_equal(0x00, le8_to_host(0x00));
        assert_int_equal(0xff, le8_to_host(0xff));
        assert_int_equal(0x11, host_to_le8(0x11));
        assert_int_equal(0x00, host_to_le8(0x00));
        assert_int_equal(0xff, host_to_le8(0xff));

        uint8_t buf[10];
        uint8_t poison;
        uint64_t v;

        poison = 0xa1;
        v = 0x1122334455667788;
        memset(buf, poison, sizeof(buf));
        le64_encode(buf, v);
        assert_int_equal(buf[0], v & 0xff);
        assert_int_equal(buf[1], (v >> 1 * 8) & 0xff);
        assert_int_equal(buf[2], (v >> 2 * 8) & 0xff);
        assert_int_equal(buf[3], (v >> 3 * 8) & 0xff);
        assert_int_equal(buf[4], (v >> 4 * 8) & 0xff);
        assert_int_equal(buf[5], (v >> 5 * 8) & 0xff);
        assert_int_equal(buf[6], (v >> 6 * 8) & 0xff);
        assert_int_equal(buf[7], v >> 7 * 8);
        assert_int_equal(buf[8], poison);
        assert_int_equal(buf[9], poison);
        assert_int_equal(v, le64_decode(buf));

        poison = 0xee;
        v = 0xaabbccdd;
        memset(buf, poison, sizeof(buf));
        le32_encode(buf, v);
        assert_int_equal(buf[0], v & 0xff);
        assert_int_equal(buf[1], (v >> 1 * 8) & 0xff);
        assert_int_equal(buf[2], (v >> 2 * 8) & 0xff);
        assert_int_equal(buf[3], (v >> 3 * 8) & 0xff);
        assert_int_equal(buf[4], poison);
        assert_int_equal(buf[5], poison);
        assert_int_equal(buf[6], poison);
        assert_int_equal(buf[7], poison);
        assert_int_equal(buf[8], poison);
        assert_int_equal(buf[9], poison);
        assert_int_equal(v, le32_decode(buf));

        poison = 0x11;
        v = 0xabcd;
        memset(buf, poison, sizeof(buf));
        le16_encode(buf, v);
        assert_int_equal(buf[0], v & 0xff);
        assert_int_equal(buf[1], (v >> 1 * 8) & 0xff);
        assert_int_equal(buf[2], poison);
        assert_int_equal(buf[3], poison);
        assert_int_equal(buf[4], poison);
        assert_int_equal(buf[5], poison);
        assert_int_equal(buf[6], poison);
        assert_int_equal(buf[7], poison);
        assert_int_equal(buf[8], poison);
        assert_int_equal(buf[9], poison);
        assert_int_equal(v, le16_decode(buf));

        poison = 0xdc;
        v = 0x77;
        memset(buf, poison, sizeof(buf));
        le8_encode(buf, v);
        assert_int_equal(buf[0], v & 0xff);
        assert_int_equal(buf[1], poison);
        assert_int_equal(buf[2], poison);
        assert_int_equal(buf[3], poison);
        assert_int_equal(buf[4], poison);
        assert_int_equal(buf[5], poison);
        assert_int_equal(buf[6], poison);
        assert_int_equal(buf[7], poison);
        assert_int_equal(buf[8], poison);
        assert_int_equal(buf[9], poison);
        assert_int_equal(v, le8_decode(buf));
}

int
main(int argc, char **argv)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(test_leb128),
                cmocka_unit_test(test_endian),
        };
        return cmocka_run_group_tests(tests, NULL, NULL);
}
