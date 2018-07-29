#ifndef _CSTACK_
#define _CSTACK_

#include "../common.h"

/** Common Type Stack.
*   1. It stores the data's pointer, instead of the data itself.
*   2. Please make sure the data's memory is allocated manually,
*      errors will happen if you push a pointer that point to stack memory onto the stack.
*   3. Do not free the data manually once it's pointer was push onto the stack,
*      the destruct function will do the job.
*   4. The memory that the popped pointer points to will not be freed by destructuring function,
*      you need to free it manually.
**/

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
	size_t (*total_size)(CStack *);
};

// Construct and Destruct Fuctions.
/** New a common type stack.
*   \param  capacity    How many elements.
**/
CStack * new_CStack(const uint64_t capacity);
void delete_CStack(CStack *this);

// Member Fuctions.
/** Push data onto stack.
*   \param  data        Point to the data.
*   \param  size        How many bytes.
*   \return true on success.
*          false on error.
**/
bool CStack_push(CStack *this, void *data, size_t size);

/** Pop the top element.
*   \param  size        Store the top element's data size (bytes).
*   \return NULL on error.
*           else on success.
**/
void * CStack_pop(CStack *this, size_t *size);

/** Check if stack is empty.
*   \return true on empty.
*          false on not empty.
**/
bool CStack_is_empty(CStack *this);

/** Check if stack is full.
*   \return true on full.
*          false on not full.
*/
bool CStack_is_full(CStack *this);

/** Get total data size.
*   \return how many bytes.
**/
size_t CStack_total_size(CStack *this);

#endif