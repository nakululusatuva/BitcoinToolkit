#include <stdlib.h>
#include <string.h>
#include "../common.h"
#include "CStack.h"

CStack * new_CStack(const uint64_t capacity)
{
	if (capacity <= 0)
		return INVALID_CAPACITY;
	
	CStack *stack = (CStack *)calloc(1, sizeof(CStack));
	if (stack == NULL)
		return MEMORY_ALLOCATE_FAILED;

	else
	{
		stack->base = (void **)calloc(capacity, sizeof(void *));
		if (stack->base == NULL)
		{
			free(stack);
			return MEMORY_ALLOCATE_FAILED;
		}
		else
		{
			stack->size = (uint32_t *)calloc(capacity, sizeof(uint32_t));
			if (stack->size == NULL)
			{
				free(stack->base);
				free(stack);
				return MEMORY_ALLOCATE_FAILED;
			}
		//	memset(stack->size, 0, capacity * sizeof(uint32_t));
		}
	}

	stack->top  = stack->base;
	stack->capacity = capacity;

	stack->push       = &CStack_push;
	stack->pop        = &CStack_pop;
	stack->is_empty   = &CStack_is_empty;
	stack->is_full    = &CStack_is_full;
	stack->total_size = &CStack_total_size;
	stack->get_element_amount = &CStack_get_element_amount;
	stack->get_capacity = &CStack_get_capacity;

	return stack;
}

void delete_CStack(CStack *this)
{
	for (int i = 0; i < this->top - this->base; ++i)
	{
		free((this->base)[i]);
	}
	free(this->base);
	free(this->size);
	free(this);
}

bool CStack_push(CStack *this, void *data, size_t size)
{
	if (CStack_is_full(this) || size < 0)
		return false;
	else {
		*(this->top) = data;
		(this->size)[this->top - this->base] = size;
		this->top++;
		return true;
	}
}

void * CStack_pop(CStack *this, size_t *size)
{
	if (CStack_is_empty(this) || size == NULL)
		return CSTACK_EMPTY;
	else {
		this->top--;
		void *to_pop = *(this->top);
		*(this->top) = NULL;
		*size = (this->size)[this->top - this->base];
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

size_t CStack_total_size(CStack *this)
{
	size_t total_size = 0;

	for (size_t i = 0; i < this->capacity; ++i)
		total_size = total_size + (this->size)[i];

	return total_size;
}

uint64_t CStack_get_element_amount(CStack *this)
{
	return this->top - this->base;
}

uint64_t CStack_get_capacity(CStack *this)
{
	return this->capacity;
}