#include "internal/container/CLinkedlist.h"
#include <stdlib.h>
#include <string.h>

CLinkedlist * new_CLinkedlist()
{
	CLinkedlist *new = (CLinkedlist *)calloc(1, sizeof(CLinkedlist));
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;

	else
	{
		new->head = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));
		if (new->head == NULL)
		{
			free(new);
			return MEMORY_ALLOCATE_FAILED;
		}
	}

	new->head->previous = NULL;
	new->head->data = NULL;
	new->head->next = NULL;

	new->length = 0;

	new->add                 = &CLinkedlist_add;
	new->delete              = &CLinkedlist_delete;
	new->insert              = &CLinkedlist_insert;
	new->change              = &CLinkedlist_change;
	new->forward_iter  = &CLinkedlist_forward_iter;
	new->backward_iter = &CLinkedlist_backward_iter;
	new->reverse             = &CLinkedlist_reverse;
	new->is_empty            = &CLinkedlist_is_empty;
	new->total_size          = &CLinkedlist_total_size;
	new->last_node           = &CLinkedlist_last_node;
	new->get_node       = &CLinkedlist_get_node;
	new->get_length          = &CLinkedlist_get_length;

	return new;
}

void delete_CLinkedlist(CLinkedlist *self)
{
	// Check if empty.
	if (CLinkedlist_is_empty(self))
	{
		free(self->head);
		free(self);
	}

	else
	{	// Delete all nodes.
		CLinkedlistNode *start = self->head->next;

		while (true)
		{
			if (start->next != NULL)
			{	// If not the last one.
				free(start->data);
				start = start->next;
				free(start->previous);
			}
			else if (start->next == NULL)
			{	// If the last one.
				free(start->data);
				free(start);
				break;
			}
		}
		free(self->head);
		free(self);
	}
}

CLinkedlistNode * CLinkedlist_last_node(CLinkedlist *self)
{
	if (CLinkedlist_is_empty(self))
		return CLINKEDLIST_EMPTY;

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

CLinkedlistNode * CLinkedlist_get_node(CLinkedlist *self, uint64_t index)
{
	// Check if empty or index out range.
	if (CLinkedlist_is_empty(self))
		return CLINKEDLIST_EMPTY;
	else if (index < 0 || index >= self->length)
		return INDEX_OUT_RANGE;

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

Status CLinkedlist_add(CLinkedlist *self, void *data, size_t size, void *type)
{
	CLinkedlistNode *last = CLinkedlist_last_node(self);
	// If empty, the last one will be the head.
	if (last == CLINKEDLIST_EMPTY)
		last = self->head;

	CLinkedlistNode *new  = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;

	// Re-link the nodes.
	last->next = new;
	new->previous = last;
	new->data = data;
	new->size = size;
	new->type = type;
	new->next = NULL;

	self->length++;

	return SUCCEEDED;
}

Status CLinkedlist_delete(CLinkedlist *self, uint64_t index)
{
	CLinkedlistNode *target = CLinkedlist_get_node(self, index);

	// Index out range or empty linked list.
	if (target == CLINKEDLIST_EMPTY || target == INDEX_OUT_RANGE)
		return (void *)target;

	// The only node.
	else if (target->previous->previous == NULL && target->next == NULL)
	{
		self->head->next = NULL;
		free(target->data);
		free(target);
		self->length--;
		return SUCCEEDED;
	}

	// The first node.
	else if (target->previous->previous == NULL && target->next != NULL)
	{
		self->head->next = target->next;
		target->next->previous = self->head;
		free(target->data);
		free(target);
		self->length--;
		return SUCCEEDED;
	}

	// The last node.
	else if (target->previous->previous != NULL && target->next == NULL)
	{
		target->previous->next = NULL;
		self->length--;
		free(target->data);
		free(target);
		return SUCCEEDED;
	}

	// Any where else (Procedures are as same as the first node).
	// (target->previous->previous != NULL && target->next != NULL)
	else
	{
		target->previous->next = target->next;
		target->next->previous = target->previous;
		free(target->data);
		free(target);
		self->length--;
		return SUCCEEDED;
	}
}

Status CLinkedlist_insert(CLinkedlist *self, uint64_t after, void *data, size_t size, void *type)
{
	CLinkedlistNode *new_node   = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));
	if (new_node == NULL)
		return MEMORY_ALLOCATE_FAILED;

	CLinkedlistNode *after_node = CLinkedlist_get_node(self, after);

	// Check if index out range or empty linked list.
	if (after_node == INDEX_OUT_RANGE || after_node == CLINKEDLIST_EMPTY)
	{
		free(new_node);
		return (void *)after_node;
	}

	// Always between two nodes, or between head node and a normal node.
	else
	{// Re-link the nodes, length+1
		self->length++;

		after_node->previous->next = new_node;
		new_node->previous = after_node->previous;

		new_node->data = data;
		new_node->size = size;
		new_node->type = type;

		new_node->next = after_node;
		after_node->previous = new_node;

		return SUCCEEDED;
	}
}

Status CLinkedlist_change(CLinkedlist *self, uint64_t index, void *data, size_t size, void *type)
{
	CLinkedlistNode *target = CLinkedlist_get_node(self, index);

	// Index out range or empty linked list.
	if (target == INDEX_OUT_RANGE || target == CLINKEDLIST_EMPTY)
		return (void *)target;
	else
	{
		if (target->data) free(target->data);
		target->data = data;
		target->size = size;
		target->type = type;
		return SUCCEEDED;
	}
}

CLinkedlistNode ** CLinkedlist_forward_iter(CLinkedlist *self)
{
	if (CLinkedlist_is_empty(self))
		return CLINKEDLIST_EMPTY;

	// Actually it return a pointer-array that store the nodes's pointers.
	CLinkedlistNode *start = self->head->next;
	CLinkedlistNode **list = (CLinkedlistNode **)calloc(self->length, sizeof(CLinkedlistNode *));
	if (list == NULL)
		return MEMORY_ALLOCATE_FAILED;

	for (uint64_t i = 0; i < self->length; ++i)
	{
		list[i] = start;
		start = start->next;
	}

	return list;
}

CLinkedlistNode ** CLinkedlist_backward_iter(CLinkedlist *self)
{
	if (CLinkedlist_is_empty(self))
		return CLINKEDLIST_EMPTY;

	// Actually it return a pointer-array that store the node's pointers.
	CLinkedlistNode  *last = CLinkedlist_last_node(self);
	CLinkedlistNode **list = (CLinkedlistNode **)calloc(self->length, sizeof(CLinkedlistNode *));
	if (list == NULL)
		return MEMORY_ALLOCATE_FAILED;

	for (uint64_t i = self->length; i >= 1; --i)
	{
		list[self->length - i] = last;
		last = last->previous;
	}

	return list;
}

Status CLinkedlist_reverse(CLinkedlist *self)
{
	// Check if empty.
	CLinkedlistNode *last = CLinkedlist_last_node(self);
	if (last == CLINKEDLIST_EMPTY)
		return CLINKEDLIST_EMPTY;

	// Get the nodes' pointer and store in a pointer-array (reversed).
	CLinkedlistNode **nodes = CLinkedlist_backward_iter(self);
	if (nodes == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

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

	free(nodes);
	return SUCCEEDED;
}

bool CLinkedlist_is_empty(CLinkedlist *self)
{
	if (self->length <= 0 || self->head->next == NULL)
		return true;
	else return false;
}

uint64_t CLinkedlist_total_size(CLinkedlist *self)
{
	uint64_t total_size = 0;

	if (self->is_empty(self))
		return 0xffffffffffffffff;

	CLinkedlistNode **list = CLinkedlist_forward_iter(self);
	if (list == NULL)
		return 0xfffffffffffffffe;

	for (uint64_t i = 0; i < self->length; ++i)
		total_size += list[i]->size;

	free(list);
	return total_size;
}

uint64_t CLinkedlist_get_length(CLinkedlist *self)
{
	return self->length;
}