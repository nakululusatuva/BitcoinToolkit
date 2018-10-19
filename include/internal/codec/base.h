/** AUTOHEADER TAG: DELETE BEGIN **/
#ifndef _BASE_
#define _BASE_

#include "internal/common.h"
/** AUTOHEADER TAG: DELETE END **/

/** Base6 Encoder. (Leaading '0x00' bytes will be ignored)
*   \param  payload     Byte array, bytes's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, store the encoded payload.
*   \return  0 on success.
*           -1 on byte's value out range.
*         else on encoded payload's length, if param 'encoded' is NULL.
**/
size_t base6encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base6 Decoder. (Leaading '0' characters will be ignored)
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     Byte array, store the decoded payload.
*   \return  0 on success.
*           -1 on non-base6 character(s) in the payload string.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
size_t base6decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/** Base58 Encoder. (Leaading '0x00' bytes will be replaced by character '1')
*   \param  payload     Byte array, byte's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, store the encoded payload.
*   \return  0 on success.
*           -1 on byte's value out range.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
size_t base58encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base58 Decoder. (Leaading '1' characters will be replaced by byte '0x00')
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     Byte array, store the decoded payload.
*   \return  0 on success.
*           -1 on non-base58 character(s) in the payload string.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
size_t base58decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/** Base58 Encoder. (Leaading '0x00' bytes will be replaced by character '1')
*   \param  payload     Byte array, byte's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, store the encoded payload.
*   \return  0 on success.
*           -1 on byte's value out range.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
size_t base58check_encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base58 Decoder. (Leaading '1' characters will be replaced by byte '0x00')
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     Byte array, store the decoded payload.
*   \return  0 on success.
*           -1 on non-base58 character(s) in the payload string.
*           -2 on Invalid Checksum.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
size_t base58check_decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/** Base64 Encoder.
*   \param  payload     Byte array, byte's value range: 0x00 ~ 0xFF.
*   \param  payload_len Length of the payload.
*   \param  encoded     String, store the encoded payload.
*   \return  0 on success.
*         else on decoded payload length, if param 'decoded' is NULL.
**/
size_t base64encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base64 Decoder.
*   \param  payload     The string need to be decode.
*   \param  payload_len Length of the payload string.
*   \param  decoded     Byte array, store the decoded payload.
*   \return  0 on success.
*           -1 on non-base64 character(s) in the payload string.
*         else on encoded payload length, if param 'encoded' is NULL.
**/
size_t base64decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/** AUTOHEADER TAG: DELETE BEGIN **/
#endif
/** AUTOHEADER TAG: DELETE END **/