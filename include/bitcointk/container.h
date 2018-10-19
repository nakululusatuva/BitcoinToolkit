/** 
*  MIT LICENSE
*  Copyright (c) 2018 Yirain Suen
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to
*  deal in the Software without restriction, including without limitation the
*  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
*  sell copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*  
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*  
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
*  IN THE SOFTWARE.
**/
/** 
*  AUTO-GENERATED CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
*  Maintainers: Do not include this header internal to this library.
**/
#ifdef __cpluscplus
extern "C" {
#endif
#include "common.h"


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
void delete_CLinkedlist(CLinkedlist *self);



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
void delete_CStack(CStack *self);

#ifdef __cpluscplus
}
#endif
