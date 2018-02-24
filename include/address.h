#ifndef BTC_ADDRESS_H
#define BTC_ADDRESS_H

#include "std.h"

typedef struct address_st {
	int32_t  cmpr_flag;
	BYTE 	 ver_byte;				//  1 byte long.
	BYTE 	 net_byte;				//  1 byte long.
	BYTE 	 private_key[32];		// 32 byte long.
	BYTE 	 public_key[65];		// 65 byte long.
	BYTE 	 public_key_cmpr[33];	// 33 byte long.
	uint8_t  priv_wif[52];			// 51 charaters long + '\0' ending.
	uint8_t  priv_wif_cmpr[53];		// 52 charaters long + '\0' ending.
	uint8_t  address[35];			// 34 charaters long + '\0' ending.
} ADDRESS;

/** Generate a ECDSA-secp256k1 key pair.
*	\param	priv 		32 bytes, store the private key.
*	\param	pub 		65/33 bytes, store the public key.
*	\param	comp_flag	Get compressed public key? 0=no 1=yes.
*	\return  0 on success.
*			-1 on wrong cmpr_flag.
*			-2 otherwise.
**/
int32_t generate_ecdsa_secp256k1_key_pair(uint8_t *priv, uint8_t *pub, int32_t cmpr_flag);

/** Convert private key from hexadecimal to wallet import format.
*	\param	priv 		ECDSA-secp256k1 private key byte array.
*	\param	wif 		Store the private key WIF.
*	\param	cmpr_flag 	Compressed private key WIF? 0=no 1=yes.
*	\param	ver_byte	Network version byte, '0x80' for mainnet, '0xef' for testnet.
*	\return  0 on success.
*			-1 on wrong ver_byte.
*			-2 on wrong cmpr_flag.
**/
int32_t hex_to_wif(BYTE *priv, uint8_t *wif, int32_t cmpr_flag, BYTE ver_byte);

/** Decode private key from WIF to 32 bytes hexadecimal.
*	\param	wif 		Private key WIF, uint8_t (unsigned char) string.
*	\param	private_key	ECDSA-secp256k1 private key byte array.
*	\return  0 on success.
*			-1 on invalid WIF length.
*			-2 on not base58 charater in the private key WIF.
*			-3 on invalid checksum.
**/
int32_t wif_to_hex(uint8_t *wif, BYTE *private_key);

/** Convert public key to bitcoin address.
*	\param	public_key 		ECDSA-secp256k1 public key byte array.
*	\param	cmpr_flag 		Compressed ECDSA-secp256k1 public key? 0=no, 1=yes.
*	\param	network_byte	Address network ID byte, 0x00 = Main Network,
*													 0x6F = Test Network,
*													 0x34 = Namecoin Network.
*	\param	address 		Bitcoin address string.
*	\return  0 on success.
*			-1 on wrong network_byte.
*			-2 on wrong cmpr_flag.
**/
int32_t pub_to_address(BYTE *public_key, int32_t cmpr_flag, BYTE network_byte, uint8_t *address);

/** Get bitcoin address's hash160 value.
*	\param	address 	Bitcoin address string.
*	\param	hash160 	hash160 value of the bitcoin address.
*	\return  0 on success.
*			-1 on invalid bitcoin address.
**/
int32_t address_to_hash160(uint8_t *address, BYTE *hash160);

#endif