#include "bigint.h"
#include "../status.h"
#include "../codec/codec.h"
#include <stdlib.h>
#include <string.h>

Bigint * new_Bigint()
{
	Bigint *bn = (Bigint *)calloc(1, sizeof(Bigint));
	if (bn == NULL)
		return MEMORY_ALLOCATE_FAILED;
	bn->d = NULL;
	bn->top = 0;
	bn->neg = false;
	return bn;
}

void delete_Bigint(Bigint *this)
{
	if (!(this->d))
		free(this->d);
	free(this);
}