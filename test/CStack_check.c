#include <check.h>
#include <string.h>
#include <stdlib.h>
#include "../src/container/CStack.h"

START_TEST(stack_push_and_pop)
{
	CStack *stack = new_CStack(1);
	BYTE *data = (BYTE *)malloc(2);
	data[0] = 0xaa;
	data[1] = 0xff;

	stack->push(stack, data, 2);
	size_t popped_size;
	BYTE *popped = stack->pop(stack, &popped_size);

	ck_assert_ptr_eq(popped, data);
	ck_assert_uint_eq(popped[0], data[0]);
	ck_assert_uint_eq(popped[1], data[1]);
	ck_assert_uint_eq(popped_size, 2);

	delete_CStack(stack);
}
END_TEST

START_TEST(stack_is_empty_and_is_full)
{
	CStack *stack = new_CStack(2);
	ck_assert(stack->is_empty(stack));

	BYTE *data1 = (BYTE *)malloc(1);
	BYTE *data2 = (BYTE *)malloc(1);
	stack->push(stack, data1, 1);
	stack->push(stack, data2, 1);
	
	ck_assert(stack->is_full(stack));

	delete_CStack(stack);
	free(data1);
	free(data2);
}
END_TEST

START_TEST(stack_total_size)
{
	CStack *stack = new_CStack(2);
	ck_assert_uint_eq(stack->total_size(stack), 0);

	BYTE *data1 = (BYTE *)malloc(5);
	stack->push(stack, data1, 5);
	ck_assert_uint_eq(stack->total_size(stack), 5);

	BYTE *data2 = (BYTE *)malloc(10);
	stack->push(stack, data1, 10);
	ck_assert_uint_eq(stack->total_size(stack), 15);

	delete_CStack(stack);
	free(data1);
	free(data2);
}
END_TEST

Suite * make_CStack_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("CStack");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, stack_push_and_pop);
	tcase_add_test(tc_core, stack_is_empty_and_is_full);
	tcase_add_test(tc_core, stack_total_size);
	suite_add_tcase(s, tc_core);

	return s;
}