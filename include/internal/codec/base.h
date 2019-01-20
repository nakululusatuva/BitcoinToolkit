/** AUTOHEADER TAG: DELETE BEGIN **/
#ifndef _BASE_
#define _BASE_

#include "internal/common.h"
/** AUTOHEADER TAG: DELETE END **/

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

/** Base 2^8 to Base 2^32
 *  \param  payload     An integer represented in a base 2^8 array (Little-endian).
 *  \param  payload_len Length of the payload.
 *  \param  converted   Store the converted number, represented in a base 2^32 array (Little-endian).
 *                      Array's item will be reset to zero by memset().
 *  \return  0 on success.
 *        else on converted array's length, if param 'converted' is NULL.
**/
size_t base2p8_to_base2p32(uint8_t *payload, size_t payload_len, uint32_t *converted);

/** Base 2^32 to Base 2^8
 *  \param  payload     An integer represented in a base 2^32 array (Little-endian).
 *  \param  payload_len Length of the payload.
 *  \param  converted   Store the converted number, represented in a base 2^8 array (Little-endian).
 *                      Array's item will be reset to zero by memset().
 *  \return  0 on success.
 *        else on converted array's length, if param 'converted' is NULL.
**/
size_t base2p32_to_base2p8(uint32_t *payload, size_t payload_len, uint8_t *converted);

/** Base 2^16 to Base 2^32
 *  \param  payload     An integer represented in a base 2^16 array (Little-endian).
 *  \param  payload_len Length of the payload.
 *  \param  converted   Store the converted number, represented in a base 2^32 array (Little-endian).
 *                      Array's item will be reset to zero by memset().
 *  \return  0 on success.
 *        else on converted array's length, if param 'converted' is NULL.
**/
size_t base2p16_to_base2p32(uint16_t *payload, size_t payload_len, uint32_t *converted);

/** Base 2^32 to Base 2^16
 *  \param  payload     An integer represented in a base 2^32 array (Little-endian).
 *  \param  payload_len Length of the payload.
 *  \param  converted   Store the converted number, represented in a base 2^16 array (Little-endian).
 *                      Array's item will be reset to zero by memset().
 *  \return  0 on success.
 *        else on converted array's length, if param 'converted' is NULL.
**/
size_t base2p32_to_base2p16(uint32_t *payload, size_t payload_len, uint16_t *converted);

/** AUTOHEADER TAG: DELETE BEGIN **/
#endif
/** AUTOHEADER TAG: DELETE END **/