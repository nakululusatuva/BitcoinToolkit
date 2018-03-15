#ifndef STD_STRING
#define STD_STRING

#include "common.h"

/** Get the length of the string.
*   \param  string      The string need to be counted.
*   \return -1 on error.
*         else on string length.
**/
int32_t get_strlen(int8_t *string);

/** Convert a hexadecimal string to a byte array.
*   \param  str         Hexadecimal string.
*   \param  str_len     String length, must be even.
*   \param  arr         Store the byte array.
*   \return  0 on success.
*           -1 on odd string length.
*           -2 on not hexadecimal charaters.
*   \example: "0A1B2C" -> '0x0A', '0X1B', '0X2C'
**/
int32_t hexstr_to_bytearr(int8_t *str, size_t str_len, BYTE *arr);

/** Convert a byte array to a hexadecimal string.
*   \param  arr         Byte array, byte value range: 0x00 ~ 0xFF.
*   \param  arr_len     The length of arr.
*   \param  str         Store the hexadecimal string.
*   \return  0 on success.
*           -1 on byte value out range.
*   \example: '0x0A', '0X1B', '0X2C' -> "0A1B2C"
**/
int32_t bytearr_to_hexstr(BYTE *arr, size_t arr_len, int8_t *str);

#endif