#include <check.h>
#include <string.h>
#include <stdlib.h>
#include "../src/container/CLinkedlist.h"

START_TEST(clinkedlist_add_and_is_empty)
{
	CLinkedlist *list = new_CLinkedlist();
	ck_assert(list->is_empty(list));

	BYTE *data1 = (BYTE *)malloc(10);
	ck_assert(list->add(list, data1, 10));
	ck_assert_ptr_eq(list->head->next->data, data1);
	ck_assert(!(list->is_empty(list)));

	delete_CLinkedlist(list);
}
END_TEST

START_TEST(clinkedlist_delete)
{
	CLinkedlist *list = new_CLinkedlist();
	BYTE *data1 = (BYTE *)malloc(10);
	list->add(list, data1, 10);

	list->delete(list, 0);
	ck_assert_ptr_eq(list->head->next, NULL);
	ck_assert(list->is_empty(list));

	delete_CLinkedlist(list);
	free(data1);
}
END_TEST

START_TEST(clinkedlist_insert)
{
	CLinkedlist *list1 = new_CLinkedlist();
	BYTE *data1 = (BYTE *)malloc(10);
	BYTE *data2 = (BYTE *)malloc(10);
	BYTE *data3 = (BYTE *)malloc(10);
	list1->add(list1, data1, 10);
	list1->add(list1, data3, 10);
	list1->insert(list1, 1, data2, 10);
	ck_assert_ptr_eq(list1->head->next->data, data1);
	ck_assert_ptr_eq(list1->head->next->next->data, data2);
	ck_assert_ptr_eq(list1->head->next->next->next->data, data3);

	CLinkedlist *list2 = new_CLinkedlist();
	BYTE *data4 = (BYTE *)malloc(10);
	BYTE *data5 = (BYTE *)malloc(10);
	BYTE *data6 = (BYTE *)malloc(10);
	list2->add(list2, data5, 10);
	list2->add(list2, data6, 10);
	list2->insert(list2, 0, data4, 10);
	ck_assert_ptr_eq(list2->head->next->data, data4);
	ck_assert_ptr_eq(list2->head->next->next->data, data5);
	ck_assert_ptr_eq(list2->head->next->next->next->data, data6);

	delete_CLinkedlist(list1);
	delete_CLinkedlist(list2);
}
END_TEST

START_TEST(clinkedlist_change)
{
	CLinkedlist *list = new_CLinkedlist();
	BYTE *data1 = (BYTE *)malloc(10);
	BYTE *data2 = (BYTE *)malloc(10);
	list->add(list, data1, 10);
	ck_assert(list->change(list, 0, data2, 10));
	ck_assert_ptr_eq(list->head->next->data, data2);
	delete_CLinkedlist(list);
}
END_TEST

START_TEST(clinkedlist_forward_traversing)
{
	CLinkedlist *list = new_CLinkedlist();
	BYTE *data1 = (BYTE *)malloc(10);
	BYTE *data2 = (BYTE *)malloc(10);
	BYTE *data3 = (BYTE *)malloc(10);
	list->add(list, data1, 10);
	list->add(list, data2, 10);
	list->add(list, data3, 10);
	CLinkedlistNode *node1 = list->head->next;
	CLinkedlistNode *node2 = list->head->next->next;
	CLinkedlistNode *node3 = list->head->next->next->next;
	CLinkedlistNode **nodes = list->forward_traversing(list);
	ck_assert_ptr_eq(nodes[0], node1);
	ck_assert_ptr_eq(nodes[1], node2);
	ck_assert_ptr_eq(nodes[2], node3);
	ck_assert_ptr_eq(nodes[0]->data, data1);
	ck_assert_ptr_eq(nodes[1]->data, data2);
	ck_assert_ptr_eq(nodes[2]->data, data3);
	delete_CLinkedlist(list);
}
END_TEST

START_TEST(clinkedlist_backward_traversing)
{
	CLinkedlist *list = new_CLinkedlist();
	BYTE *data1 = (BYTE *)malloc(10);
	BYTE *data2 = (BYTE *)malloc(10);
	BYTE *data3 = (BYTE *)malloc(10);
	list->add(list, data1, 10);
	list->add(list, data2, 10);
	list->add(list, data3, 10);
	CLinkedlistNode *node1 = list->head->next;
	CLinkedlistNode *node2 = list->head->next->next;
	CLinkedlistNode *node3 = list->head->next->next->next;
	CLinkedlistNode **nodes = list->backward_traversing(list);
	ck_assert_ptr_eq(nodes[0], node3);
	ck_assert_ptr_eq(nodes[1], node2);
	ck_assert_ptr_eq(nodes[2], node1);
	ck_assert_ptr_eq(nodes[0]->data, data3);
	ck_assert_ptr_eq(nodes[1]->data, data2);
	ck_assert_ptr_eq(nodes[2]->data, data1);
	delete_CLinkedlist(list);
}
END_TEST

START_TEST(clinkedlist_reverse)
{
	CLinkedlist *list = new_CLinkedlist();
	BYTE *data1 = (BYTE *)malloc(10);
	BYTE *data2 = (BYTE *)malloc(10);
	BYTE *data3 = (BYTE *)malloc(10);
	list->add(list, data1, 10);
	list->add(list, data2, 10);
	list->add(list, data3, 10);
	CLinkedlistNode *node1 = list->head->next;
	CLinkedlistNode *node2 = list->head->next->next;
	CLinkedlistNode *node3 = list->head->next->next->next;
	list->reverse(list);
	ck_assert_ptr_eq(list->head->next, node3);
	ck_assert_ptr_eq(list->head->next->next, node2);
	ck_assert_ptr_eq(list->head->next->next->next, node1);
	ck_assert_ptr_eq(list->head->next->data, data3);
	ck_assert_ptr_eq(list->head->next->next->data, data2);
	ck_assert_ptr_eq(list->head->next->next->next->data, data1);
	delete_CLinkedlist(list);
}
END_TEST

START_TEST(clinkedlist_total_size)
{
	CLinkedlist *list = new_CLinkedlist();
	ck_assert_uint_eq(list->total_size(list), 0);
	BYTE *data1 = (BYTE *)malloc(10);
	BYTE *data2 = (BYTE *)malloc(10);
	BYTE *data3 = (BYTE *)malloc(10);
	list->add(list, data1, 10);
	ck_assert_uint_eq(list->total_size(list), 10);
	list->add(list, data2, 10);
	ck_assert_uint_eq(list->total_size(list), 20);
	list->add(list, data3, 10);
	ck_assert_uint_eq(list->total_size(list), 30);
	delete_CLinkedlist(list);
}
END_TEST

START_TEST(clinkedlist_last_node)
{
	CLinkedlist *list = new_CLinkedlist();
	BYTE *data1 = (BYTE *)malloc(10);
	BYTE *data2 = (BYTE *)malloc(10);
	list->add(list, data1, 10);
	CLinkedlist *last1 = list->last_node(list);
	ck_assert_ptr_eq(list->head->next, last1);
	ck_assert_ptr_eq(list->head->next->data, data1);
	list->add(list, data2, 10);
	CLinkedlist *last2 = list->last_node(list);
	ck_assert_ptr_eq(list->head->next->next, last2);
	ck_assert_ptr_eq(list->head->next->next->data, data2);
	delete_CLinkedlist(list);
}
END_TEST

START_TEST(clinkedlist_specific_node)
{
	CLinkedlist *list = new_CLinkedlist();
	BYTE *data1 = (BYTE *)malloc(10);
	BYTE *data2 = (BYTE *)malloc(10);
	BYTE *data3 = (BYTE *)malloc(10);
	list->add(list, data1, 10);
	list->add(list, data2, 10);
	list->add(list, data3, 10);
	CLinkedlistNode *node1 = list->head->next;
	CLinkedlistNode *node2 = list->head->next->next;
	CLinkedlistNode *node3 = list->head->next->next->next;
	ck_assert_ptr_eq(list->specific_node(list, 0), node1);
	ck_assert_ptr_eq(list->specific_node(list, 1), node2);
	ck_assert_ptr_eq(list->specific_node(list, 2), node3);
	delete_CLinkedlist(list);
}
END_TEST

Suite * make_CLinkedlist_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("CLinkedlist");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, clinkedlist_add_and_is_empty);
	tcase_add_test(tc_core, clinkedlist_delete);
	tcase_add_test(tc_core, clinkedlist_insert);
	tcase_add_test(tc_core, clinkedlist_change);
	tcase_add_test(tc_core, clinkedlist_forward_traversing);
	tcase_add_test(tc_core, clinkedlist_backward_traversing);
	tcase_add_test(tc_core, clinkedlist_reverse);
	tcase_add_test(tc_core, clinkedlist_total_size);
	tcase_add_test(tc_core, clinkedlist_last_node);
	tcase_add_test(tc_core, clinkedlist_specific_node);
	suite_add_tcase(s, tc_core);

	return s;
}