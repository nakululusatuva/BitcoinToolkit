#ifndef _CLINKEDLIST_
#define _CLINKEDLIST_

#include "../common.h"

typedef struct CLinkedlistNode CLinkedlistNode;
struct CLinkedlistNode
{
	CLinkedlistNode *previous;
	void *data;
	CLinkedlistNode *next;
};

typedef struct CLinkedlist CLinkedlist;
struct CLinkedlist
{
	CLinkedlistNode *head;
	uint64_t length;

	void (*add)(CLinkedlist *, void *);
	bool (*delete)(CLinkedlist *, uint64_t);
	bool (*insert)(CLinkedlist *, uint64_t, void *);
	bool (*change)(CLinkedlist *, uint64_t, void *);
	void ** (*forward_traversing)(CLinkedlist *);
	void ** (*backward_traversing)(CLinkedlist *);
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
void CLinkedlist_add(CLinkedlist *self, void *data);
bool CLinkedlist_delete(CLinkedlist *self, uint64_t index);
bool CLinkedlist_insert(CLinkedlist *self, uint64_t after, void *data);
bool CLinkedlist_change(CLinkedlist *self, uint64_t index, void *data);
void ** CLinkedlist_forward_traversing(CLinkedlist *self);
void ** CLinkedlist_backward_traversing(CLinkedlist *self);
bool CLinkedlist_reverse(CLinkedlist *self);
bool CLinkedlist_is_empty(CLinkedlist *self);

#endif