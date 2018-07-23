#include <stdlib.h>
#include <string.h>
#include "../common.h"
#include "CStack.h"

CStack * new_CStack(const uint64_t capacity)
{
	if (capacity <= 0)
		return NULL;
	
	CStack *stack = (CStack *)calloc(1, sizeof(CStack));
	if (stack == NULL)
		return NULL;

	else
	{
		stack->base = (void **)calloc(capacity, sizeof(void *));
		if (stack->base == NULL)
		{
			free(stack);
			return NULL;
		}
		else
		{
			stack->size = (uint32_t *)calloc(capacity, sizeof(uint32_t));
			if (stack->size == NULL)
			{
				free(stack->base);
				free(stack);
				return NULL;
			}
			memset(stack->size, 0, capacity * sizeof(uint32_t));
		}
	}

	stack->top  = stack->base;
	stack->capacity = capacity;

	stack->push     = &CStack_push;
	stack->pop      = &CStack_pop;
	stack->is_empty = &CStack_is_empty;
	stack->is_full  = &CStack_is_full;

	return stack;
}

void delete_CStack(CStack *self)
{
	free(self->size);
	free(self->base);
	free(self);
}

bool CStack_push(CStack *self, void *data, size_t size)
{
	if (CStack_is_full(self) || size < 0)
		return false;
	else {
		*(self->top) = data;
		(self->size)[self->top - self->base] = size;
		self->top++;
		return true;
	}
}

void * CStack_pop(CStack *self, size_t *size)
{
	if (CStack_is_empty(self) || size == NULL)
		return NULL;
	else {
		self->top--;
		void *to_pop = *(self->top);
		*(self->top) = NULL;
		*size = (self->size)[self->top - self->base];
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