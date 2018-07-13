#include "../common.h"

int32_t get_strlen(int8_t *string)
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

int32_t hexstr_to_bytearr(int8_t *str, size_t str_len, BYTE *arr)
{
	int8_t high, low;

	if (str_len % 2 == 1)
		return -1;

	for (int32_t i = 0; i < str_len; i+=2)
	{
		high = str[i];
		low = str[i+1];

		if (high >= '0' && high <= '9')
			high = high - '0';
		else if (high >= 'A' && high <= 'F')
			high = high - 'A' + 10;
		else if (high >= 'a' && high <= 'f')
			high = high - 'a' + 10;
		else return -2;

		if (low >= '0' && low <= '9')
			low = low - '0';
		else if (low >= 'A' && low <= 'F')
			low = low - 'A' + 10;
		else if (low >= 'a' && low <= 'f')
			low = low - 'a' + 10;
		else return -2;

		arr[i/2] = (high << 4) | low;
	}

	return 0;
}

int32_t bytearr_to_hexstr(BYTE *arr, size_t arr_len, int8_t *str)
{
	uint8_t high, low;

	for (int32_t i = 0; i < arr_len; ++i)
	{
		if (arr[i] > 0xFF || arr[i] < 0x00)
			return -1;

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

	return 0;
}