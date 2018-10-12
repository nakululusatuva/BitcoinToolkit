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

void delete_CStack(CStack *this)
{
	for (int i = 0; i < this->top - this->base; ++i)
	{
		if ((this->base)[i]) free((this->base)[i]);
	}
	free(this->base);
	free(this->size);
	free(this->type);
	free(this);
}

Status CStack_push(CStack *this, void *data, size_t size, void *type)
{
	if (CStack_is_full(this))
		return CSTACK_FULL;
	else
	{
		*(this->top) = data;
		if (data == NULL)
			(this->size)[this->top - this->base] = 0;
		else
			(this->size)[this->top - this->base] = size;
		(this->type)[this->top - this->base] = type;
		this->top++;
		return SUCCEEDED;
	}
}

Status CStack_pop(CStack *this, size_t *size, void **type)
{
	if (CStack_is_empty(this))
		return CSTACK_EMPTY;
	else if (size == NULL)
		return PASSING_NULL_POINTER;
	else
	{
		this->top--;
		void *to_pop = *(this->top);
		*(this->top) = NULL;
		if (size != NULL)
			*size = (this->size)[this->top - this->base];
		if (type != NULL)
			type[0] = (this->type)[this->top - this->base];
		return to_pop;
	}
}

bool CStack_is_empty(CStack *this)
{
	if (this->top == this->base)
		return true;
	else return false;
}

bool CStack_is_full(CStack *this)
{
	if (this->top - this->base >= this->capacity)
		return true;
	else return false;
}

uint64_t CStack_total_size(CStack *this)
{
	uint64_t total_size = 0;

	for (uint64_t i = 0; i < this->capacity; ++i)
		total_size = total_size + (this->size)[i];

	return total_size;
}

uint64_t CStack_get_depth(CStack *this)
{
	return this->top - this->base;
}

uint64_t CStack_get_capacity(CStack *this)
{
	return this->capacity;
}