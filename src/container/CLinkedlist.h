#ifndef _CLINKEDLIST_
#define _CLINKEDLIST_

#include "../common.h"

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

	void * (*add)(CLinkedlist *, void *, size_t, void *);
	void * (*delete)(CLinkedlist *, uint64_t);
	void * (*insert)(CLinkedlist *, uint64_t, void *, size_t, void *);
	void * (*change)(CLinkedlist *, uint64_t, void *, size_t, void *);
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
void * CLinkedlist_add(CLinkedlist *this, void *data, size_t size, void *type);

/** Delete a node.
*   \param  index       Node's position, start from zero.
*   \return success: SUCCEEDED
*           errors:  CLINKEDLIST_EMPTY
*                    INDEX_OUT_RANGE
**/
void * CLinkedlist_delete(CLinkedlist *this, uint64_t index);

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
void * CLinkedlist_insert(CLinkedlist *this, uint64_t after, void *data, size_t size, void *type);

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
void * CLinkedlist_change(CLinkedlist *this, uint64_t index, void *data, size_t size, void *type);

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
void * CLinkedlist_reverse(CLinkedlist *this);
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

#endif