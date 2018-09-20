#include "status.h"
#include "common.h"

bool __btc_is_status_ptr(void *ptr)
{
	if ((uintptr_t)ptr >= 0x0000 && (uintptr_t)ptr <= 0xffff)
		return true;
	else return false;
}

void * __btc_status_ptr(const uintptr_t errnum)
{
	return (void *)errnum;
}

uintptr_t __btc_status_num(const void *errptr)
{
	return (uintptr_t)errptr;
}