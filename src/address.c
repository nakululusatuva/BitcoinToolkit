#include <time.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include "std.h"
#include "base.h"

void genpriv(int8_t *priv_str);
int8_t selector(int32_t item);

int32_t hex_to_wif(BYTE *priv, uint8_t *wif, int32_t cmpr_flag, BYTE ver_byte)
{
	BYTE *extended, extended_key[33], extended_key_cmpr[34];

	BYTE first_sha256[32], second_sha256[32];

	BYTE *to_base58, ready_to_base58[37], ready_to_base58_cmpr[38];

	if (ver_byte != 0x80 && ver_byte != 0xef)
		return -1;

	/* Step 1 : Extend the private key with network byte prefix and compress byte ending. */
	if (cmpr_flag == 0)
	{
		extended 	= extended_key;
		extended[0] = ver_byte;
		to_base58 	= ready_to_base58;
	}
	else if (cmpr_flag == 1)
	{
		extended 	 = extended_key_cmpr;
		extended[0]  = ver_byte;
		extended[33] = 0x01;
		to_base58 	 = ready_to_base58_cmpr;
	}
	else return -2;

	for (int32_t i = 1; i <= 32; ++i)
		extended[i] = priv[i-1];

	/* Step 2 : Double sha256 the extended key for checksum. */
	SHA256(extended, (cmpr_flag ? 34 : 33), first_sha256);

	SHA256(first_sha256, 32, second_sha256);

	/* Step 3 : Append the first four byte checksum to the extended key. */
	for (int32_t i = 0; i < (cmpr_flag ? 34 : 33); ++i)
		to_base58[i] = extended[i];

	for (int32_t i = 0; i < 4; ++i)
		to_base58[(cmpr_flag ? 38 : 37) - 4 + i] = second_sha256[i];

	/* Step 4 : Base58 encode the 'to_base' for wallet import format. */
	base58encode(to_base58, cmpr_flag ? 38 : 37, wif);

	return 0;
}

int32_t wif_to_hex(uint8_t *wif, BYTE *private_key)
{
	int32_t decoded_len = base58decode(wif, get_strlen(wif), NULL);
	BYTE to_sha256[decoded_len - 4], checksum_t[4];
	BYTE first_sha256[32], second_sha256[32];

	if (decoded_len != 37 && decoded_len != 38)
		return -1;
	else if (decoded_len == -1)
		return -2;

	BYTE decoded[decoded_len];
	base58decode(wif, get_strlen(wif), decoded);

	for (int32_t i = 0; i < (decoded_len - 4); ++i)
		to_sha256[i] = decoded[i];
	for (int32_t i = 0; i < 4; ++i)
		checksum_t[i] = decoded[decoded_len - 4 + i];

	SHA256(to_sha256, decoded_len - 4, first_sha256);
	SHA256(first_sha256, 32, second_sha256);

	for (int32_t i = 0; i < 4; ++i)
	{
		if (checksum_t[i] != second_sha256[i])
			return -3;
	}

	for (int32_t i = 0; i < 32; ++i)
		private_key[i] = to_sha256[i+1];

	return 0;
}

int32_t pub_to_address(BYTE *public_key, int32_t cmpr_flag, BYTE network_byte, uint8_t *address)
{
	if (network_byte != 0x00 && network_byte != 0x6f && network_byte != 0x34)
		return -1;
	if (cmpr_flag != 0 && cmpr_flag != 1)
		return -2;

	BYTE pub_sha256[32], pub_ripemd160[20];

	BYTE net_byte_added[21], first_checksum_sha[32], second_checksum_sha[32], to_base58[25];

	/* Step 1 : hash160 the public key. */
	SHA256(public_key, cmpr_flag ? 33 : 65, pub_sha256);

	RIPEMD160(pub_sha256, 32, pub_ripemd160);

	/* Step 2 : Extend the hashed public key with network byte prefix. */
	net_byte_added[0] = network_byte;
	for (int32_t i = 0; i < 20; ++i)
		net_byte_added[i + 1] = pub_ripemd160[i];

	/* Step 3 : Double sha256 the extended byte array for checksum. */
	SHA256(net_byte_added, 21, first_checksum_sha);

	SHA256(first_checksum_sha, 32, second_checksum_sha);

	/* Append the checksum to the extended hashed public key */
	for (int32_t i = 0; i < 21; ++i)
		to_base58[i] = net_byte_added[i];
	for (int32_t i = 0; i < 4; ++i)
		to_base58[21 + i] = second_checksum_sha[i];

	/* Step 5 : Base58 encode the 'to_base' payload, get an address. */
	base58encode(to_base58, 25, address);

	return 0;
}

int32_t address_to_hash160(uint8_t *address, BYTE *hash160)
{
	int32_t decoded_len;
	BYTE hash160_t[21], checksum_t[4];
	BYTE first_sha256[32], second_sha256[32];

	/* Get and check the decoded data's length. */
	decoded_len = base58decode(address, get_strlen(address), NULL);
	
	if (decoded_len != 25)
		return -1;
	else if (decoded_len == -1)
		return -2;

	/* b58decode the address. */
	BYTE decoded[decoded_len];
	base58decode(address, get_strlen(address), decoded);
	
	/* Separate the hash160 part and checksum part. */
	for (int32_t i = 0; i < 21; ++i)
		hash160_t[i] = decoded[i];

	for (int32_t i = 0; i < 4; ++i)
		checksum_t[i] = decoded[decoded_len-4+i];

	/* Double sha256 the hash160 and check the checksum. */
	SHA256(hash160_t, 21, first_sha256);
	SHA256(first_sha256, 32, second_sha256);

	for (int32_t i = 0; i < 4; ++i)
	{
		if (checksum_t[i] != second_sha256[i])
			return -3;
	}

	/* Get the hash160 value if checksum is correct. */
	for (int32_t i = 0; i < 20; ++i)
		hash160[i] = hash160_t[i+1];

	return 0;
}

int32_t generate_ecdsa_secp256k1_key_pair(BYTE *priv, BYTE *pub, int32_t cmpr_flag)
{
	int32_t ret;
	int32_t pub_len = 0;
	int32_t NID_secp256k1 = 714;
	int8_t priv_str[65];
	uint8_t pub_internal[130];
	uint8_t *p_pub = pub_internal;

	point_conversion_form_t forms[] = {
		POINT_CONVERSION_UNCOMPRESSED,
		POINT_CONVERSION_COMPRESSED
	};

	BIGNUM *privkey = BN_new();
	EC_KEY *keys = EC_KEY_new_by_curve_name(NID_secp256k1);
	// Bug. const *group can't be freed by free() or OPENSSL_free(), cause memory leak.
	const EC_GROUP *group = EC_KEY_get0_group(keys);			// Struct group store the G and calculation rules
	EC_POINT *pubkey = EC_POINT_new(group);						// Public key is a point on curve
	BN_CTX *ctx = BN_CTX_new();

	if (cmpr_flag != 0 && cmpr_flag != 1)
		return -1;

	/* Generate a private key in string */
	genpriv(priv_str);

	/* Convert private key from string to BIGNUM */
	BN_init(privkey);
	if (BN_hex2bn(&privkey, priv_str) != 0x40)
		return -2;

	/* Generate public key */
	ret = EC_POINT_mul(group, pubkey, privkey, NULL, NULL, ctx);
	EC_KEY_set_private_key(keys, privkey);
	EC_KEY_set_public_key(keys, pubkey);

	/* Convert public key from BIGNUM to array */
	EC_KEY_set_conv_form(keys, forms[cmpr_flag]);
	pub_len = i2o_ECPublicKey(keys, &p_pub);

	BN_clear_free(privkey);
	EC_POINT_free(pubkey);
	BN_CTX_free(ctx);
	EC_KEY_free(keys);

	if (pub_len != 33 && pub_len != 65)
		return -2;

	/* Convert private key from string to byte array */
	hexstr_to_bytearr(priv_str, 64, priv);
	for (int32_t i = 0; i < (cmpr_flag?33:65); ++i)
		pub[i] = pub_internal[i];

	return 0;
}

void genpriv(int8_t *priv_str)
{
//	struct timespec start, end;
//	clock_gettime(CLOCK_MONOTONIC, &start);

	int32_t item, i = 0;
	int8_t bin[256];

	/* Get ns level time stamp for random seed*/
	/* Function 'clock_gettime' might only works on Linux */
	struct timespec nstime;
	clock_gettime(CLOCK_MONOTONIC, &nstime);

	/* Generate binary private key */
	srand(nstime.tv_nsec);
	for (int32_t k = 0; k < 256; ++k)
		bin[k] = rand()%2;

	/* Convert binary private key to hexadecimal */
	do{	item = (bin[i] + 1) * 1000 + (bin[i+1] + 1) * 100 + (bin[i+2] + 1) * 10 + (bin[i+3] + 1);
		priv_str[i/4] = selector(item);
		i += 4;
	}while(i < 256);
	priv_str[64] = '\0';
	
//	clock_gettime(CLOCK_MONOTONIC, &end);
//	printf("%ld ns\n", end.tv_nsec - start.tv_nsec);
}

int8_t selector(int32_t item)
{
	if (item == 1111) return '0';
	else if (item == 1112) return '1';
	else if (item == 1121) return '2';
	else if (item == 1122) return '3';
	else if (item == 1211) return '4';
	else if (item == 1212) return '5';
	else if (item == 1221) return '6';
	else if (item == 1222) return '7';
	else if (item == 2111) return '8';
	else if (item == 2112) return '9';
	else if (item == 2121) return 'A';
	else if (item == 2122) return 'B';
	else if (item == 2211) return 'C';
	else if (item == 2212) return 'D';
	else if (item == 2221) return 'E';
	else if (item == 2222) return 'F';
}
