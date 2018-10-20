/** AUTOHEADER TAG: DELETE BEGIN **/
#ifndef _CSTACK_
#define _CSTACK_

#include "internal/common.h"
/** AUTOHEADER TAG: DELETE END **/

/* 0x001010 ~ 0x00101f : CStack */
#define CSTACK_EMPTY            (void *)0x1010
#define CSTACK_FULL             (void *)0x1011
#define CSTACK_INVALID_CAPACITY (void *)0x1012

/** Common Type Stack. It stores the data's pointer, instead of the data itself **/
typedef struct CStack CStack;
struct CStack
{
	void   **base;
	void   **top;
	uint64_t capacity;
	uint32_t *size;
	void   **type;
	bool    *autofree;

	Status (*push)(CStack *, void *, size_t, void *, bool);
	Status (*pop)(CStack *, size_t *, void **, bool *);
	bool (*is_empty)(CStack *);
	bool (*is_full)(CStack *);
	size_t (*total_size)(CStack *);
	uint64_t (*get_depth)(CStack *);
	uint64_t (*get_capacity)(CStack *);
};

// Construct and Destruct Fuctions.
/** New a common type stack.
*   \param  capacity    How many elements.
*   \return errors INVALID_CAPACITY
*                  MEMORY_ALLOCATE_FAILED
*   \else on succeeded.
**/
CStack * new_CStack(const uint64_t capacity);
void delete_CStack(CStack *self);

/** AUTOHEADER TAG: DELETE BEGIN **/
// Member Fuctions.
/** Push data onto stack.
*   \param  data        Data's pointer.
*   \param  size        How many bytes.
*   \param  type        A pointer that prompt the data type, check ./src/status.h
*                       NULL is allowed if you don't need to mark the data type.
*   \return success: SUCCEEDED
*           errors:  CSTACK_FULL
*   Parameter 'data' must be allocated on heap memory, NULL is allowed.
*   Once CStack_push() returns SUCCEEDED:
*   1. Do not push 'data' to another CStack.
*   2. Do not free 'data' manually, the destruct function will do the job.
**/
Status CStack_push(CStack *self, void *data, size_t size, void *type, bool autofree);

/** Pop the top element.
*   \param  size        Store the top element's data size (bytes).
*   \param  type        Store a pointer that prompt the data type, check ./src/status.h
*                       NULL is allowed if you don't need to know the data type.
*   \return errors: CSTACK_EMPTY
*   \else on succeeded.
*   Once CStack_pop() success, you need to free the popped pointer manually.
**/
Status CStack_pop(CStack *self, size_t *size, void **type, bool *autofree);

/* Check if stack is empty */
bool CStack_is_empty(CStack *self);

/* Check if stack is full */
bool CStack_is_full(CStack *self);

/* Get total data size, return how many bytes */
uint64_t CStack_total_size(CStack *self);
uint64_t CStack_get_depth(CStack *self);
uint64_t CStack_get_capacity(CStack *self);

#endif
/** AUTOHEADER TAG: DELETE END **/