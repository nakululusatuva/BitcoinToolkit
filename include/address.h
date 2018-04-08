#ifndef BTC_ADDRESS_H
#define BTC_ADDRESS_H

#include "common.h"

typedef struct address_st {
	int32_t  cmpr_flag;             //  0 = no, 1 = yes.
	BYTE     ver_byte;              //  1 byte long, 0x80 or 0xEF, used in WIF privkey.
	BYTE     net_byte;              //  1 byte long, 0x00 or 0x6F, used in address.
	BYTE     private_key[32];       // 32 byte long.
	BYTE     public_key[65];        // 65 byte long.
	BYTE     public_key_cmpr[33];   // 33 byte long.
	uint8_t  priv_wif[52];          // 51 charaters long + '\0' ending.
	uint8_t  priv_wif_cmpr[53];     // 52 charaters long + '\0' ending.
	uint8_t  address[35];           // 34 charaters long + '\0' ending.
} ADDRESS;

/** Generate ECDSA-secp256k1 private key.
*   \param  priv        32 bytes, store the private key.
**/
void generate_ecdsa_secp256k1_private_key(BYTE *priv);

/** Generate ECDSA-secp256k1 public key by given private key.
*   \param  priv        32 bytes, the given private key.
*   \param  pub         65/33 bytes, store the public key.
*   \param  comp_flag   Get compressed public key? 0=no 1=yes.
*   \return  0 on success.
*           -1 on wrong cmpr_flag.
**/
int32_t ecdsa_secp256k1_privkey_to_pubkey(BYTE *priv, BYTE *pub, int32_t cmpr_flag);

/** Convert private key from hexadecimal to wallet import format.
*   \param  priv        ECDSA-secp256k1 private key byte array.
*   \param  wif         Store the private key WIF.
*   \param  cmpr_flag   Compressed private key WIF? 0=no 1=yes.
*   \param  ver_byte    Network version byte, '0x80' for mainnet, '0xef' for testnet.
*   \return  0 on success.
*           -1 on wrong ver_byte.
*           -2 on wrong cmpr_flag.
**/
int32_t hex_to_wif(BYTE *priv, uint8_t *wif, int32_t cmpr_flag, BYTE ver_byte);

/** Convert private key from WIF to 32 bytes hexadecimal.
*   \param  wif         Private key WIF, uint8_t (unsigned char) string.
*   \param  private_key ECDSA-secp256k1 private key byte array.
*   \return  0 on success.
*           -1 on invalid WIF length.
*           -2 on not base58 charater in the private key WIF.
*           -3 on invalid checksum.
**/
int32_t wif_to_hex(uint8_t *wif, BYTE *private_key);

/** Convert private key from base6 to 32 bytes hexadecimal.
*   \param  b6          Private key Base6 format, Maximum 99 charaters long.
*   \param  b6_len      Length of the Base6 private key.
*   \param  private_key Decoded B6 private key, 32 bytes hexadecimal.
*   \return  0 on success.
*         else on decode B6 private key lengt, if param 'private_key' is NULL.
*                 sometimes will smaller than 32, due to the leading 0 number.
**/
int32_t b6_to_hex(uint8_t *b6, size_t b6_len, BYTE *private_key);

/** Convert public key to bitcoin address.
*   \param  public_key      ECDSA-secp256k1 public key byte array.
*   \param  cmpr_flag       Compressed ECDSA-secp256k1 public key? 0=no, 1=yes.
*   \param  network_byte    Address network ID byte, 0x00 = Main Network,
*                                                    0x6F = Test Network,
*                                                    0x34 = Namecoin Network.
*   \param  address         Bitcoin address string.
*   \return 0 on success.
*          -1 on wrong network_byte.
*          -2 on wrong cmpr_flag.
**/
int32_t pub_to_address(BYTE *public_key, int32_t cmpr_flag, BYTE network_byte, uint8_t *address);

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
*   \param  privkey     Private key string in [B6], [WIF], [Hexadecimal] format.
*   \param  length      Length of param 'privkey'.
*   \return  0 on Unsupported format.
*            1 on Standard mainnet private key.
*            2 on Compressed mainnet private key.
*            3 on Standard testnet private key.
*            4 on Compressed testnet private key.
*            5 on Hexadecimal private key.
*            6 on Base6 private key.
*           -1 on Unsupported format.
*           -2 on Invalid WIF private key.
*           -3 on Invalid Hexadecimal private key.
*           -4 on Invalid Base6 private key.
*           -5 on ecdsa-secp256k1 private key value out range.
**/
int32_t privkey_validation(int8_t *privkey, size_t length);

/** Generate an address by given parameter.
*   \param  cmpr        Compress flag, -1 = Invalid compress flag.
*                                      -2 = Invalid version byte.
*                                      -3 = Invalid network byte
*   \param  ver         Version byte, 0x80 = mainnet, 0xEF = testnet.
*   \param  net         Network byte, 0x00 = mainnet, 0x6F = testnet.
**/
ADDRESS generate_address(int32_t cmpr, BYTE ver, BYTE net);

/** Generate an address by given parameter and private key.
*   \param  cmpr        Compress flag, -1 = Invalid compress flag.
*                                      -2 = Invalid version byte.
*                                      -3 = Invalid network byte
*   \param  ver         Version byte, 0x80 = mainnet, 0xEF = testnet.
*   \param  net         Network byte, 0x00 = mainnet, 0x6F = testnet.
*   \param  hex         Private key hexadecimal, 32 bytes long.
**/
ADDRESS generate_address_by_private_key(int32_t cmpr, BYTE ver, BYTE net, BYTE *hex);

/** Convert the private key from any format to hexadecimal byte array.
*   \param  key         Any format private key string.
*   \param  cmpr        Store the compress flag, the value is 0 or 1.
*   \param  ver         Store the version byte, 1 byte long.
*   \param  net         Store the network byte, 1 byte lnog.
*   \param  hex         Store the hexadecimal byte array, 32 bytes long
*   \return  0 on cmpr_flag/ver_byte/net_byte are known.
*            1 on cmpr_flag/ver_byte/net_byte are N/A.
**/
int32_t privkey_anyformat_to_hex(int8_t *key, int32_t *cmpr, BYTE *ver, BYTE *net, BYTE *hex);

/** Print the address which stored in an ADDRESS structure.
*   \param  addr        An ADDRESS structure that stored the address.
*   It prints the address, public key, private key WIF and private key hexadecimal.
**/
void print_address(ADDRESS addr);

/** Initialize the ADDRESS structure.
*   \param addr         The ADDRESS structure.
*   \value              cmpr_flag = 127
*                       ver_byte  = 127
*                       net_byte  = 127
**/
void ADDRESS_init(ADDRESS *addr);

#endif