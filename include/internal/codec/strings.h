/** AUTOHEADER TAG: DELETE BEGIN **/
#ifndef _STRINGS_
#define _STRINGS_

#include "internal/common.h"
/** AUTOHEADER TAG: DELETE END **/

#define BYTEARRAY_ALL_ZERO (void *)0x000001

/** Get the length of given string.
*   \param  string      The given string.
*   \return -1 on error.
*         else on string length.

int32_t get_strlen(int8_t *string);
**/
/** Convert a hexadecimal string to a byte array.
*   \param  str         Hexadecimal string.
*   \param  str_len     String length, must be even.
*   \param  arr         Store the byte array.
*   \return  0 on success.
*           -1 on odd string length.
*           -2 on non-hexadecimal characters in the string.
*   \example: "0A1B2C" -> {'0x0A', '0X1B', '0X2C'}
**/
Status hexstr_to_bytearr(uint8_t *str, size_t str_len, BYTE *arr);

/** Convert a byte array to a hexadecimal string.
*   \param  arr         Byte array, byte's value range: -128 ~ 127.
*   \param  arr_len     The length of arr.
*   \param  str         Store the hexadecimal string.
*   \return  0 on success.
*           -1 on byte(s) value out range.
*   \example: {'0x0A', '0X1B', '0X2C'} -> "0A1B2C"
**/
Status bytearr_to_hexstr(BYTE *arr, size_t arr_len, uint8_t *str);
void bytearr_reverse(BYTE *arr, size_t size);
void hexstr_reverse(uint8_t *str, size_t len);
void * lsrtip(const void *arr, size_t arr_size, size_t *r_size);
void * rsrtip(const void *arr, size_t arr_size, size_t *r_size);

/** AUTOHEADER TAG: DELETE BEGIN **/
#endif
/** AUTOHEADER TAG: DELETE END **/