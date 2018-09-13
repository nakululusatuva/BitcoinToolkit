#ifndef _CSTACK_
#define _CSTACK_

#include "../common.h"

/** Common Type Stack. It stores the data's pointer, instead of the data itself **/
typedef struct CStack CStack;
struct CStack
{
	void   **base;
	void   **top;
	uint64_t capacity;
	uint32_t *size;
	void   **type;

	void * (*push)(CStack *, void *, size_t, void *);
	void * (*pop)(CStack *, size_t *, void **);
	bool (*is_empty)(CStack *);
	bool (*is_full)(CStack *);
	size_t (*total_size)(CStack *);
	uint64_t (*get_depth)(CStack *);
	uint64_t (*get_capacity)(CStack *);
};

// Construct and Destruct Fuctions.
/** New a common type stack.
*   \param  capacity    How many elements.
*   \return error codes:
*           INVALID_CAPACITY
*           MEMORY_ALLOCATE_FAILED
*   \else on success.
**/
CStack * new_CStack(const uint64_t capacity);
void delete_CStack(CStack *this);

// Member Fuctions.
/** Push data onto stack.
*   \param  data        Point to the data.
*   \param  size        How many bytes.
*   \return true on success.
*          false on error.
*   Parameter 'data' must be allocated on heap memory.
*   Once CStack_push() returns true:
*   1. Do not push 'data' to another CStack.
*   2. Do not free 'data' manually, the destruct function will do the job.
**/
void * CStack_push(CStack *this, void *data, size_t size, void *type);

/** Pop the top element.
*   \param  size        Store the top element's data size (bytes).
*   \return error codes:
*           CSTACK_EMPTY
*   \else on success.
*   Once CStack_pop() success, you need to free the returned pointer manually.
**/
void * CStack_pop(CStack *this, size_t *size, void **type);

/* Check if stack is empty */
bool CStack_is_empty(CStack *this);

/* Check if stack is full */
bool CStack_is_full(CStack *this);

/* Get total data size, return how many bytes */
size_t CStack_total_size(CStack *this);
uint64_t CStack_get_depth(CStack *this);
uint64_t CStack_get_capacity(CStack *this);

#endif