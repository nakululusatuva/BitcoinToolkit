#include <check.h>
#include "../src/script/script.h"

BYTE sample1[28] = {0x04,0xff,0xaa,0xdd,0xee,0xa9,0x14,0xf3,0x70,0x78,0xa5,\
0x3a,0xf0,0x2c,0xe0,0x81,0x79,0xfe,0x34,0x6a,0x54,0x0a,0x0a,0x62,0x56,0x70,0x07,0x87};
BYTE sample2[6] = {0x4d,0x03,0x00,0x00,0xff,0xff};
const char *sample1_str = "PUSHDATA(0x04)[FFAADDEE] OP_HASH160 PUSHDATA(0x14)[F37078A53AF02CE08179FE346A540A0A62567007] OP_EQUAL";
const char *sample2_str = "OP_PUSHDATA2[00FFFF]";

START_TEST(script_new_from_bytes_and_to_bytes)
{
	Script *script1 = new_Script_from_bytes(sample1, 28);
	ck_assert_ptr_ne(script1, NULL);
	size_t bytes_size1;
	BYTE *bytes1 = script1->to_bytes(script1, &bytes_size1);
	ck_assert_ptr_ne(bytes1, NULL);
	ck_assert_uint_eq(bytes_size1, 28);
	for (uint8_t i = 0; i < bytes_size1; ++i)
	{
		ck_assert_uint_eq(bytes1[i], sample1[i]);
	}

	Script *script2 = new_Script_from_bytes(sample2, 6);
	ck_assert_ptr_ne(script2, NULL);
	size_t bytes_size2;
	BYTE *bytes2 = script2->to_bytes(script2, &bytes_size2);
	ck_assert_ptr_ne(bytes2, NULL);
	ck_assert_uint_eq(bytes_size2, 6);
	for (uint8_t i = 0; i < bytes_size2; ++i)
	{
		ck_assert_uint_eq(bytes2[i], sample2[i]);
	}
}
END_TEST

START_TEST(script_to_string)
{
	Script *script1 = new_Script_from_bytes(sample1, 28);
	size_t size1;
	char *str1 = (char *)script1->to_string(script1, &size1);
	for (uint8_t i = 0; i < size1; ++i)
	{
		ck_assert_int_eq(str1[i], sample1_str[i]);
	}
}
END_TEST

Suite * make_Script_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("Script");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, script_new_from_bytes_and_to_bytes);
	tcase_add_test(tc_core, script_to_string);
	suite_add_tcase(s, tc_core);

	return s;
}