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

	new->add                 = &CLinkedlist_add;
	new->delete              = &CLinkedlist_delete;
	new->insert              = &CLinkedlist_insert;
	new->change              = &CLinkedlist_change;
	new->forward_traversing  = &CLinkedlist_forward_traversing;
	new->backward_traversing = &CLinkedlist_backward_traversing;
	new->reverse             = &CLinkedlist_reverse;
	new->is_empty            = &CLinkedlist_is_empty;

	return new;
}

bool delete_CLinkedlist(CLinkedlist *self)
{
	// Check if empty.
	if (CLinkedlist_is_empty(self))
	{
		free(self->head);
		free(self);
		return true;
	}

	else
	{	// Delete all nodes.
		CLinkedlistNode *start = self->head->next;

		while (1)
		{
			if (start->next != NULL)
			{	// If not the last one.
				start = start->next;
				free(start->previous);
			}
			else if (start->next == NULL)
			{	// If the last one.
				free(start);
				break;
			}
		}
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
	// Check if empty or index out range.
	if (CLinkedlist_is_empty(self) || index < 0 || index >= self->length)
		return NULL;

	uint64_t i;
	CLinkedlistNode *buffer = self->head->next;
	for (i = 0; i < self->length; ++i)
	{
		if (i == index)
			break;
		else buffer = buffer->next;
	}

	return buffer;
}

void CLinkedlist_add(CLinkedlist *self, void *data)
{
	CLinkedlistNode *last = CLinkedlist_last_node(self);
	// If empty, the last one will be the head.
	if (last == NULL)
		last = self->head;

	CLinkedlistNode *new  = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));

	// Re-link the nodes.
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

	// Any where else (Procedures are as same as the first node).
	// (target->previous->previous != NULL && target->next != NULL)
	else
	{
		target->previous->next = target->next;
		target->next->previous = target->previous;
		free(target);
		self->length--;
		return true;
	}
}

bool CLinkedlist_insert(CLinkedlist *self, uint64_t after, void *data)
{
	CLinkedlistNode *new_node   = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));
	CLinkedlistNode *after_node = CLinkedlist_specific_node(self, after);

	// Check if index out range or empty linked list.
	if (after_node == NULL)
	{
		free(new_node);
		return NULL;
	}
	// Always between two nodes, or between head node and a normal node.
	else
	{// Re-link the nodes, length+1
		self->length++;

		after_node->previous->next = new_node;
		new_node->previous = after_node->previous;

		new_node->data = data;

		new_node->next = after_node;
		after_node->previous = new_node;

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

	// Actually it return a pointer-array that store the data's pointers.
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

	// Actually it return a pointer-array that store the data's pointers.
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
	// Check if empty.
	CLinkedlistNode *last = CLinkedlist_last_node(self);
	if (last == NULL)
		return false;

	CLinkedlistNode **nodes = (CLinkedlistNode **)calloc(self->length, sizeof(CLinkedlistNode *));
	if (nodes == NULL)
		return false;

	// Get the nodes' pointer and store in a pointer-array (reversed).
	for (uint64_t i = self->length; i >= 1; --i)
	{
		nodes[self->length - i] = last;
		last = last->previous;
	}

	// Re-link nodes, follow the pointer-array.
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
	if (self->length <= 0 || self->head->next == NULL)
		return true;
	else return false;
}