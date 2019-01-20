/** 
*  MIT LICENSE
*  Copyright (c) 2018 Yirain Suen
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to
*  deal in the Software without restriction, including without limitation the
*  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
*  sell copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*  
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*  
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
*  IN THE SOFTWARE.
**/
/** 
*  AUTO-GENERATED CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
*  Maintainers: Do not include this header internal to this library.
**/
#ifdef __cpluscplus
extern "C" {
#endif
#include "common.h"


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
Status hexstr_to_bytearr(uint8_t *str, size_t str_len, byte *arr);

/** Convert a byte array to a hexadecimal string.
*   \param  arr         byte array, byte's value range: -128 ~ 127.
*   \param  arr_len     The length of arr.
*   \param  str         Store the hexadecimal string.
*   \return  0 on success.
*           -1 on byte(s) value out range.
*   \example: {'0x0A', '0X1B', '0X2C'} -> "0A1B2C"
**/
Status bytearr_to_hexstr(byte *arr, size_t arr_len, uint8_t *str);
void bytearr_reverse(byte *arr, size_t size);
void hexstr_reverse(uint8_t *str, size_t len);
void * lsrtip(const void *arr, size_t arr_size, size_t *r_size);
void * rsrtip(const void *arr, size_t arr_size, size_t *r_size);
Status integer_swap(void *a, void *b, void *dtype);



/** Base6 Encoder. (Leaading '0x00' bytes will be ignored)
*   \param  payload     byte array, bytes's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, store the encoded payload.
*   \return  0 on success.
*           -1 on byte's value out range.
*         else on encoded payload's length, if param 'encoded' is NULL.
**/
size_t base6encode(byte *payload, size_t payload_len, uint8_t *encoded);

/** Base6 Decoder. (Leaading '0' characters will be ignored)
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     byte array, store the decoded payload.
*   \return  0 on success.
*           -1 on non-base6 character(s) in the payload string.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
size_t base6decode(uint8_t *payload, size_t payload_len, byte *decoded);

/** Base58 Encoder. (Leaading '0x00' bytes will be replaced by character '1')
*   \param  payload     byte array, byte's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, store the encoded payload.
*   \return  0 on success.
*           -1 on byte's value out range.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
size_t base58encode(byte *payload, size_t payload_len, uint8_t *encoded);

/** Base58 Decoder. (Leaading '1' characters will be replaced by byte '0x00')
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     byte array, store the decoded payload.
*   \return  0 on success.
*           -1 on non-base58 character(s) in the payload string.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
size_t base58decode(uint8_t *payload, size_t payload_len, byte *decoded);

/** Base58 Encoder. (Leaading '0x00' bytes will be replaced by character '1')
*   \param  payload     byte array, byte's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, store the encoded payload.
*   \return  0 on success.
*           -1 on byte's value out range.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
size_t base58check_encode(byte *payload, size_t payload_len, uint8_t *encoded);

/** Base58 Decoder. (Leaading '1' characters will be replaced by byte '0x00')
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     byte array, store the decoded payload.
*   \return  0 on success.
*           -1 on non-base58 character(s) in the payload string.
*           -2 on Invalid Checksum.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
size_t base58check_decode(uint8_t *payload, size_t payload_len, byte *decoded);

/** Base64 Encoder.
*   \param  payload     byte array, byte's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, store the encoded payload.
*   \return  0 on success.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
size_t base64encode(byte *payload, size_t payload_len, uint8_t *encoded);

/** Base64 Decoder.
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     byte array, store the decoded payload.
*   \return  0 on success.
*           -1 on non-base64 character(s) in the payload string.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
size_t base64decode(uint8_t *payload, size_t payload_len, byte *decoded);

size_t base2p8_to_base2p32(uint8_t *payload, size_t payload_len, uint32_t *converted);
size_t base2p32_to_base2p8(uint32_t *payload, size_t payload_len, uint8_t *converted);

/** Base 2^16 to Base 2^32
 *  \param  payload     An integer represented in an base 2^16 array (Little-endian).
 *  \param  payload_len Length of the payload.
 *  \param  converted   Store the converted number, represented in an base 2^32 array (Little-endian).
 *                      Array's item will be reset to zero by memset().
 *  \return  0 on success.
 *        else on converted array's length, if param 'converted' is NULL.
**/
size_t base2p16_to_base2p32(uint16_t *payload, size_t payload_len, uint32_t *converted);

/** Base 2^16 to Base 2^32
 *  \param  payload     An integer represented in an base 2^16 array (Little-endian).
 *  \param  payload_len Length of the payload.
 *  \param  converted   Store the converted number, represented in an base 2^32 array (Little-endian).
 *                      Array's item will be reset to zero by memset().
 *  \return  0 on success.
 *        else on converted array's length, if param 'converted' is NULL.
**/
size_t base2p32_to_base2p16(uint32_t *payload, size_t payload_len, uint16_t *converted);

#ifdef __cpluscplus
}
#endif
