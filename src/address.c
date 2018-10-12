#include <time.h>
#include <string.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include "internal/codec/base.h"
#include "internal/codec/strings.h"
#include "internal/common.h"
#include "internal/address.h"

void ecdsa_secp256k1_generate_private_key(BYTE *priv_raw)
{
	uint8_t bin[256];
	uint8_t hexarray[64];
	int16_t item, i = 0;

	// Get ns level time stamp for random seed.
	// Function 'clock_gettime' only works on Linux.
	struct timespec nstime;
	clock_gettime(CLOCK_MONOTONIC, &nstime);

	// Generate binary private key.
	srand(nstime.tv_nsec);
	for (uint8_t k = 0; k < 256; ++k)
		bin[k] = rand()%2;

	// Convert binary private key to raw byte array.
	do{	item = (bin[i] + 1) * 1000 + (bin[i+1] + 1) * 100 + (bin[i+2] + 1) * 10 + (bin[i+3] + 1);
		hexarray[i/4] = selector(item);
		i += 4;
	}while(i < 256);

	hexstr_to_bytearr(hexarray, 64, priv_raw);
}
Status ecdsa_secp256k1_privkey_to_pubkey(BYTE *priv_raw, BYTE *pub_raw, bool compress)
{	
	point_conversion_form_t forms[] = {
		POINT_CONVERSION_UNCOMPRESSED,
		POINT_CONVERSION_COMPRESSED
	};
	EC_KEY *keys = EC_KEY_new_by_curve_name(714);
	const EC_GROUP *group = EC_KEY_get0_group(keys);       // Struct group store the G and calculation rules
	EC_POINT *pubkey_bn = EC_POINT_new(group);             // Public key is a point on curve
	BN_CTX *ctx = BN_CTX_new();

	// Convert private key from raw byte array to BIGNUM.
	uint8_t hexstr[64];
	BIGNUM *privkey_bn = BN_new();
	bytearr_to_hexstr(priv_raw, 32, hexstr);
	BN_hex2bn(&privkey_bn, (const char*)hexstr);

	// Generate public key.
	EC_POINT_mul(group, pubkey_bn, privkey_bn, NULL, NULL, ctx);
	EC_KEY_set_private_key(keys, privkey_bn);
	EC_KEY_set_public_key(keys, pubkey_bn);

	// Convert public key from BIGNUM to raw byte array.
	EC_KEY_set_conv_form(keys, forms[compress?1:0]);
	uint8_t pub_len;
	uint8_t *ptr_pubkey_raw = pub_raw;
	pub_len = i2o_ECPublicKey(keys, &ptr_pubkey_raw);
	if (pub_len != 33 && pub_len != 65)
		return FAILED;

	BN_free(privkey_bn);
	EC_POINT_free(pubkey_bn);
	BN_CTX_free(ctx);
	EC_KEY_free(keys);
	return SUCCEEDED;
}

void raw_to_wif(BYTE *priv_raw, uint8_t *priv_wif, bool compress, NETWORK_TYPE network)
{
	uint8_t extended_length, to_base58_length;
	BYTE *extended, extended_key[33], extended_key_cmpr[34];
	BYTE *to_base58, ready_to_base58[37], ready_to_base58_cmpr[38];

	// Step 1 : Extend the private key with prefix privkey_type byte and suffix compress byte.
	switch (compress)
	{
		case true:
		{
			extended         = extended_key;
			extended[0]      = network == MAINNET ? PREFIX_PRIV_MAINNET : PREFIX_PRIV_TESTNET;
			extended_length  = 33;
			to_base58        = ready_to_base58;
			to_base58_length = 37;
			break;
		}
		case false:
		{
			extended         = extended_key_cmpr;
			extended[0]      = network == MAINNET ? PREFIX_PRIV_MAINNET : PREFIX_PRIV_TESTNET;
			extended_length  = 34;
			extended[33]     = SUFFIX_PRIV_COMPRESS;
			to_base58        = ready_to_base58_cmpr;
			to_base58_length = 38;
			break;
		}
	}
	for (uint8_t i = 1; i <= 32; ++i)
		extended[i] = priv_raw[i-1];

	// Step 2 : Double sha256 the extended key for checksum.
	BYTE first_sha256[32], second_sha256[32];
	SHA256(extended, extended_length, first_sha256);
	SHA256(first_sha256, 32, second_sha256);

	// Step 3 : Append the first four byte checksum to the extended key.
	for (uint8_t i = 0; i < extended_length; ++i)
		to_base58[i] = extended[i];
	for (uint8_t i = 0; i < 4; ++i)
		to_base58[to_base58_length - 4 + i] = second_sha256[i];

	// Step 4 : Base58 encode the 'to_base' for wallet import format.
	base58encode(to_base58, to_base58_length, priv_wif);
}

Status wif_to_raw(uint8_t *priv_wif, BYTE *priv_raw)
{
	size_t decoded_len = base58decode(priv_wif, strlen((const char *)priv_wif), NULL);
	if (decoded_len != 37 && decoded_len != 38)
		return FAILED;
	else if (decoded_len == (size_t)-1)
		return FAILED;

	BYTE decoded[decoded_len];
	base58decode(priv_wif, strlen((const char *)priv_wif), decoded);

	BYTE to_sha256[decoded_len - 4], checksum_origin[4];
	for (uint8_t i = 0; i < (decoded_len - 4); ++i)
		to_sha256[i] = decoded[i];
	for (uint8_t i = 0; i < 4; ++i)
		checksum_origin[i] = decoded[decoded_len - 4 + i];

	BYTE first_sha256[32], second_sha256[32];
	SHA256(to_sha256, decoded_len - 4, first_sha256);
	SHA256(first_sha256, 32, second_sha256);

	for (uint8_t i = 0; i < 4; ++i)
	{
		if (checksum_origin[i] != second_sha256[i])
			return FAILED;
	}

	for (uint8_t i = 0; i < 32; ++i)
		priv_raw[i] = to_sha256[i+1];

	return SUCCEEDED;
}

// fix it, the prefix 0x00 are ignored.
Status b6_to_hex(uint8_t *b6, size_t b6_len, BYTE *priv_raw)
{
	size_t raw_len = base6decode(b6, b6_len, NULL);
	if (raw_len == (size_t)-1)
		return FAILED;
	size_t prefix_zero_count = 32 - raw_len;

	BYTE copy[raw_len];
	base6decode(b6, b6_len, copy);

	for (uint8_t i = 0; i < prefix_zero_count; ++i)
		priv_raw[i] = 0x00;
	for (uint8_t i = 0; i < raw_len; ++i)
		priv_raw[i + prefix_zero_count] = copy[i];

	return SUCCEEDED;
}

void pub_to_address(BYTE *pub_raw, uint8_t *address, bool compress, ADDRESS_TYPE addr_type)
{
	BYTE pub_sha256[32], pub_ripemd160[20];

	BYTE ripemd160_result_prefixed[21], first_checksum_sha[32], second_checksum_sha[32], to_base58[25];

	// Step 1 : hash160 the public key.
	SHA256(pub_raw, compress ? 33 : 65, pub_sha256);
	RIPEMD160(pub_sha256, 32, pub_ripemd160);

	// Step 2 : Extend the hashed public key with prefix address_type byte.
	switch (addr_type)
	{
		case MAINNET_P2PKH: ripemd160_result_prefixed[0] = PREFIX_ADDR_MAINNET_P2PKH; break;
		case MAINNET_P2SH:  ripemd160_result_prefixed[0] = PREFIX_ADDR_MAINNET_P2SH;  break;
		case TESTNET_P2PKH: ripemd160_result_prefixed[0] = PREFIX_ADDR_TESTNET_P2PKH; break;
		case TESTNET_P2SH:  ripemd160_result_prefixed[0] = PREFIX_ADDR_TESTNET_P2SH;  break;
	}
	for (int32_t i = 0; i < 20; ++i)
		ripemd160_result_prefixed[i + 1] = pub_ripemd160[i];

	// Step 3 : Double sha256 the extended byte array for checksum.
	SHA256(ripemd160_result_prefixed, 21, first_checksum_sha);

	SHA256(first_checksum_sha, 32, second_checksum_sha);

	// Step 4 : Append the checksum to the extended hashed public key.
	for (int32_t i = 0; i < 21; ++i)
		to_base58[i] = ripemd160_result_prefixed[i];
	for (int32_t i = 0; i < 4; ++i)
		to_base58[21 + i] = second_checksum_sha[i];

	// Step 5 : Base58 encode the 'to_base' payload, get an address.
	base58encode(to_base58, 25, address);
}

// fix it.
Status address_to_hash160(uint8_t *address, BYTE *hash160)
{
	// Get and check the decoded data's length.
	size_t decoded_len = base58decode(address, strlen((char*)address), NULL);
	
	if (decoded_len != 25)
		return FAILED;
	else if (decoded_len == (size_t)-1)
		return FAILED;

	// b58decode the address.
	BYTE decoded[decoded_len];
	base58decode(address, strlen((char*)address), decoded);
	
	// Separate the hash160 part and checksum part.
	BYTE hash160_origin[21], checksum_origin[4];
	for (int32_t i = 0; i < 21; ++i)
		hash160_origin[i] = decoded[i];

	for (int32_t i = 0; i < 4; ++i)
		checksum_origin[i] = decoded[decoded_len-4+i];

	// Double sha256 the hash160 and check the checksum.
	BYTE first_sha256[32], second_sha256[32];
	SHA256(hash160_origin, 21, first_sha256);
	SHA256(first_sha256, 32, second_sha256);

	for (int32_t i = 0; i < 4; ++i)
	{
		if (checksum_origin[i] != second_sha256[i])
			return FAILED;
	}

	// Get the hash160 value if checksum is correct.
	for (int32_t i = 0; i < 20; ++i)
		hash160[i] = hash160_origin[i+1];

	return SUCCEEDED;
}

Status privkey_validation(uint8_t *key, size_t len, PRIVKEY_FORMAT format);

uint8_t selector(uint16_t item)
{
	switch (item)
	{
		case 1111: return '0';
		case 1112: return '1';
		case 1121: return '2';
		case 1122: return '3';
		case 1211: return '4';
		case 1212: return '5';
		case 1221: return '6';
		case 1222: return '7';
		case 2111: return '8';
		case 2112: return '9';
		case 2121: return 'A';
		case 2122: return 'B';
		case 2211: return 'C';
		case 2212: return 'D';
		case 2221: return 'E';
		case 2222: return 'F';
	}
	return 0xff;
}

P2PKH_Main_Address * new_P2PKH_Main(bool compress)
{
	P2PKH_Main_Address *new = (P2PKH_Main_Address *)calloc(1, sizeof(P2PKH_Main_Address));
	if (!new) return MEMORY_ALLOCATE_FAILED;

	ecdsa_secp256k1_generate_private_key(new->root.priv_raw);
	ecdsa_secp256k1_privkey_to_pubkey(new->root.priv_raw, compress?new->root.pubc_raw:new->root.pub_raw, compress);
	raw_to_wif(new->root.priv_raw, new->b58_address, compress, MAINNET);
	new->compress = compress;

	return new;
}
P2PKH_Main_Address * new_P2PKH_Main_from_key(const uint8_t *anyformat, bool compress);
Status delete_P2PKH_Main(P2PKH_Main_Address *this);
Status Initialize_P2PKH_Main(P2PKH_Main_Address *addr, bool compress);
Status Abandon_P2PKH_Main(P2PKH_Main_Address *addr);