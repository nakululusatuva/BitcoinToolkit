#include "../common.h"
#include "CLinkedlist.h"
#include <stdlib.h>
#include <string.h>

CLinkedlist * new_CLinkedlist()
{
	CLinkedlist *new = (CLinkedlist *)calloc(1, sizeof(CLinkedlist));
	if (new == NULL)
		return NULL;

	new->head = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));
	if (new->head == NULL)
		return NULL;

	new->head->previous = NULL;
	new->head->data = NULL;
	new->head->next = NULL;

	new->length = 0;

	new->CLinkedlist_add                 = &CLinkedlist_add;
	new->CLinkedlist_delete              = &CLinkedlist_delete;
	new->CLinkedlist_insert              = &CLinkedlist_insert;
	new->CLinkedlist_change              = &CLinkedlist_change;
	new->CLinkedlist_forward_traversing  = &CLinkedlist_forward_traversing;
	new->CLinkedlist_backward_traversing = &CLinkedlist_backward_traversing;
	new->CLinkedlist_reverse             = &CLinkedlist_reverse;
	new->CLinkedlist_is_empty            = &CLinkedlist_is_empty;

	return new;
}

bool delete_CLinkedlist(CLinkedlist *self)
{
	if (CLinkedlist_is_empty(self))
	{
		free(self->head);
		free(self);
		return true;
	}

	else
	{
		CLinkedlistNode **nodes = (CLinkedlistNode **)calloc(self->length, sizeof(CLinkedlistNode *));
		if (nodes == NULL)
			return false;

		for (uint64_t i = 0; i < self->length; ++i)
			nodes[i] = CLinkedlist_specific_node(self, i);
		for (uint64_t i = 0; i < self->length; ++i)
			free(nodes[i]);

		free(self->head);
		free(self);

		return true;
	}
}

CLinkedlistNode * CLinkedlist_last_node(CLinkedlist *self)
{
	if (CLinkedlist_is_empty(self))
		return NULL;

	CLinkedlistNode *buffer = self->head->next;
	while (1)
	{
		if (buffer->next != NULL)
		{
			buffer = buffer->next;
			continue;
		}
		else return buffer;
	}
}

CLinkedlistNode * CLinkedlist_specific_node(CLinkedlist *self, uint64_t index)
{
	if (CLinkedlist_is_empty(self))
		return NULL;

	uint64_t i;
	CLinkedlistNode *buffer = self->head->next;
	for (i = 0; i < self->length; ++i)
	{
		if (i == index)
			break;
		else buffer = buffer->next;
	}

	if (i >= self->length)
		return NULL;
	else return buffer;
}

void CLinkedlist_add(CLinkedlist *self, void *data)
{
	CLinkedlistNode *last = CLinkedlist_last_node(self);

	if (last == NULL)
		last = self->head;

	CLinkedlistNode *new  = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));

	last->next = new;
	new->previous = last;
	new->data = data;
	new->next = NULL;

	self->length++;
}

bool CLinkedlist_delete(CLinkedlist *self, uint64_t index)
{
	CLinkedlistNode *target = CLinkedlist_specific_node(self, index);

	// Index out range or empty linked list.
	if (target == NULL)
		return false;

	// The only node.
	else if (target->previous->previous == NULL && target->next == NULL)
	{
		self->head->next = NULL;
		free(target);
		self->length--;
		return true;
	}

	// The first node.
	else if (target->previous->previous == NULL && target->next != NULL)
	{
		self->head->next = target->next;
		target->next->previous = self->head;
		free(target);
		self->length--;
		return true;
	}

	// The last node.
	else if (target->previous->previous != NULL && target->next == NULL)
	{
		target->previous->next = NULL;
		self->length--;
		free(target);
		return true;
	}

	// Any where else
	else
	{
		target->previous->next = target->next;
		target->next->previous = target->previous;
		free(target);
		self->length--;
		return true;
	}
}

bool CLinkedlist_insert(CLinkedlist *self, uint64_t previous, void *data)
{
	CLinkedlistNode *pre = CLinkedlist_specific_node(self, previous);

	// Index out range or empty linked list.
	if (pre == NULL)
		return false;

	// Only one node in the linked list.
	else if (pre->previous->previous == NULL && pre->next == NULL)
	{
		CLinkedlistNode *node = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));

		pre->next      = node;
		node->previous = pre;
		node->data     = data;
		node->next     = NULL;

		self->length++;
		return true;
	}

	// The first node.
	else if (pre->previous->previous == NULL && pre->next != NULL)
	{
		CLinkedlistNode *node = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));

		node->previous      = pre;
		node->data          = data;
		node->next          = pre->next;
		pre->next->previous = node;
		pre->next           = node;

		self->length++;
		return true;
	}

	// The last node.
	else if (pre->previous->previous != NULL && pre->next == NULL)
	{
		CLinkedlistNode *node = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));

		node->previous = pre;
		node->data     = data;
		node->next     = NULL;
		pre->next      = node;

		self->length++;
		return true;
	}

	// Any where else (Procedures are as same as the last node).
	else
	{
		CLinkedlistNode *node = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));

		node->previous      = pre;
		node->data          = data;
		node->next          = pre->next;
		pre->next->previous = node;
		pre->next           = node;

		self->length++;
		return true;
	}
}

bool CLinkedlist_change(CLinkedlist *self, uint64_t index, void *data)
{
	CLinkedlistNode *target = CLinkedlist_specific_node(self, index);

	// Index out range or empty linked list.
	if (target == NULL)
		return false;

	else
	{
		target->data = data;
		return true;
	}
}

void ** CLinkedlist_forward_traversing(CLinkedlist *self)
{
	if (CLinkedlist_is_empty(self))
		return NULL;

	CLinkedlistNode *start = self->head->next;
	void **list = (void **)calloc(self->length, sizeof(void *));
	if (list == NULL)
		return NULL;

	for (uint64_t i = 0; i < self->length; ++i)
	{
		list[i] = start->data;
		start = start->next;
	}

	return list;
}

void ** CLinkedlist_backward_traversing(CLinkedlist *self)
{
	if (CLinkedlist_is_empty(self))
		return NULL;

	CLinkedlistNode *last = CLinkedlist_last_node(self);
	void **list = (void **)calloc(self->length, sizeof(void *));
	if (list == NULL)
		return NULL;

	for (uint64_t i = self->length; i >= 1; --i)
	{
		list[self->length - i] = last->data;
		last = last->previous;
	}

	return list;
}

bool CLinkedlist_reverse(CLinkedlist *self)
{
	CLinkedlistNode *last = CLinkedlist_last_node(self);
	if (last == NULL)
		return false;

	CLinkedlistNode **nodes = (CLinkedlistNode **)calloc(self->length, sizeof(CLinkedlistNode *));
	if (nodes == NULL)
		return false;

	for (uint64_t i = self->length; i >= 1; --i)
	{
		nodes[i] = last;
		last = last->previous;
	}

	CLinkedlistNode *buffer = self->head;
	for (uint64_t i = 0; i < self->length; ++i)
	{
		buffer->next = nodes[i];
		nodes[i]->previous = buffer;
		if (i == self->length - 1)
			nodes[i]->next = NULL;
		buffer = nodes[i];
	}

	return true;
}

bool CLinkedlist_is_empty(CLinkedlist *self)
{
	if (self->length <= 0 && self->head->next == NULL)
		return false;
	else return true;
}