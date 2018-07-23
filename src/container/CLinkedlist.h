#ifndef _CLINKEDLIST_
#define _CLINKEDLIST_

#include "../common.h"

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

	void (*add)(CLinkedlist *, void *, size_t);
	bool (*delete)(CLinkedlist *, uint64_t);
	bool (*insert)(CLinkedlist *, uint64_t, void *);
	bool (*change)(CLinkedlist *, uint64_t, void *);
	CLinkedlistNode ** (*forward_traversing)(CLinkedlist *);
	CLinkedlistNode ** (*backward_traversing)(CLinkedlist *);
	bool (*reverse)(CLinkedlist *);
	bool (*is_empty)(CLinkedlist *);
};

// Construct and Destruct Functions.
CLinkedlist * new_CLinkedlist();
bool delete_CLinkedlist(CLinkedlist *self);

// Inner Fuctions.
CLinkedlistNode * CLinkedlist_last_node(CLinkedlist *self);
CLinkedlistNode * CLinkedlist_specific_node(CLinkedlist *self, uint64_t index);

// Member Fuctions.
void CLinkedlist_add(CLinkedlist *self, void *data, size_t size);
bool CLinkedlist_delete(CLinkedlist *self, uint64_t index);
bool CLinkedlist_insert(CLinkedlist *self, uint64_t after, void *data);
bool CLinkedlist_change(CLinkedlist *self, uint64_t index, void *data);
CLinkedlistNode ** CLinkedlist_forward_traversing(CLinkedlist *self);
CLinkedlistNode ** CLinkedlist_backward_traversing(CLinkedlist *self);
bool CLinkedlist_reverse(CLinkedlist *self);
bool CLinkedlist_is_empty(CLinkedlist *self);

#endif