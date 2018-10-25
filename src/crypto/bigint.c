#include "internal/crypto/ntt.h"
#include "internal/crypto/bigint.h"
#include "internal/codec/strings.h"
#include <stdlib.h>
#include <string.h>

// Vertical calculation O(n)
// r's max length = the longest + 1.
void d_add(const uint32_t *a, uint32_t a_len, const uint32_t *b, uint32_t b_len, uint32_t *r)
{
	const uint32_t *longest = a_len>=b_len ? a : b;
	const uint32_t *shortest = a_len>=b_len ? b : a;
	uint32_t longs = a_len>=b_len ? a_len : b_len;
	uint32_t shorts = a_len>=b_len ? b_len : a_len;

	bool carry = false;
	for (uint32_t i = 0; i < longs; ++i) // Iterate on the upper number.
	{
		// i>=shorts?(uint64_t)0:shortest[i]
		// If iterator doesn't reach the end of the lower number yet, it's the corresponding lower digit.
		// If iterator reached or past the end of the lower number, it's 0.
		uint64_t buff = longest[i] + (i>=shorts?(uint64_t)0:shortest[i]) + (uint64_t)carry;
	//	printf("%08x %08x %d %016lx\n", longest[i], i>=shorts?0:shortest[i], carry, buff);
		if (buff > UINT32_MAX)
		{
			r[i] = buff-(UINT32_MAX+0x01);
			carry = true;
		}
		else if (buff <= UINT32_MAX)
		{
			carry = false;
			r[i] = buff;
		}
	}
}

// Vertical calculation O(n)
// r's max length = the longest.
void d_sub(const uint32_t *a, uint32_t a_len, const uint32_t *b, uint32_t b_len, uint32_t *r)
{
	// Check which one is bigger.
	const uint32_t *bigger = NULL;
	const uint32_t *smaller = NULL;
	uint32_t longs = 0;
	uint32_t shorts = 0;
	int8_t ret = d_equal(a, a_len, b, b_len);
	if (ret == -1)
	{
		bigger = a; smaller = b; longs = a_len; shorts = b_len;
	}
	else if (ret == 1)
	{
		bigger = b; smaller = a; longs = b_len; shorts = a_len;
	}

	bool borrow = false;
	for (uint32_t i = 0; i < longs; ++i) // Iterate on the upper number.
	{
		uint64_t buff = bigger[i] - (uint64_t)borrow;
		if (i < shorts) // Iterator doesn't reach the end of the lower number yet.
		{
			if (bigger[i] > 0 && buff >= smaller[i])
			{
				r[i] = buff - smaller[i];
				borrow = false;
			}
			else if (bigger[i] > 0 && buff < smaller[i])
			{
				r[i] = buff + UINT32_MAX + 0x01 - smaller[i];
				borrow = true;
			}
			else if (bigger[i] == 0 && borrow)
			{
				r[i] = UINT32_MAX - smaller[i];
				borrow = true;
			}
			else // (bigger[i] == 0 && !borrow)
			{
				r[i] = UINT32_MAX + 0x01 -  smaller[i];
				borrow = true;
			}
		}
		else // (s >= shorts) // Iterator reached or past the end of the lower number.
		{
			if (bigger[i] > 0)
			{
				r[i] = buff;
				borrow = false;
			}
			else
			{
				r[i] = buff;
				borrow = true;
			}
		}
	}
}

// NTT algorithm O(nlogn)
// r's max length = a_len + b_len.
void d_mul(const uint32_t *a, uint32_t a_len, const uint32_t *b, uint32_t b_len, uint32_t *r)
{
	
}

// Vertical calculation O(n^2)
// r's max length = the longest.
void d_div(const uint32_t *a, uint32_t a_len, const uint32_t *b, uint32_t b_len, uint32_t *r)
{

}

int8_t d_equal(const uint32_t *a, uint32_t a_len, const uint32_t *b, uint32_t b_len)
{
	if (a_len > b_len)
		return -1;
	else if (a_len < b_len)
		return 1;
	else if (a_len == b_len)
	{
		if (a[a_len-1] > b[b_len-1])
			return -1;
		else if (a[a_len-1] < b[b_len-1])
			return 1;
	}
	else
		return 0;
	return -2; // Error.
}

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
	size_t stripped_len;
	BYTE *stripped = (BYTE *)rsrtip((const BYTE *)arr, len, &stripped_len);
	if (stripped == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

	// Set bn->len and bn->neg.
	bn->len = stripped_len%8 ? stripped_len/8+1 : stripped_len/8;
	bn->neg = neg;

	// Byte array all zero.
	if (stripped == BYTEARRAY_ALL_ZERO)
	{
		bn->d = (uint32_t *)calloc(1, sizeof(uint32_t));
		if (!(bn->d))
		{
			free(stripped);
			return MEMORY_ALLOCATE_FAILED;
		}
	}
	else
	{
		bn->d = (uint32_t *)calloc(bn->len, sizeof(uint32_t));
		if (!(bn->d))
		{
			free(stripped);
			return MEMORY_ALLOCATE_FAILED;
		}
		for (size_t i = 0; i < stripped_len; ++i)
		{
			(bn->d)[i/4] |= stripped[i];
			if ((i+1)%4)
				(bn->d)[i/4] <<= 8;
		}
	}
	return bn;
}

Bigint * Bigint_add(Bigint *a, Bigint *b)
{
	Bigint *bn = new_Bigint();
	if (bn == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

	bn->len = a->len + b->len + 1; // Temp.
	bn->d = (uint32_t *)calloc(bn->len, sizeof(uint32_t)); // Temp.

	if (a->neg && b->neg)
	{

	}
	else if (!a->neg && b->neg)
	{

	}
	else if (a->neg && !b->neg)
	{

	}
	else // !a->neg && !b->neg
	{

	}

	// Strip the ending 0 in bn->d and reset bn->len.

	return bn;
}