#include <stdlib.h>
#include <string.h>
#include "../common.h"
#include "CStack.h"

CStack * new_CStack(const uint64_t size)
{
	CStack *stack = (CStack *)calloc(1, sizeof(CStack));
	if (stack == NULL)
		return NULL;
	memset(stack, 0, sizeof(CStack));

	stack->base = (void **)calloc(size, sizeof(void *));
	if (stack->base == NULL)
		return NULL;

	stack->top  = stack->base;
	stack->size = size;

	stack->CStack_push     = &CStack_push;
	stack->CStack_pop      = &CStack_pop;
	stack->CStack_is_empty = &CStack_is_empty;
	stack->CStack_is_full  = &CStack_is_full;

	return stack;
}

void delete_CStack(CStack *self)
{
	free(self->base);
	free(self);
}

bool CStack_push(CStack *self, void *data)
{
	if (CStack_is_full(self))
		return false;
	else {
		*(self->top) = data;
		self->top++;
		return true;
	}
}

void * CStack_pop(CStack *self)
{
	if (CStack_is_empty(self))
		return NULL;
	else {
		void *CStack_pop = *(self->top);
		*(self->top) = NULL;
		self->top--;
		return CStack_pop;
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
	if (self->top - self->base >= self->size)
		return true;
	else return false;
}