#ifndef _CLINKEDLIST_
#define _CLINKEDLIST_

#include "internal/common.h"

/* 0x1000 ~ 0x100f : CLinkedlist */
#define CLINKEDLIST_EMPTY (void *)0x1000

/** Common Type Linked List. It stores the data's pointer, instead of the data itself **/
typedef struct CLinkedlistNode CLinkedlistNode;
struct CLinkedlistNode
{
	CLinkedlistNode *previous;
	void *data;
	size_t size; // How many bytes.
	void *type;
	CLinkedlistNode *next;
};
typedef struct CLinkedlist CLinkedlist;
struct CLinkedlist
{
	CLinkedlistNode *head;
	uint64_t length;

	Status (*add)(CLinkedlist *, void *, size_t, void *);
	Status (*delete)(CLinkedlist *, uint64_t);
	Status (*insert)(CLinkedlist *, uint64_t, void *, size_t, void *);
	Status (*change)(CLinkedlist *, uint64_t, void *, size_t, void *);
	CLinkedlistNode ** (*forward_iter)(CLinkedlist *);
	CLinkedlistNode ** (*backward_iter)(CLinkedlist *);
	void * (*reverse)(CLinkedlist *);
	bool (*is_empty)(CLinkedlist *);
	size_t (*total_size)(CLinkedlist *);
	CLinkedlistNode * (*last_node)(CLinkedlist *);
	CLinkedlistNode * (*get_node)(CLinkedlist *, uint64_t);
	uint64_t (*get_length)(CLinkedlist *);
};

/** Construct and Destruct Functions **/
CLinkedlist * new_CLinkedlist();
void delete_CLinkedlist(CLinkedlist *this);

/** Member Fuctions **/
/** Add a data's pointer to the linked list.
*   \param  data        Data's pointer.
*   \param  size        Data's size, how many bytes.
*   \param  type        A pointer that prompt the data type, check ./src/status.h
*                       NULL is allowed if you don't need to mark the data type.
*   \return success: SUCCEEDED
*           errors:  MEMORY_ALLOCATE_FAILED
*   Parameter 'data' must be allocated on heap memory, NULL is allowed.
*   Once CLinkedlist_add() returns SUCCEEDED:
*   1. Do not add or insert 'data' to another CLinkedlist.
*   2. Do not free 'data' manually, the destruct function will do the job.
**/
Status CLinkedlist_add(CLinkedlist *this, void *data, size_t size, void *type);

/** Delete a node.
*   \param  index       Node's position, start from zero.
*   \return success: SUCCEEDED
*           errors:  CLINKEDLIST_EMPTY
*                    INDEX_OUT_RANGE
**/
Status CLinkedlist_delete(CLinkedlist *this, uint64_t index);

/** Insert a data's pointer.
*   \param  after       Node's position, the new node will be in front of it.
*   \param  data        Data's pointer.
*   \param  size        Data's size, how many bytes.
*   \param  type        A pointer that prompt the data type, check ./src/status.h
*                       NULL is allowed if you don't need to mark the data type.
*   \return success: SUCCEEDED
*           errors:  MEMORY_ALLOCATE_FAILED
*                    INDEX_OUT_RANGE
*				     CLINKEDLIST_EMPTY
*   Parameter 'data' must be allocated on heap memory, NULL is allowed.
*   Once CLinkedlist_insert() returns SUCCEEDED:
*   1. Do not add or insert 'data' to another CLinkedlist.
*   2. Do not free 'data' manually, the destruct function will do the job.
**/
Status CLinkedlist_insert(CLinkedlist *this, uint64_t after, void *data, size_t size, void *type);

/** Change change node's data.
*   \param  index       Node's position.
*   \param  data        New data's pointer, the old data will be freed automatically.
*   \param  size        New data's size, how many bytes.
*   \param  type        A pointer that prompt the data type, check ./src/status.h
*                       NULL is allowed if you don't need to mark the data type.
*   \return success: SUCCEEDED
*           errors:  INDEX_OUT_RANGE
*                    CLINKEDLIST_EMPTY
*   Parameter 'data' must be allocated on heap memory, NULL is allowed.
*   Once CLinkedlist_change() returns SUCCEEDED:
*   1. Do not add or insert 'data' to another CLinkedlist.
*   2. Do not free 'data' manually, the destruct function will do the job.
*   3. The old data will be freed automatically.
**/
Status CLinkedlist_change(CLinkedlist *this, uint64_t index, void *data, size_t size, void *type);

/** Forward iterate the linked list.
*   \return errors: CLINKEDLIST_EMPTY
*                   MEMORY_ALLOCATE_FAILED
*   \else on an array pointer that store the nodes' pointer, need to be freed manually.
*   Do not free the nodes or the node->data manually.
**/
CLinkedlistNode ** CLinkedlist_forward_iter(CLinkedlist *this);

/** Backward iterate the linked list.
*   \return errors: CLINKEDLIST_EMPTY
*                   MEMORY_ALLOCATE_FAILED
*   \else on an array pointer that store the nodes' pointer, need to be freed manually.
*   Do not free the nodes or the node->data manually.
**/
CLinkedlistNode ** CLinkedlist_backward_iter(CLinkedlist *this);
/* Reverse the linked list */
Status CLinkedlist_reverse(CLinkedlist *this);
bool CLinkedlist_is_empty(CLinkedlist *this);
/* Get the total data size
* return -1(0xffffffffffffffff) on empty linked list
* -2(0xfffffffffffffffe) on memory allocated failed */
uint64_t CLinkedlist_total_size(CLinkedlist *this);

/** Get the last node's pointer
*   \return errors: CLINKEDLIST_EMPTY
*   \else on node's pointer.
*   Do not free the node or the node->data manually.
**/
CLinkedlistNode * CLinkedlist_last_node(CLinkedlist *this);

/** Get the  node's pointer
*   \return errors: CLINKEDLIST_EMPTY
*                   INDEX_OUT_RANGE
*   \else on node's pointer.
*   Do not free the node or the node->data manually.
**/
CLinkedlistNode * CLinkedlist_get_node(CLinkedlist *this, uint64_t index);
uint64_t CLinkedlist_get_length(CLinkedlist *this);

#endif#ifndef _CSTACK_
#define _CSTACK_

#include "internal/common.h"

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

	Status (*push)(CStack *, void *, size_t, void *);
	Status (*pop)(CStack *, size_t *, void **);
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
void delete_CStack(CStack *this);

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
Status CStack_push(CStack *this, void *data, size_t size, void *type);

/** Pop the top element.
*   \param  size        Store the top element's data size (bytes).
*   \param  type        Store a pointer that prompt the data type, check ./src/status.h
*                       NULL is allowed if you don't need to know the data type.
*   \return errors: CSTACK_EMPTY
*   \else on succeeded.
*   Once CStack_pop() success, you need to free the popped pointer manually.
**/
Status CStack_pop(CStack *this, size_t *size, void **type);

/* Check if stack is empty */
bool CStack_is_empty(CStack *this);

/* Check if stack is full */
bool CStack_is_full(CStack *this);

/* Get total data size, return how many bytes */
uint64_t CStack_total_size(CStack *this);
uint64_t CStack_get_depth(CStack *this);
uint64_t CStack_get_capacity(CStack *this);

#endif