#ifndef BTC_ADDRESS_H
#define BTC_ADDRESS_H

#define MAX_PRIVKEY_WIF_SM "5Km2kuu7vtFDPpxywn4u3NLpbr5jKpTB3jsuDU2KYEqetqj84qw"
#define MAX_PRIVKEY_WIF_CM "L5oLkpV3aqBjhki6LmvChTCV6odsp4SXM6FfU2Gppt5kFLaHLuZ9"
#define MAX_PRIVKEY_WIF_ST "93XfLeifX7KMMtUGa7xouxtnFWSSUyzNPgjrJ6Npsyahfqjy7oJ"
#define MAX_PRIVKEY_WIF_CT "cWALDjUu1tszsCBMjBjL4mhYj2wHUWYDR8Q8aSjLKzjkW5eBtpzu"
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
	BYTE     privkey_type;              //  1 byte long, 0x80 or 0xEF, used in WIF privkey.
	BYTE     address_type;              //  1 byte long, 0x00 or 0x6F, used in address.
	BYTE     privkey[32];       // 32 byte long.
	BYTE     pubkey[65];        // 65 byte long.
	BYTE     pubkey_cmpr[33];   // 33 byte long.
	uint8_t  privkey_wif[52];          // 51 charaters long + '\0' ending.
	uint8_t  privkey_wif_cmpr[53];     // 52 charaters long + '\0' ending.
	uint8_t  address[35];           // 34 charaters long + '\0' ending.
} ADDRESS;

/** Generate ECDSA-secp256k1 private key.
*   \param  privkey_raw        32 bytes, store the private key.
**/
void generate_ecdsa_secp256k1_private_key(BYTE *privkey_raw);

/** Generate ECDSA-secp256k1 public key by given private key.
*   \param  privkey_raw        32 bytes, the given private key.
*   \param  pubkey_raw         65/33 bytes, store the public key.
*   \param  comp_flag   Get compressed public key? 0=no 1=yes.
*   \return  0 on success.
*           -1 on wrong cmpr_flag.
**/
int32_t ecdsa_secp256k1_privkey_to_pubkey(BYTE *privkey_raw, BYTE *pubkey_raw, int32_t cmpr_flag);

/** Convert private key from hexadecimal to wallet import format.
*   \param  privkey_raw        ECDSA-secp256k1 private key byte array.
*   \param  privkey_wif         Store the private key WIF.
*   \param  cmpr_flag   Compressed private key WIF? 0=no 1=yes.
*   \param  privkey_type    Network version byte, '0x80' for mainnet, '0xef' for testnet.
*   \return  0 on success.
*           -1 on wrong privkey_type.
*           -2 on wrong cmpr_flag.
**/
int32_t raw_to_wif(BYTE *privkey_raw, uint8_t *privkey_wif, int32_t cmpr_flag, BYTE privkey_type);

/** Convert private key from WIF to 32 bytes hexadecimal.
*   \param  privkey_wif         Private key WIF, uint8_t (unsigned char) string.
*   \param  privkey_raw ECDSA-secp256k1 private key byte array.
*   \return  0 on success.
*           -1 on invalid WIF length.
*           -2 on not base58 charater in the private key WIF.
*           -3 on invalid checksum.
**/
int32_t wif_to_raw(uint8_t *privkey_wif, BYTE *privkey_raw);

/** Convert private key from base6 to 32 bytes hexadecimal.
*   \param  b6          Private key Base6 format, Maximum 99 charaters long.
*   \param  b6_len      Length of the Base6 private key.
*   \param  privkey_raw Decoded B6 private key, 32 bytes hexadecimal.
*   \return  0 on success.
*         else on decode B6 private key lengt, if param 'privkey_raw' is NULL.
*                 sometimes will smaller than 32, due to the leading 0 number.
**/
int32_t b6_to_hex(uint8_t *b6, size_t b6_len, BYTE *privkey_raw);

/** Convert public key to bitcoin address.
*   \param  pubkey_raw      ECDSA-secp256k1 public key byte array.
*   \param  cmpr_flag       Compressed ECDSA-secp256k1 public key? 0=no, 1=yes.
*   \param  privkey_type    Address network ID byte, 0x00 = Main Network,
*                                                    0x6F = Test Network,
*                                                    0x34 = Namecoin Network.
*   \param  address         Bitcoin address string.
*   \return 0 on success.
*          -1 on wrong privkey_type.
*          -2 on wrong cmpr_flag.
**/
int32_t pub_to_address(BYTE *pubkey_raw, int32_t cmpr_flag, BYTE privkey_type, uint8_t *address);

/** Get bitcoin address's hash160 value.
*   \param  address     Bitcoin address string.
*   \param  hash160     hash160 value of the bitcoin address, 20 bytes long.
*   \return  0 on success.
*           -1 on invalid charaters in the address.
*           -2 on invalid address length.
*           -3 on invalid checksum.
**/
int32_t address_to_hash160(uint8_t *address, BYTE *hash160);

/** Get the private key format and check validation.
*   \param  anyformat     Private key string in [B6], [WIF], [Hexadecimal] format.
*   \param  length      Length of param 'anyformat'.
*   \return  0 on Unsupported format.
**/
int32_t privkey_validation(int8_t *anyformat, size_t length);

/** Generate an address by given parameter.
*   \param  cmpr_flag        Compress flag, -1 = Invalid compress flag.
*                                      -2 = Invalid version byte.
*                                      -3 = Invalid network byte
*   \param  privkey_type         Version byte, 0x80 = mainnet, 0xEF = testnet.
*   \param  address_type         Network byte, 0x00 = mainnet, 0x6F = testnet.
**/
ADDRESS generate_address(int32_t cmpr_flag, BYTE privkey_type, BYTE address_type);

/** Generate an address by given parameter and private key.
*   \param  cmpr_flag        Compress flag, -1 = Invalid compress flag.
*                                      -2 = Invalid version byte.
*                                      -3 = Invalid network byte
*   \param  privkey_type         Version byte, 0x80 = mainnet, 0xEF = testnet.
*   \param  address_type         Network byte, 0x00 = mainnet, 0x6F = testnet.
*   \param  privkey_raw         Private key hexadecimal, 32 bytes long.
**/
ADDRESS generate_address_by_private_key(int32_t cmpr_flag, BYTE privkey_type, BYTE address_type, BYTE *privkey_raw);

/** Convert the private key from any format to hexadecimal byte array.
*   \param  anyformat         Any format private key string.
*   \param  cmpr_flag        Store the compress flag, the value is 0 or 1.
*   \param  privkey_type         Store the version byte, 1 byte long.
*   \param  address_type         Store the network byte, 1 byte lnog.
*   \param  privkey_raw         Store the hexadecimal byte array, 32 bytes long
*   \return  0 on cmpr_flag/privkey_type/address_type are known.
*            1 on cmpr_flag/privkey_type/address_type are N/A.
**/
int32_t anyformat_to_raw(int8_t *anyformat, int32_t *cmpr_flag, BYTE *privkey_type, BYTE *address_type, BYTE *privkey_raw);

/** Print the address which stored in an ADDRESS structure.
*   \param  addr        An ADDRESS structure that stored the address.
*   It prints the address, public key, private key WIF and private key hexadecimal.
**/
void print_address(ADDRESS addr);

#endif