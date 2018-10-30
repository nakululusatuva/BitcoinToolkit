#include "internal/codec/strings.h"
#include <string.h>
#include <stdlib.h>
/**
int32_t get_strlen(uint8_t *string)
{
	int32_t length = 0;

	for (int32_t i = 0; ; ++i)
	{
		if (string[i] != '\0')
			length++;
		else break;
	}

	return length;
}
**/
// fix it
Status hexstr_to_bytearr(uint8_t *str, size_t str_len, BYTE *arr)
{
	uint8_t high, low;

	if (str_len % 2 == 1)
		return FAILED;

	for (size_t i = 0; i < str_len; i+=2)
	{
		high = str[i];
		low = str[i+1];

		if (high >= '0' && high <= '9')
			high = high - '0';
		else if (high >= 'A' && high <= 'F')
			high = high - 'A' + 10;
		else if (high >= 'a' && high <= 'f')
			high = high - 'a' + 10;
		else return FAILED;

		if (low >= '0' && low <= '9')
			low = low - '0';
		else if (low >= 'A' && low <= 'F')
			low = low - 'A' + 10;
		else if (low >= 'a' && low <= 'f')
			low = low - 'a' + 10;
		else return FAILED;

		arr[i/2] = (high << 4) | low;
	}

	return SUCCEEDED;
}

// fix it
Status bytearr_to_hexstr(BYTE *arr, size_t arr_len, uint8_t *str)
{
	uint8_t high, low;

	for (size_t i = 0; i < arr_len; ++i)
	{
		if (arr[i] > 0xFF || arr[i] < 0x00)
			return FAILED;

		high = arr[i];
		low = arr[i];

		high = high >> 4;
		low = low << 4;
		low = low >> 4;

		if (high >= 0x00 && high <= 0x09)
			str[i*2] = high + 0x30;
		else if (high >= 0X0A && high <= 0x0F)
			str[i*2] = high + 0x37;

		if (low >= 0x00 && low <= 0x09)
			str[i*2+1] = low + 0x30;
		else if (low >= 0X0A && low <= 0X0F)
			str[i*2+1] = low + 0x37;
	}
	str[arr_len*2] = '\0';

	return SUCCEEDED;
}

void bytearr_reverse(BYTE *arr, size_t size)
{
	size_t len = 0;
	if (size % 2 != 0) len = (size-1)/2;
	for (size_t i = 0; i < len; ++i)
	{
		BYTE buffer = arr[i];
		arr[i] = arr[size-1-i];
		arr[size-1-i] = buffer;
	}
}

// finish it.
void hexstr_reverse(uint8_t *str, size_t len)
{

}

void * lstrip(const void *arr, size_t arr_size, size_t *r_size)
{
	size_t count = 0;
	for (size_t i = 0; i < arr_size; ++i)
	{
		if ( ((BYTE *)arr)[i] == 0x00 )
			count++;
	}
	r_size[0] = arr_size-count;

	if (count == arr_size) return BYTEARRAY_ALL_ZERO; // All zero.

	void *r = (void *)malloc(r_size[0]);
	if (!r) return MEMORY_ALLOCATE_FAILED;
	memcpy(r, arr+count, r_size[0]);

	return r;
}

void * rsrtip(const void *arr, size_t arr_size, size_t *r_size)
{
	size_t i, count = 0;
	for (i = arr_size-1; i > 0; --i)
	{
		if ( ((BYTE *)arr)[i] == 0x00 )
			count++;
	}
	if ( ((BYTE *)arr)[0] == 0x00 && i == 0 )
		count++;
	if (count == arr_size) return BYTEARRAY_ALL_ZERO; // All zero.

	*r_size = arr_size-count;

	void *r = (void *)malloc(*r_size);
	if (!r) return MEMORY_ALLOCATE_FAILED;
	memcpy(r, arr, *r_size);

	return r;
}

Status integer_swap(void *a, void *b, void *dtype)
{
	switch ((uintptr_t)dtype)
	{
		case (0): return PASSING_NULL_POINTER;
		case (uintptr_t)BYTE_TYPE:
		{
			*(BYTE*)a = *(BYTE*)a ^ *(BYTE*)b;
			*(BYTE*)b = *(BYTE*)b ^ *(BYTE*)a;
			*(BYTE*)a = *(BYTE*)a ^ *(BYTE*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)INT8_TYPE:
		{
			*(int8_t*)a = *(int8_t*)a ^ *(int8_t*)b;
			*(int8_t*)b = *(int8_t*)b ^ *(int8_t*)a;
			*(int8_t*)a = *(int8_t*)a ^ *(int8_t*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)INT16_TYPE:
		{
			*(int16_t*)a = *(int16_t*)a ^ *(int16_t*)b;
			*(int16_t*)b = *(int16_t*)b ^ *(int16_t*)a;
			*(int16_t*)a = *(int16_t*)a ^ *(int16_t*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)INT32_TYPE:
		{
			*(int32_t*)a = *(int32_t*)a ^ *(int32_t*)b;
			*(int32_t*)b = *(int32_t*)b ^ *(int32_t*)a;
			*(int32_t*)a = *(int32_t*)a ^ *(int32_t*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)INT64_TYPE:
		{
			*(int64_t*)a = *(int64_t*)a ^ *(int64_t*)b;
			*(int64_t*)b = *(int64_t*)b ^ *(int64_t*)a;
			*(int64_t*)a = *(int64_t*)a ^ *(int64_t*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)UINT8_TYPE:
		{
			*(uint8_t*)a = *(uint8_t*)a ^ *(uint8_t*)b;
			*(uint8_t*)b = *(uint8_t*)b ^ *(uint8_t*)a;
			*(uint8_t*)a = *(uint8_t*)a ^ *(uint8_t*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)UINT16_TYPE:
		{
			*(uint16_t*)a = *(uint16_t*)a ^ *(uint16_t*)b;
			*(uint16_t*)b = *(uint16_t*)b ^ *(uint16_t*)a;
			*(uint16_t*)a = *(uint16_t*)a ^ *(uint16_t*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)UINT32_TYPE:
		{
			*(uint32_t*)a = *(uint32_t*)a ^ *(uint32_t*)b;
			*(uint32_t*)b = *(uint32_t*)b ^ *(uint32_t*)a;
			*(uint32_t*)a = *(uint32_t*)a ^ *(uint32_t*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)UINT64_TYPE:
		{
			*(uint64_t*)a = *(uint64_t*)a ^ *(uint64_t*)b;
			*(uint64_t*)b = *(uint64_t*)b ^ *(uint64_t*)a;
			*(uint64_t*)a = *(uint64_t*)a ^ *(uint64_t*)b;
			return SUCCEEDED;
		}
		case (uintptr_t)BOOL_TYPE:
		{
			*(bool*)a = *(bool*)a ^ *(bool*)b;
			*(bool*)b = *(bool*)b ^ *(bool*)a;
			*(bool*)a = *(bool*)a ^ *(bool*)b;
			return SUCCEEDED;
		}
		default: 
			return UNSUPPORTED_DATATYPE;
	}
}