#ifndef BASE_H
#define BASE_H

#include "std.h"

/**
**/
int32_t base6encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/**
**/
int32_t base6decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/**
**/
int32_t base32encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/**
**/
int32_t base32decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/** Base58 Encoder.
*	\param	payload 	String, item's value range: 0x00 ~ 0xFF.
*	\param	payload_len	Length of the payload string.
*	\param	encoded 	String, encoded payload.
*	\return	 0 on success.
*			-1 on item's value out range.
*		  else on encoded payload length, if param 'encoded' is NULL.
**/
int32_t base58encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base58 Decoder.
*	\param	payload 	The string need to be decode.
*	\param	payload_len	Length of the payload string.
*	\param	decoded 	Decoded data in byte array.
*	\return  0 on success.
*			-1 on not base58 charater in the payload string.
*		  else on decoded payload length, if param 'decoded' is NULL.
**/
int32_t base58decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

/**
**/
int32_t base58check_encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/**
**/
int32_t base58check_decode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/** Base64 Encoder.
*	\param	payload 	String, item's value range: 0x00 ~ 0xFF.
*	\param	payload_len	Length of the payload string.
*	\param	encoded 	String, encoded payload.
*	\return	 0 on success.
8		  else on decoded payload length, if param 'decoded' is NULL.
**/
int32_t base64encode(BYTE *payload, size_t payload_len, uint8_t *encoded);

/**
**/
int32_t base64decode(uint8_t *payload, size_t payload_len, BYTE *decoded);

#endif