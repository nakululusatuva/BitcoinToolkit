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
	CLinkedlistNode ** (*forward_traversing)(CLinkedlist *);
	CLinkedlistNode ** (*backward_traversing)(CLinkedlist *);
	void * (*reverse)(CLinkedlist *);
	bool (*is_empty)(CLinkedlist *);
	size_t (*total_size)(CLinkedlist *);
	CLinkedlistNode * (*last_node)(CLinkedlist *);
	CLinkedlistNode * (*specific_node)(CLinkedlist *, uint64_t);
	uint64_t (*get_length)(CLinkedlist *);
};

/** Construct and Destruct Functions **/
CLinkedlist * new_CLinkedlist();
void delete_CLinkedlist(CLinkedlist *this);

/** Member Fuctions **/
/** Add a data's pointer.
*   \param  data        Data's pointer.
*   \param  size        Data's size, how many bytes.
*   \return true on success.
*          false on error.
*   Parameter 'data' must be allocated on heap memory.
*   Once CLinkedlist_add() returns true:
*   1. Do not add or insert 'data' to another CLinkedlist.
*   2. Do not free 'data' manually, the destruct function will do the job.
**/
void * CLinkedlist_add(CLinkedlist *this, void *data, size_t size, void *type);

/** Delete a node.
*   \param  index       Node's position, start from zero.
*   \return true on success.
*          false on error.
**/
void * CLinkedlist_delete(CLinkedlist *this, uint64_t index);

/** Insert a data's pointer.
*   \param  after       Node's position, the new node will be in front of it.
*   \param  data        Data's pointer.
*   \param  size        Data's size, how many bytes.
*   \return true on success.
*          false on error.
*   Parameter 'data' must be allocated on heap memory.
*   Once CLinkedlist_insert() returns true:
*   1. Do not add or insert 'data' to another CLinkedlist.
*   2. Do not free 'data' manually, the destruct function will do the job.
**/
void * CLinkedlist_insert(CLinkedlist *this, uint64_t after, void *data, size_t size, void *type);

/** Change specific node's data.
*   \param  index       Node's position, the node you want to edit.
*   \param  data        New data's pointer, the old data itself will be freed.
*   \param  size        New data's size, how many bytes.
*   \return true on success.
*          false on error.
*   Parameter 'data' must be allocated on heap memory.
*   Once CLinkedlist_insert() returns true:
*   1. Do not add or insert 'data' to another CLinkedlist.
*   2. Do not free 'data' manually, the destruct function will do the job.
*   3. The old data will be freed.
**/
void * CLinkedlist_change(CLinkedlist *this, uint64_t index, void *data, size_t size, void *type);

/** Forward traversing the linked list.
*   \return error codes:
*           CLINKEDLIST_EMPTY
*           MEMORY_ALLOCATE_FAILED
*   \else on a pointer-array that store the nodes' pointer.
*   Need to be freed manually.
*   Do not free the nodes or the node->data manually.
**/
CLinkedlistNode ** CLinkedlist_forward_traversing(CLinkedlist *this);

/** Backward traversing the linked list.
*   \return error codes:
*           CLINKEDLIST_EMPTY
*           MEMORY_ALLOCATE_FAILED
*   \else on a pointer-array that store the nodes' pointer.
*   Need to be freed manually.
*   Do not free the nodes or the node->data manually.
**/
CLinkedlistNode ** CLinkedlist_backward_traversing(CLinkedlist *this);
void * CLinkedlist_reverse(CLinkedlist *this);
bool CLinkedlist_is_empty(CLinkedlist *this);
/* Get the total data size, return -1 on empty linked list, -2 on memory allocated failed */
size_t CLinkedlist_total_size(CLinkedlist *this);

/** Get the last node's pointer
*   \return error codes:
*           CLINKEDLIST_EMPTY
*   \else on node's pointer.
*   Do not free the node or the node->data manually.
**/
CLinkedlistNode * CLinkedlist_last_node(CLinkedlist *this);

/** Get the specific node's pointer
*   \return error codes:
*           CLINKEDLIST_EMPTY
*           INDEX_OUT_RANGE
*   \else on node's pointer.
*   Do not free the node or the node->data manually.
**/
CLinkedlistNode * CLinkedlist_specific_node(CLinkedlist *this, uint64_t index);
uint64_t CLinkedlist_get_length(CLinkedlist *this);

#endif