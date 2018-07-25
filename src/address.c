#include <time.h>
#include <string.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include "codec/codec.h"
#include "common.h"
#include "address.h"

int8_t selector(int32_t item);

int32_t ecdsa_secp256k1_privkey_to_pubkey(BYTE *privkey_raw, BYTE *pubkey_raw, int32_t cmpr_flag)
{
	int32_t pub_len = 0;
	int32_t NID_secp256k1 = 714;
	int8_t  privkey_str[64];
	uint8_t *ptr_pubkey_raw = pubkey_raw;

	point_conversion_form_t forms[] = {
		POINT_CONVERSION_UNCOMPRESSED,
		POINT_CONVERSION_COMPRESSED
	};

	BIGNUM *privkey_bn = BN_new();
	EC_KEY *keys = EC_KEY_new_by_curve_name(NID_secp256k1);

	const EC_GROUP *group = EC_KEY_get0_group(keys);       // Struct group store the G and calculation rules
	EC_POINT *pubkey_bn = EC_POINT_new(group);             // Public key is a point on curve
	BN_CTX *ctx = BN_CTX_new();

	if (cmpr_flag != 0 && cmpr_flag != 1)
		return -1;

	// Convert private key from raw byte array to BIGNUM.
	bytearr_to_hexstr(privkey_raw, 32, privkey_str);
	BN_hex2bn(&privkey_bn, (const char*)privkey_str);

	// Generate public key.
	EC_POINT_mul(group, pubkey_bn, privkey_bn, NULL, NULL, ctx);
	EC_KEY_set_private_key(keys, privkey_bn);
	EC_KEY_set_public_key(keys, pubkey_bn);

	// Convert public key from BIGNUM to raw byte array.
	EC_KEY_set_conv_form(keys, forms[cmpr_flag]);
	pub_len = i2o_ECPublicKey(keys, &ptr_pubkey_raw);

	if (pub_len != 33 && pub_len != 65)
		return -2;

	BN_free(privkey_bn);
	EC_POINT_free(pubkey_bn);
	BN_CTX_free(ctx);
	EC_KEY_free(keys);

	return 0;
}

void generate_ecdsa_secp256k1_private_key(BYTE *privkey_raw)
{
	int8_t bin[256];
	int8_t privkey_rawstr[64];
	int32_t item, i = 0;

	// Get ns level time stamp for random seed.
	// Function 'clock_gettime' might only works on Linux.
	struct timespec nstime;
	clock_gettime(CLOCK_MONOTONIC, &nstime);

	// Generate binary private key.
	srand(nstime.tv_nsec);
	for (int32_t k = 0; k < 256; ++k)
		bin[k] = rand()%2;

	// Convert binary private key to raw byte array.
	do{	item = (bin[i] + 1) * 1000 + (bin[i+1] + 1) * 100 + (bin[i+2] + 1) * 10 + (bin[i+3] + 1);
		privkey_rawstr[i/4] = selector(item);
		i += 4;
	}while(i < 256);

	hexstr_to_bytearr(privkey_rawstr, 64, privkey_raw);
}

int8_t selector(int32_t item)
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
		default : return -1;
	}
}

int32_t raw_to_wif(BYTE *privkey_raw, uint8_t *privkey_wif, int32_t cmpr_flag, BYTE privkey_type)
{
	int extended_length, to_base58_length;

	BYTE *extended, extended_key[33], extended_key_cmpr[34];

	BYTE first_sha256[32], second_sha256[32];

	BYTE *to_base58, ready_to_base58[37], ready_to_base58_cmpr[38];

	if (privkey_type != PRIVATE_KEY_MAINNET_BYTE_PREFIX &&
		privkey_type != PRIVATE_KEY_TESTNET_BYTE_PREFIX)
		return -1;

	// Step 1 : Extend the private key with prefix privkey_type byte and suffix compress byte.
	if (cmpr_flag == 0)
	{
		extended          = extended_key;
		extended[0]      = privkey_type;
		extended_length  = 33;
		to_base58        = ready_to_base58;
		to_base58_length = 37;
	}
	else if (cmpr_flag == 1)
	{
		extended         = extended_key_cmpr;
		extended[0]      = privkey_type;
		extended_length  = 34;
		extended[33]     = PRIVATE_KEY_COMPRESS_BYTE_SUFFIX;
		to_base58        = ready_to_base58_cmpr;
		to_base58_length = 38;
	}
	else return -2;
	
	for (int32_t i = 1; i <= 32; ++i)
		extended[i] = privkey_raw[i-1];

	// Step 2 : Double sha256 the extended key for checksum.
	SHA256(extended, extended_length, first_sha256);

	SHA256(first_sha256, 32, second_sha256);

	// Step 3 : Append the first four byte checksum to the extended key.
	for (int32_t i = 0; i < extended_length; ++i)
		to_base58[i] = extended[i];

	for (int32_t i = 0; i < 4; ++i)
		to_base58[to_base58_length - 4 + i] = second_sha256[i];

	// Step 4 : Base58 encode the 'to_base' for wallet import format.
	base58encode(to_base58, to_base58_length, privkey_wif);

	return 0;
}

int32_t wif_to_raw(uint8_t *privkey_wif, BYTE *privkey_raw)
{
	int32_t decoded_len = base58decode(privkey_wif, get_strlen((int8_t*)privkey_wif), NULL);
	BYTE to_sha256[decoded_len - 4], checksum_origin[4];
	BYTE first_sha256[32], second_sha256[32];

	if (decoded_len != 37 && decoded_len != 38)
		return -1;
	else if (decoded_len == -1)
		return -2;

	BYTE decoded[decoded_len];
	base58decode(privkey_wif, get_strlen((int8_t*)privkey_wif), decoded);

	for (int32_t i = 0; i < (decoded_len - 4); ++i)
		to_sha256[i] = decoded[i];
	for (int32_t i = 0; i < 4; ++i)
		checksum_origin[i] = decoded[decoded_len - 4 + i];

	SHA256(to_sha256, decoded_len - 4, first_sha256);
	SHA256(first_sha256, 32, second_sha256);

	for (int32_t i = 0; i < 4; ++i)
	{
		if (checksum_origin[i] != second_sha256[i])
			return -3;
	}

	for (int32_t i = 0; i < 32; ++i)
		privkey_raw[i] = to_sha256[i+1];

	return 0;
}

int32_t b6_to_raw(uint8_t *b6, size_t b6_len, BYTE *privkey_raw)
{
	int32_t raw_len;
	int32_t prefix_zero_count;

	raw_len = base6decode(b6, b6_len, NULL);
	prefix_zero_count = 32 - raw_len;

	if (privkey_raw == NULL)
		return raw_len;

	BYTE copy[raw_len];
	base6decode(b6, b6_len, copy);

	for (int32_t i = 0; i < prefix_zero_count; ++i)
		privkey_raw[i] = 0x00;
	for (int32_t i = 0; i < raw_len; ++i)
		privkey_raw[i + prefix_zero_count] = copy[i];

	return 0;
}

int32_t pub_to_address(BYTE *pubkey_raw, int32_t cmpr_flag, BYTE address_type, uint8_t *address)
{
	/* April 1st 2018 21:46
	** delete start
		if (network_byte != 0x00 && network_byte != 0x6f && network_byte != 0x34)
	** delete end
	*/
	if (address_type != ADDRESS_MAINNET_PUBKEY_HASH_BYTE_PREFIX &&
		address_type != ADDRESS_TESTNET_PUBKEY_HASH_BYTE_PREFIX)
		return -1;
	if (cmpr_flag != 0 && cmpr_flag != 1)
		return -2;

	BYTE pub_sha256[32], pub_ripemd160[20];

	BYTE address_type_added[21], first_checksum_sha[32], second_checksum_sha[32], to_base58[25];

	// Step 1 : hash160 the public key.
	SHA256(pubkey_raw, cmpr_flag ? 33 : 65, pub_sha256);

	RIPEMD160(pub_sha256, 32, pub_ripemd160);

	// Step 2 : Extend the hashed public key with prefix address_type byte.
	address_type_added[0] = address_type;
	for (int32_t i = 0; i < 20; ++i)
		address_type_added[i + 1] = pub_ripemd160[i];

	// Step 3 : Double sha256 the extended byte array for checksum.
	SHA256(address_type_added, 21, first_checksum_sha);

	SHA256(first_checksum_sha, 32, second_checksum_sha);

	// Step 4 : Append the checksum to the extended hashed public key.
	for (int32_t i = 0; i < 21; ++i)
		to_base58[i] = address_type_added[i];
	for (int32_t i = 0; i < 4; ++i)
		to_base58[21 + i] = second_checksum_sha[i];

	// Step 5 : Base58 encode the 'to_base' payload, get an address.
	base58encode(to_base58, 25, address);

	return 0;
}

int32_t address_to_hash160(uint8_t *address, BYTE *hash160)
{
	int32_t decoded_len;
	BYTE hash160_origin[21], checksum_origin[4];
	BYTE first_sha256[32], second_sha256[32];

	// Get and check the decoded data's length.
	decoded_len = base58decode(address, get_strlen((int8_t*)address), NULL);
	
	if (decoded_len != 25)
		return -2;
	else if (decoded_len == -1)
		return -1;

	// b58decode the address.
	BYTE decoded[decoded_len];
	base58decode(address, get_strlen((int8_t*)address), decoded);
	
	// Separate the hash160 part and checksum part.
	for (int32_t i = 0; i < 21; ++i)
		hash160_origin[i] = decoded[i];

	for (int32_t i = 0; i < 4; ++i)
		checksum_origin[i] = decoded[decoded_len-4+i];

	// Double sha256 the hash160 and check the checksum.
	SHA256(hash160_origin, 21, first_sha256);
	SHA256(first_sha256, 32, second_sha256);

	for (int32_t i = 0; i < 4; ++i)
	{
		if (checksum_origin[i] != second_sha256[i])
			return -3;
	}

	// Get the hash160 value if checksum is correct.
	for (int32_t i = 0; i < 20; ++i)
		hash160[i] = hash160_origin[i+1];

	return 0;
}

int32_t privkey_validation(int8_t *anyformat, size_t length)
{
	switch(length)
	{
	// Check if WIF.
		case 51:case 52:
		{
			// Check if a valid WIF string.
			BYTE privkey_raw[32];
			int32_t ret = wif_to_raw((uint8_t*)anyformat, privkey_raw);

			if (ret == -1)
				return -3;             // Invalid WIF string.
			else if (ret == -2)
				return -3;             // Invalid WIF string.
			else if (ret == -3)
				return -4;             // Invalid checksum.
			else if (ret == 0)         // Valid WIF format string.
			{
				// Check if key's value out range.
				BIGNUM *privkey_bn = BN_new();
				BIGNUM *EC_MAX  = BN_new();
				int8_t privkey_rawstr[64];

				bytearr_to_hexstr(privkey_raw, 32, privkey_rawstr);
				BN_hex2bn(&privkey_bn, (const char *)privkey_rawstr);
				BN_hex2bn(&EC_MAX , (const char *)MAX_PRIVKEY_HEX);
				int32_t ret_cmp = BN_cmp(privkey_bn, EC_MAX);

				BN_free(privkey_bn);
				BN_free(EC_MAX);
				if (ret_cmp == -1)
					return 0x00;       // Not out range.
				else return -1;        // out range.
			}

			break;
		}

	// Check if HEX.
		case 64:
		{
			BIGNUM *privkey_bn = BN_new();
			BIGNUM *EC_MAX  = BN_new();

			int32_t ret = BN_hex2bn(&privkey_bn, (const char *)anyformat);

			// Check if a valid hexadecimal string.
			if (ret == 64)
			{
				// Check if key's value out range.
				BN_hex2bn(&EC_MAX, (const char *)MAX_PRIVKEY_HEX);
				int32_t ret_cmp = BN_cmp(privkey_bn, EC_MAX);
				BN_free(privkey_bn);
				BN_free(EC_MAX);
				if (ret_cmp == -1)
					return 0x01;       // Not out range.
				else return -1;        // out range.
			}

			else if (ret == 0)
			{
				// Invalid Hexadecimal string.
				BN_free(privkey_bn);
				BN_free(EC_MAX);
				return -5;
			}

			else {
				// Invalid Hexadecimal string.
				BN_free(privkey_bn);
				BN_free(EC_MAX);
				return -5;
			}

			break;
		}

	// Check if B6.
		case 99:case 100: {
			int32_t ret = base6decode((uint8_t *)anyformat, length, NULL);

			// Check if a valid Base6 string.
			if (ret == 32)
			{
				// Check if key's value out range.
				BIGNUM *privkey_bn = BN_new();
				BIGNUM *EC_MAX  = BN_new();
				int8_t privkey_rawstr[64];
				BYTE privkey_raw[32];

				base6decode((uint8_t *)anyformat, length, privkey_raw);
				bytearr_to_hexstr(privkey_raw, 32, privkey_rawstr);
				BN_hex2bn(&privkey_bn, (const char *)privkey_rawstr);
				BN_hex2bn(&EC_MAX , (const char *)MAX_PRIVKEY_HEX);
				int32_t ret_cmp = BN_cmp(privkey_bn, EC_MAX);

				BN_free(privkey_bn);
				BN_free(EC_MAX);
				if (ret_cmp == -1)
					return 0x02;       // Not out range.
				else return -1;        // out range.
			}

			else if (ret == -1)
				return -6;             // Invalid Base6 string.
			else return -6;            // Invalid Base6 string.

			break;
		}

	// Check if Base64.
		case 44: {
			int32_t ret = base64decode((uint8_t *)anyformat, 44, NULL);

			// Check if a valid Base64 string.
			if (ret == 32)
			{
				// Check if key's value out range.
				BIGNUM *privkey_bn = BN_new();
				BIGNUM *EC_MAX  = BN_new();
				int8_t privkey_rawstr[64];
				BYTE privkey_raw[32];

				base6decode((uint8_t *)anyformat, length, privkey_raw);
				bytearr_to_hexstr(privkey_raw, 32, privkey_rawstr);
				BN_hex2bn(&privkey_bn, (const char *)privkey_rawstr);
				BN_hex2bn(&EC_MAX , (const char *)MAX_PRIVKEY_HEX);
				int32_t ret_cmp = BN_cmp(privkey_bn, EC_MAX);

				BN_free(privkey_bn);
				BN_free(EC_MAX);
				if (ret_cmp == -1)
					return 0x03;       // Not out range.
				else return -1;        // out range.
			}

			else if (ret == -1)
				return -7;             // Invalid Base64 string.
			else return -7;            // Invalid Base64 string.

			break;
		}

	// Check if BIP38
		case 58: {
			break;
		}

		default: return -2;
	}
	return -2;
}

int32_t anyformat_to_raw(int8_t *anyformat, int32_t *cmpr_flag, BYTE *privkey_type, BYTE *address_type, BYTE *privkey_raw)
{
	int32_t ret;
	ret = privkey_validation(anyformat, get_strlen(anyformat));

	switch(ret)
	{
	// WIF key
		case 0x00:
		{
			if (anyformat[0] == '5')
			{
				*privkey_type  = PRIVATE_KEY_MAINNET_BYTE_PREFIX;
				*address_type  = ADDRESS_MAINNET_PUBKEY_HASH_BYTE_PREFIX;
				*cmpr_flag = 0;
				wif_to_raw((uint8_t *)anyformat, privkey_raw);
				return 0;
			}
			else if (anyformat[0] == 'K' || anyformat[0] == 'L')
			{
				*privkey_type  = PRIVATE_KEY_MAINNET_BYTE_PREFIX;
				*address_type  = ADDRESS_MAINNET_PUBKEY_HASH_BYTE_PREFIX;
				*cmpr_flag = 1;
				wif_to_raw((uint8_t *)anyformat, privkey_raw);
				return 0;
			}
			else if (anyformat[0] == '9')
			{
				*privkey_type  = PRIVATE_KEY_TESTNET_BYTE_PREFIX;
				*address_type  = ADDRESS_TESTNET_PUBKEY_HASH_BYTE_PREFIX;
				*cmpr_flag = 0;
				wif_to_raw((uint8_t *)anyformat, privkey_raw);
				return 0;
			}
			else if (anyformat[0] == 'c')
			{
				*privkey_type  = PRIVATE_KEY_TESTNET_BYTE_PREFIX;
				*address_type  = ADDRESS_TESTNET_PUBKEY_HASH_BYTE_PREFIX;
				*cmpr_flag = 1;
				wif_to_raw((uint8_t *)anyformat, privkey_raw);
				return 0;
			}
			break;
		}

	// HEX key
		case 0x01:
		{
			hexstr_to_bytearr(anyformat, 64, privkey_raw);
			return 1;
			break;
		}

	// B6 key
		case 0x02:
		{
			b6_to_raw((uint8_t*)anyformat, get_strlen(anyformat), privkey_raw);
			return 1;
			break;
		}

	// B64 key
		case 0x03:
		{
			base64decode((uint8_t*)anyformat, 44, privkey_raw);
			return 1;
			break;
		}

	// BIP38 key
		case 0x04:
		{
			break;
		}
	}

	return 0;
}

ADDRESS generate_address(int32_t cmpr_flag, BYTE privkey_type, BYTE address_type)
{
	ADDRESS new;

	new.cmpr_flag = cmpr_flag;
	new.privkey_type = privkey_type;
	new.address_type = address_type;

	generate_ecdsa_secp256k1_private_key(new.privkey);
	ecdsa_secp256k1_privkey_to_pubkey(new.privkey, new.cmpr_flag ? new.pubkey_cmpr : new.pubkey, new.cmpr_flag);
	raw_to_wif(new.privkey, new.cmpr_flag ? new.privkey_wif_cmpr : new.privkey_wif, new.cmpr_flag, new.privkey_type);
	pub_to_address(new.cmpr_flag ? new.pubkey_cmpr : new.pubkey, new.cmpr_flag, new.address_type, new.address);

	return new;
}

ADDRESS generate_address_by_private_key(int32_t cmpr_flag, BYTE privkey_type, BYTE address_type, BYTE *hex)
{
	ADDRESS new;

	new.cmpr_flag = cmpr_flag;
	new.privkey_type = privkey_type;
	new.address_type = address_type;
	for (int32_t i = 0; i < 32; ++i)
		new.privkey[i] = hex[i];

	ecdsa_secp256k1_privkey_to_pubkey(new.privkey, new.cmpr_flag ? new.pubkey_cmpr : new.pubkey, new.cmpr_flag);
	raw_to_wif(new.privkey, new.cmpr_flag ? new.privkey_wif_cmpr : new.privkey_wif, new.cmpr_flag, new.privkey_type);
	pub_to_address(new.cmpr_flag ? new.pubkey_cmpr : new.pubkey, new.cmpr_flag, new.address_type, new.address);

	return new;
}

void print_address(ADDRESS addr)
{
	printf("Address:\n");
	printf("%s\n\n", addr.address);

	printf("Private Key WIF:\n");
	printf("%s\n\n", addr.cmpr_flag?addr.privkey_wif_cmpr:addr.privkey_wif);

	printf("Public Key Hexadecimal:\n");
	if (addr.cmpr_flag == 0)
	{
		for (int32_t i = 0; i < 65; ++i)
		printf("%02X", addr.pubkey[i]);
	}
	else if (addr.cmpr_flag == 1)
	{
		for (int32_t i = 0; i < 33; ++i)
		printf("%02X", addr.pubkey_cmpr[i]);
	}
	printf("\n\n");

	printf("Private Key Hexadecimal:\n");
	for (int32_t i = 0; i < 32; ++i)
		printf("%02X", addr.privkey[i]);
	printf("\n");
}