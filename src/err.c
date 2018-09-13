#include "err.h"
#include "common.h"

bool iserr(void *ptr)
{
	if ((uintptr_t)ptr >= 0x000000 && (uintptr_t)ptr <= 0xffffff)
		return true;
	else return false;
}

void * errptr(const uintptr_t errnum)
{
	return (void *)errnum;
}

uintptr_t errnum(const void *errptr)
{
	return (uintptr_t)errptr;
}