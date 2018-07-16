#ifndef _CSTACK_
#define _CSTACK_

#include "../common.h"

typedef struct CStack CStack;
struct CStack
{
	void   **base;
	void   **top;
	uint64_t size;

	bool (*CStack_push)(CStack *, void *);
	void * (*CStack_pop)(CStack *);
	bool (*CStack_is_empty)(CStack *);
	bool (*CStack_is_full)(CStack *);
};

// Construct and Destruct Fuctions.
CStack * new_CStack(const uint64_t size);
void delete_CStack(CStack *self);

// Member Fuctions.
bool CStack_push(CStack *self, void *data);
void * CStack_pop(CStack *self);
bool CStack_is_empty(CStack *self);
bool CStack_is_full(CStack *self);

#endif