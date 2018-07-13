#ifndef BTC_ADDRESS_H
#define BTC_ADDRESS_H

#define MAX_PRIVKEY_WIF_SM "5Km2kuu7vtFDPpxywn4u3NLpbr5jKpTB3jsuDU2KYEqetqj84qw"
#define MAX_PRIVKEY_WIF_CM "L5oLkpV3aqBjhki6LmvChTCV6odsp4SXM6FfU2Gppt5kFLaHLuZ9"
#define MAX_PRIVKEY_WIF_ST "93XfLeifX7KMMtUGa7xouxtnFWSSUyzNPgjrJ6Npsyahfqjy7oJ"
#define MAX_PRIVKEY_WIF_CT "cWALDjUu1tszsCBMjBjL4mhYj2wHUWYDR0Q8aSjLKzjkW5eBtpzu"
#define MAX_PRIVKEY_HEX "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140"
#define MAX_PRIVKEY_B6  "1021410542201502023034020312354303525141003020114142003134301540433233134222423333133255354344331040"

#define PRIVATE_KEY_MAINNET_BYTE_PREFIX 0x80
#define PRIVATE_KEY_TESTNET_BYTE_PREFIX 0xEF
#define PRIVATE_KEY_COMPRESS_BYTE_SUFFIX 0X01
#define ADDRESS_MAINNET_PUBKEY_HASH_BYTE_PREFIX 0x00
#define ADDRESS_MAINNET_SCRIPT_HASH_BYTE_PREFIX 0x05
#define ADDRESS_TESTNET_PUBKEY_HASH_BYTE_PREFIX 0x6F
#define ADDRESS_TESTNET_SCRIPT_HASH_BYTE_PREFIX 0xC4

#include "common.h"

typedef struct address_st {
	int32_t  cmpr_flag;             //  0 = no, 1 = yes.
	BYTE     privkey_type;          //  1 byte long.
	BYTE     address_type;          //  1 byte long.
	BYTE     privkey[32];           // 32 byte long.
	BYTE     pubkey[65];            // 65 byte long.
	BYTE     pubkey_cmpr[33];       // 33 byte long.
	uint8_t  privkey_wif[52];       // 51 charaters long + '\0' ending.
	uint8_t  privkey_wif_cmpr[53];  // 52 charaters long + '\0' ending.
	uint8_t  address[36];           // 34 or 35 charaters long + '\0' ending.
} ADDRESS;

/** Generate a ECDSA-secp256k1 private key.
*   \param  privkey_raw  32 bytes long, store the raw private key.
**/
void generate_ecdsa_secp256k1_private_key(BYTE *privkey_raw);

/** Generate a ECDSA-secp256k1 public key by given raw private key.
*   \param  privkey_raw  32 bytes long, the given raw private key.
*   \param  pubkey_raw   65 or 33 bytes long, store the public key.
*   \param  comp_flag    Compressed public key? 0=no 1=yes.
*   \return  0 on success.
*           -1 on invalid cmpr_flag.
**/
int32_t ecdsa_secp256k1_privkey_to_pubkey(BYTE *privkey_raw, BYTE *pubkey_raw, int32_t cmpr_flag);

/** Convert a private key from raw to wallet import format.
*   \param  privkey_raw   ECDSA-secp256k1 private key byte array.
*   \param  privkey_wif   34 characters long string, store the private key WIF.
*   \param  cmpr_flag     Compressed private key WIF? 0=no 1=yes.
*   \param  privkey_type  Prefix byte, '0x80' for mainnet, '0xef' for testnet.
*   \return  0 on success.
*           -1 on invalid privkey_type.
*           -2 on invalid cmpr_flag.
**/
int32_t raw_to_wif(BYTE *privkey_raw, uint8_t *privkey_wif, int32_t cmpr_flag, BYTE privkey_type);

/** Convert a private key from WIF to raw byte array.
*   \param  privkey_wif  String, private key WIF.
*   \param  privkey_raw  ECDSA-secp256k1 private key byte array.
*   \return  0 on success.
*           -1 on invalid WIF length.
*           -2 on non-base58 charater in the private key WIF.
*           -3 on invalid checksum.
**/
int32_t wif_to_raw(uint8_t *privkey_wif, BYTE *privkey_raw);

/** Convert a private key from base6 to raw byte array.
*   \param  b6           String, private key B6 format, 99 or 100 charaters long.
*   \param  b6_len       Length of the B6 private key.
*   \param  privkey_raw  Byte array, store the raw private key.
*   \return  0 on success.
*         else on raw private key length, if param 'privkey_raw' is NULL.
*                 sometimes will smaller than 32, due to the leading 0 number.
**/
int32_t b6_to_hex(uint8_t *b6, size_t b6_len, BYTE *privkey_raw);

/** Convert a public key to bitcoin address.
*   \param  pubkey_raw    ECDSA-secp256k1 public key byte array.
*   \param  cmpr_flag     Compressed ECDSA-secp256k1 public key? 0=no, 1=yes.
*   \param  address_type  Address type
*   \param  address       String, 34 characters long, store the address.
*   \return  0 on success.
*           -1 on invalid address_type.
*           -2 on invalid cmpr_flag.
**/
int32_t pub_to_address(BYTE *pubkey_raw, int32_t cmpr_flag, BYTE address_type, uint8_t *address);

/** Get a bitcoin address's hash160 value.
*   \param  address     String, bitcoin address.
*   \param  hash160     Byte array, 20 bytes long, store the hash160 value.
*   \return  0 on success.
*           -1 on invalid charaters in the address.
*           -2 on invalid address length.
*           -3 on invalid checksum.
**/
int32_t address_to_hash160(uint8_t *address, BYTE *hash160);

/** Get the private key format and check validation.
*   \param  anyformat   String, private key in [B6], [WIF], [Hexadecimal] format.
*   \param  length      Character length of 'anyformat'.
*   \return  0 on Unsupported format.
*           -1 on EC key value out range.
*           -2 on unsupported format.
*           -3 on invalid base58 string.
*           -4 on invalid checksum.
*           -5 on invalid HEX string.
*           -6 on invalid B6 string.
*           -7 on invalid B64 string.
*         0x00 on valid WIF   format
*         0x01 on valid HEX   format.
*         0x02 on valid B6    format.
*         0x03 on valid B64   format.
*         0x04 on valid BIP38 format
**/
int32_t privkey_validation(int8_t *anyformat, size_t length);

/** Generate an address by given private key type and address type.
*   \param  cmpr_flag     Compressed address? 0=no 1=yes.
*   \param  privkey_type  Private key type byte prefix.
*   \param  address_type  Address key type byte prefix.
**/
ADDRESS generate_address(int32_t cmpr_flag, BYTE privkey_type, BYTE address_type);

/** Generate an address by given private key type, address type and raw private key.
*   \param  cmpr_flag     Compressed address? 0=no 1=yes.
*   \param  privkey_type  Private key type byte prefix.
*   \param  address_type  Address key type byte prefix.
*   \param  privkey_raw   Raw private key.
**/
ADDRESS generate_address_by_private_key(int32_t cmpr_flag, BYTE privkey_type, BYTE address_type, BYTE *privkey_raw);

/** Convert the private key from any format to hexadecimal byte array.
*   \param  anyformat     Any format private key string.
*   \param  cmpr_flag     Store the compress flag, 0 or 1.
*   \param  privkey_type  Store the private key type prefix byte, 1 byte long.
*   \param  address_type  Store the address type prefix byte, 1 byte long.
*   \param  privkey_raw   Store the raw private key.
*   \return  0 on cmpr_flag/privkey_type/address_type are known.
*            1 on cmpr_flag/privkey_type/address_type are N/A.
**/
int32_t anyformat_to_raw(int8_t *anyformat, int32_t *cmpr_flag, BYTE *privkey_type, BYTE *address_type, BYTE *privkey_raw);

/** Print the address.
*   \param  addr  An ADDRESS structure that stored the address.
*   It prints the address, public key, private key WIF and private key hexadecimal.
**/
void print_address(ADDRESS addr);

#endif