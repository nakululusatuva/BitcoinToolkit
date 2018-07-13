#ifndef _STRING_H_
#define _STRING_H_

#include "../common.h"

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
*   \param  arr         Byte array, byte value range: -128 ~ 127.
*   \param  arr_len     The length of arr.
*   \param  str         Store the hexadecimal string.
*   \return  0 on success.
*           -1 on byte value out range.
*   \example: '0x0A', '0X1B', '0X2C' -> "0A1B2C"
**/
int32_t bytearr_to_hexstr(BYTE *arr, size_t arr_len, int8_t *str);

#endif

#ifndef _BASE_H_
#define _BASE_H_

/** Base6 Encoder. (Leaading '0x00' bytes will be ignored)
*   \param  payload     Byte array, item's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, encoded payload.
*   \return 0 on success.
*           -1 on item's value out range.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
int32_t base6encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base6 Decoder. (Leaading '0' charaters will be ignored)
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     Decoded data in byte array.
*   \return  0 on success.
*           -1 on not base6 charater in the payload string.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
int32_t base6decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/** Base58 Encoder. (Leaading '0x00' bytes will be replaced by charater '1')
*   \param  payload     Byte array, item's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, encoded payload.
*   \return  0 on success.
*           -1 on item's value out range.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
int32_t base58encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base58 Decoder. (Leaading '1' charaters will be replaced by byte '0x00')
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     Decoded data in byte array.
*   \return  0 on success.
*           -1 on not base58 charater in the payload string.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
int32_t base58decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/** Base58Check Encoder. (Leaading '0x00' bytes will be replaced by charater '1')
*   \param  payload     Byte array, item's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, encoded payload.
*   \return  0 on success.
*           -1 on item's value out range.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
int32_t base58check_encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base58Check Decoder. (Leaading '1' charaters will be replaced by byte '0x00')
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     Decoded data in byte array.
*   \return  0 on success.
*           -1 on not base58 charater in the payload string.
*           -2 on Invalid Checksum.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
int32_t base58check_decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/** Base64 Encoder.
*   \param  payload     Byte array, item's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, encoded payload.
*   \return  0 on success.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
int32_t base64encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base64 Decoder.
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     Decoded data in byte array.
*   \return  0 on success.
*           -1 on item's value out range.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
int32_t base64decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

#endif