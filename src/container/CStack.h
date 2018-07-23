#ifndef _CSTACK_
#define _CSTACK_

#include "../common.h"

typedef struct CStack CStack;
struct CStack
{
	void   **base;
	void   **top;
	uint64_t capacity;
	uint32_t *size;

	bool (*push)(CStack *, void *, size_t);
	void * (*pop)(CStack *, size_t *);
	bool (*is_empty)(CStack *);
	bool (*is_full)(CStack *);
};

// Construct and Destruct Fuctions.
CStack * new_CStack(const uint64_t capacity);
void delete_CStack(CStack *self);

// Member Fuctions.
bool CStack_push(CStack *self, void *data, size_t size);
void * CStack_pop(CStack *self, size_t *size);
bool CStack_is_empty(CStack *self);
bool CStack_is_full(CStack *self);

#endif