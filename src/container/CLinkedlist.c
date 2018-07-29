#include "../common.h"
#include "CLinkedlist.h"
#include <stdlib.h>
#include <string.h>

CLinkedlist * new_CLinkedlist()
{
	CLinkedlist *new = (CLinkedlist *)calloc(1, sizeof(CLinkedlist));
	if (new == NULL)
		return NULL;

	else
	{
		new->head = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));
		if (new->head == NULL)
		{
			free(new);
			return NULL;
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
	new->forward_traversing  = &CLinkedlist_forward_traversing;
	new->backward_traversing = &CLinkedlist_backward_traversing;
	new->reverse             = &CLinkedlist_reverse;
	new->is_empty            = &CLinkedlist_is_empty;
	new->total_size          = &CLinkedlist_total_size;
	new->last_node           = &CLinkedlist_last_node;
	new->specific_node       = &CLinkedlist_specific_node;

	return new;
}

void delete_CLinkedlist(CLinkedlist *this)
{
	// Check if empty.
	if (CLinkedlist_is_empty(this))
	{
		free(this->head);
		free(this);
	}

	else
	{	// Delete all nodes.
		CLinkedlistNode *start = this->head->next;

		while (1)
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
		free(this->head);
		free(this);
	}
}

CLinkedlistNode * CLinkedlist_last_node(CLinkedlist *this)
{
	if (CLinkedlist_is_empty(this))
		return NULL;

	CLinkedlistNode *buffer = this->head->next;
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

CLinkedlistNode * CLinkedlist_specific_node(CLinkedlist *this, uint64_t index)
{
	// Check if empty or index out range.
	if (CLinkedlist_is_empty(this) || index < 0 || index >= this->length)
		return NULL;

	uint64_t i;
	CLinkedlistNode *buffer = this->head->next;
	for (i = 0; i < this->length; ++i)
	{
		if (i == index)
			break;
		else buffer = buffer->next;
	}

	return buffer;
}

bool CLinkedlist_add(CLinkedlist *this, void *data, size_t size)
{
	CLinkedlistNode *last = CLinkedlist_last_node(this);
	// If empty, the last one will be the head.
	if (last == NULL)
		last = this->head;

	CLinkedlistNode *new  = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));
	if (new == NULL)
		return false;

	// Re-link the nodes.
	last->next = new;
	new->previous = last;
	new->data = data;
	new->size = size;
	new->next = NULL;

	this->length++;

	return true;
}

bool CLinkedlist_delete(CLinkedlist *this, uint64_t index)
{
	CLinkedlistNode *target = CLinkedlist_specific_node(this, index);

	// Index out range or empty linked list.
	if (target == NULL)
		return false;

	// The only node.
	else if (target->previous->previous == NULL && target->next == NULL)
	{
		this->head->next = NULL;
		free(target->data);
		free(target);
		this->length--;
		return true;
	}

	// The first node.
	else if (target->previous->previous == NULL && target->next != NULL)
	{
		this->head->next = target->next;
		target->next->previous = this->head;
		free(target->data);
		free(target);
		this->length--;
		return true;
	}

	// The last node.
	else if (target->previous->previous != NULL && target->next == NULL)
	{
		target->previous->next = NULL;
		this->length--;
		free(target->data);
		free(target);
		return true;
	}

	// Any where else (Procedures are as same as the first node).
	// (target->previous->previous != NULL && target->next != NULL)
	else
	{
		target->previous->next = target->next;
		target->next->previous = target->previous;
		free(target->data);
		free(target);
		this->length--;
		return true;
	}
}

bool CLinkedlist_insert(CLinkedlist *this, uint64_t after, void *data, size_t size)
{
	CLinkedlistNode *new_node   = (CLinkedlistNode *)calloc(1, sizeof(CLinkedlistNode));
	if (new_node == NULL)
		return false;

	CLinkedlistNode *after_node = CLinkedlist_specific_node(this, after);

	// Check if index out range or empty linked list.
	if (after_node == NULL)
	{
		free(new_node);
		return NULL;
	}
	// Always between two nodes, or between head node and a normal node.
	else
	{// Re-link the nodes, length+1
		this->length++;

		after_node->previous->next = new_node;
		new_node->previous = after_node->previous;

		new_node->data = data;
		new_node->size = size;

		new_node->next = after_node;
		after_node->previous = new_node;

		return true;
	}
}

bool CLinkedlist_change(CLinkedlist *this, uint64_t index, void *data, size_t size)
{
	CLinkedlistNode *target = CLinkedlist_specific_node(this, index);

	// Index out range or empty linked list.
	if (target == NULL)
		return false;

	else
	{
		free(target->data);
		target->data = data;
		target->size = size;
		return true;
	}
}

CLinkedlistNode ** CLinkedlist_forward_traversing(CLinkedlist *this)
{
	if (CLinkedlist_is_empty(this))
		return NULL;

	// Actually it return a pointer-array that store the nodes's pointers.
	CLinkedlistNode *start = this->head->next;
	CLinkedlistNode **list = (CLinkedlistNode **)calloc(this->length, sizeof(CLinkedlistNode *));
	if (list == NULL)
		return NULL;

	for (uint64_t i = 0; i < this->length; ++i)
	{
		list[i] = start;
		start = start->next;
	}

	return list;
}

CLinkedlistNode ** CLinkedlist_backward_traversing(CLinkedlist *this)
{
	if (CLinkedlist_is_empty(this))
		return NULL;

	// Actually it return a pointer-array that store the node's pointers.
	CLinkedlistNode  *last = CLinkedlist_last_node(this);
	CLinkedlistNode **list = (CLinkedlistNode **)calloc(this->length, sizeof(CLinkedlistNode *));
	if (list == NULL)
		return NULL;

	for (uint64_t i = this->length; i >= 1; --i)
	{
		list[this->length - i] = last;
		last = last->previous;
	}

	return list;
}

bool CLinkedlist_reverse(CLinkedlist *this)
{
	// Check if empty.
	CLinkedlistNode *last = CLinkedlist_last_node(this);
	if (last == NULL)
		return false;

	// Get the nodes' pointer and store in a pointer-array (reversed).
	CLinkedlistNode **nodes = CLinkedlist_backward_traversing(this);
	if (nodes == NULL)
		return false;

	// Re-link nodes, follow the pointer-array.
	CLinkedlistNode *buffer = this->head;
	for (uint64_t i = 0; i < this->length; ++i)
	{
		buffer->next = nodes[i];
		nodes[i]->previous = buffer;
		if (i == this->length - 1)
			nodes[i]->next = NULL;
		buffer = nodes[i];
	}

	free(nodes);
	return true;
}

bool CLinkedlist_is_empty(CLinkedlist *this)
{
	if (this->length <= 0 || this->head->next == NULL)
		return true;
	else return false;
}

size_t CLinkedlist_total_size(CLinkedlist *this)
{
	size_t total_size = 0;

	CLinkedlistNode **list = CLinkedlist_forward_traversing(this);
	if (list == NULL)
		return -1;

	for (size_t i = 0; i < this->length; ++i)
		total_size += list[i]->size;

	free(list);
	return total_size;
}