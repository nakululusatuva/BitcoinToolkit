#include <stdlib.h>
#include <string.h>
#include "internal/container/CStack.h"

CStack * new_CStack(const uint64_t capacity)
{
	if (capacity <= 0)
		return CSTACK_INVALID_CAPACITY;
	
	CStack *stack = (CStack *)calloc(1, sizeof(CStack));
	if (stack == NULL)
		return MEMORY_ALLOCATE_FAILED;

	stack->base = (void **)calloc(capacity, sizeof(void *));
	if (stack->base == NULL)
	{
		free(stack);
		return MEMORY_ALLOCATE_FAILED;
	}

	stack->size = (uint32_t *)calloc(capacity, sizeof(uint32_t));
	if (stack->size == NULL)
	{
		free(stack->base);
		free(stack);
		return MEMORY_ALLOCATE_FAILED;
	}

	stack->type = (void **)calloc(capacity, sizeof(void *));
	if (stack->type == NULL)
	{
		free(stack->size);
		free(stack->base);
		free(stack);
		return MEMORY_ALLOCATE_FAILED;
	}

	stack->autofree = (bool *)calloc(capacity, sizeof(bool));
	if (stack->autofree == NULL)
	{
		free(stack->size);
		free(stack->base);
		free(stack->type);
		free(stack);
		return MEMORY_ALLOCATE_FAILED;
	}

	stack->top  = stack->base;
	stack->capacity = capacity;

	stack->push       = &CStack_push;
	stack->pop        = &CStack_pop;
	stack->is_empty   = &CStack_is_empty;
	stack->is_full    = &CStack_is_full;
	stack->total_size = &CStack_total_size;
	stack->get_depth  = &CStack_get_depth;
	stack->get_capacity = &CStack_get_capacity;

	return stack;
}

void delete_CStack(CStack *self)
{
	for (int i = 0; i < self->top - self->base; ++i)
	{
		if ((self->base)[i] && (self->autofree)[i])
			free((self->base)[i]);
	}
	free(self->base);
	free(self->size);
	free(self->type);
	free(self->autofree);
	free(self);
}

Status CStack_push(CStack *self, void *data, size_t size, void *type, bool autofree)
{
	if (CStack_is_full(self))
		return CSTACK_FULL;
	else
	{
		*(self->top) = data;
		if (data == NULL)
			(self->size)[self->top - self->base] = 0;
		else
			(self->size)[self->top - self->base] = size;
		(self->type)[self->top - self->base] = type;
		(self->autofree)[self->top - self->base] = autofree;
		self->top++;
		return SUCCEEDED;
	}
}

Status CStack_pop(CStack *self, size_t *size, void **type, bool *autofree)
{
	if (CStack_is_empty(self))
		return CSTACK_EMPTY;
	else if (size == NULL)
		return PASSING_NULL_POINTER;
	else
	{
		self->top--;
		void *to_pop = *(self->top);
		*(self->top) = NULL;
		if (size != NULL)
			size[0] = (self->size)[self->top - self->base];
		if (type != NULL)
			type[0] = (self->type)[self->top - self->base];
		if (autofree != NULL)
			autofree[0] = (self->autofree)[self->top - self->base];
		return to_pop;
	}
}

bool CStack_is_empty(CStack *self)
{
	if (self->top == self->base)
		return true;
	else return false;
}

bool CStack_is_full(CStack *self)
{
	if (self->top - self->base >= self->capacity)
		return true;
	else return false;
}

uint64_t CStack_total_size(CStack *self)
{
	uint64_t total_size = 0;

	for (uint64_t i = 0; i < self->capacity; ++i)
		total_size = total_size + (self->size)[i];

	return total_size;
}

uint64_t CStack_get_depth(CStack *self)
{
	return self->top - self->base;
}

uint64_t CStack_get_capacity(CStack *self)
{
	return self->capacity;
}