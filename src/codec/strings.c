#include "../status.h"
#include "../common.h"
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

// fix it
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

size_t lstrip(void *arr, size_t size, void *r)
{
	size_t count = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if ( ((BYTE *)arr)[i] == 0x00 )
			count++;
	}
	if (!r) return size-count; // If r is NULL, return leading 0 count.

	if (count >= 0 && count < size)
	{
		for (size_t i = 0; i < size-count; ++i)
		{
			((BYTE *)r)[i] = ((BYTE *)arr)[i+count];
		}
		return 0;
	}
	else return count;  // count == size, all zero, do nothing.
}

size_t rsrtip(void *arr, size_t size, void *r)
{
	size_t count = 0;
	for (size_t i = size-1; i > 0; --i)
	{
		if ( ((BYTE *)arr)[i] == 0x00 )
			count++;
	}
	if ( ((BYTE *)arr)[0] == 0x00 )
		count++;
	if (!r) return size-count; // If r is NULL, return ending 0 count.
	
	if (count >= 0 && count < size)
	{
		for (size_t i = 0; i < size-count; ++i)
		{
			((BYTE *)r)[i] = ((BYTE *)arr)[i];
		}
		return 0;
	}
	else return count; // count == size, all zero, do nothing.
}