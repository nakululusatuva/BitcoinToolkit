#include "internal/crypto/bigint.h"
#include "internal/codec/strings.h"
#include <stdlib.h>
#include <string.h>

Bigint * new_Bigint()
{
	Bigint *bn = (Bigint *)calloc(1, sizeof(Bigint));
	if (bn == NULL)
		return MEMORY_ALLOCATE_FAILED;
	bn->d = NULL;
	bn->len = 0;
	bn->neg = false;
	return bn;
}

void delete_Bigint(Bigint *self)
{
	if (!(self->d))
		free(self->d);
	free(self);
}

Bigint * Bigint_set_bytearr(BYTE *arr, size_t len, bool neg)
{
	Bigint *bn = new_Bigint();
	if (bn == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

	// Strip the ending 0x00, and get the real length.
	size_t r_len;
	BYTE *r = (BYTE *)rsrtip((const BYTE *)arr, len, &r_len);
	if (r == MEMORY_ALLOCATE_FAILED) return MEMORY_ALLOCATE_FAILED;

	// Byte array all zero.
	if (r == BYTEARRAY_ALL_ZERO)
	{
		bn->d = (uint32_t *)calloc(1, sizeof(uint32_t));
		if (!(bn->d))
		{
			free(r);
			return MEMORY_ALLOCATE_FAILED;
		}
	}

	// >0 and < original len.
	else
	{
		bn->d = (uint32_t *)malloc(len%8 ? len/8+1 : len/8);
		if (!(bn->d))
		{
			free(r);
			return MEMORY_ALLOCATE_FAILED;
		}

		for (size_t i = 0; i < r_len; ++i)
		{

		}
	}
}