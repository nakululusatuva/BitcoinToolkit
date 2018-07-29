#ifndef _CLINKEDLIST_
#define _CLINKEDLIST_

#include "../common.h"

/** Common Type Linked List.
*   1. It stores the data's pointer, instead of the data itself.
*   2. Please make sure the data's memory is allocated manually,
*      errors will happen if you add a pointer that point to stack memory into the linked list.
*   3. Do not free the data manually once it's pointer was add into the linked list,
*      the destruct function will do the job.
*   4. If you change a node's data pointer, the old data itself will be freed.
**/

typedef struct CLinkedlistNode CLinkedlistNode;
struct CLinkedlistNode
{
	CLinkedlistNode *previous;
	void *data;
	size_t size; // How many bytes.
	CLinkedlistNode *next;
};

typedef struct CLinkedlist CLinkedlist;
struct CLinkedlist
{
	CLinkedlistNode *head;
	uint64_t length;

	bool (*add)(CLinkedlist *, void *, size_t);
	bool (*delete)(CLinkedlist *, uint64_t);
	bool (*insert)(CLinkedlist *, uint64_t, void *, size_t);
	bool (*change)(CLinkedlist *, uint64_t, void *, size_t);
	CLinkedlistNode ** (*forward_traversing)(CLinkedlist *);
	CLinkedlistNode ** (*backward_traversing)(CLinkedlist *);
	bool (*reverse)(CLinkedlist *);
	bool (*is_empty)(CLinkedlist *);
	size_t (*total_size)(CLinkedlist *);
	CLinkedlistNode * (*last_node)(CLinkedlist *);
	CLinkedlistNode * (*specific_node)(CLinkedlist *, uint64_t);
};

// Construct and Destruct Functions.
CLinkedlist * new_CLinkedlist();
void delete_CLinkedlist(CLinkedlist *this);

// Member Fuctions.
/** Add a data's pointer.
*   \param  data        Data's pointer.
*   \param  size        Data's size, how many bytes.
*   \return true on success.
*          false on error.
**/
bool CLinkedlist_add(CLinkedlist *this, void *data, size_t size);

/** Delete a node.
*   \param  index       Node's position, start from zero.
*   \return true on success.
*          false on error.
**/
bool CLinkedlist_delete(CLinkedlist *this, uint64_t index);

/** Insert a data's pointer.
*   \param  after       Node's position, the new node will be in front of it.
*   \param  data        Data's pointer.
*   \param  size        Data's size, how many bytes.
*   \return true on success.
*          false on error.
**/
bool CLinkedlist_insert(CLinkedlist *this, uint64_t after, void *data, size_t size);

/** Change specific node's data.
*   \param  index       Node's position, the node you want to edit.
*   \param  data        New data's pointer, the old data itself will be freed.
*   \param  size        New data's size, how many bytes.
*   \return true on success.
*          false on error.
**/
bool CLinkedlist_change(CLinkedlist *this, uint64_t index, void *data, size_t size);

/** Forward traversing the linked list.
*   \return NUll on error.
*           else on a pointer-array that store the nodes' pointer.
*   Need to be freed manually.
**/
CLinkedlistNode ** CLinkedlist_forward_traversing(CLinkedlist *this);

/** Backward traversing the linked list.
*   \return NUll on error.
*           else on a pointer-array that store the nodes' pointer.
*   Need to be freed manually.
**/
CLinkedlistNode ** CLinkedlist_backward_traversing(CLinkedlist *this);
bool CLinkedlist_reverse(CLinkedlist *this);
bool CLinkedlist_is_empty(CLinkedlist *this);
size_t CLinkedlist_total_size(CLinkedlist *this);
CLinkedlistNode * CLinkedlist_last_node(CLinkedlist *this);
CLinkedlistNode * CLinkedlist_specific_node(CLinkedlist *this, uint64_t index);

#endif