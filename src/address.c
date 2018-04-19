#include <time.h>
#include <string.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include "base.h"
#include "common.h"
#include "strings.h"
#include "address.h"

int8_t selector(int32_t item);

int32_t ecdsa_secp256k1_privkey_to_pubkey(BYTE *priv, BYTE *pub, int32_t cmpr_flag)
{
	int32_t pub_len = 0;
	int32_t NID_secp256k1 = 714;
	int8_t priv_str[64];
	uint8_t pub_internal[130];
	uint8_t *p_pub = pub_internal;

	point_conversion_form_t forms[] = {
		POINT_CONVERSION_UNCOMPRESSED,
		POINT_CONVERSION_COMPRESSED
	};

	BIGNUM *privkey = BN_new();
	EC_KEY *keys = EC_KEY_new_by_curve_name(NID_secp256k1);
	// Bug. const *group can't be freed by free() or OPENSSL_free(), cause memory leak.
	const EC_GROUP *group = EC_KEY_get0_group(keys);            // Struct group store the G and calculation rules
	EC_POINT *pubkey = EC_POINT_new(group);	                    // Public key is a point on curve
	BN_CTX *ctx = BN_CTX_new();

	if (cmpr_flag != 0 && cmpr_flag != 1)
		return -1;

	// Convert private key from byte array to BIGNUM.
	bytearr_to_hexstr(priv, 32, priv_str);
	BN_init(privkey);
	BN_hex2bn(&privkey, (const char*)priv_str);

	// Generate public key.
	EC_POINT_mul(group, pubkey, privkey, NULL, NULL, ctx);
	EC_KEY_set_private_key(keys, privkey);
	EC_KEY_set_public_key(keys, pubkey);

	// Convert public key from BIGNUM to array.
	EC_KEY_set_conv_form(keys, forms[cmpr_flag]);
	pub_len = i2o_ECPublicKey(keys, &p_pub);

	BN_free(privkey);
	EC_POINT_free(pubkey);
	BN_CTX_free(ctx);
	EC_KEY_free(keys);

	if (pub_len != 33 && pub_len != 65)
		return -2;

	for (int32_t i = 0; i < (cmpr_flag?33:65); ++i)
		pub[i] = pub_internal[i];

	return 0;
}

void generate_ecdsa_secp256k1_private_key(BYTE *priv)
{
	int8_t bin[256];
	int8_t priv_str[64];
	int32_t item, i = 0;

	// Get ns level time stamp for random seed.
	// Function 'clock_gettime' might only works on Linux.
	struct timespec nstime;
	clock_gettime(CLOCK_MONOTONIC, &nstime);

	// Generate binary private key.
	srand(nstime.tv_nsec);
	for (int32_t k = 0; k < 256; ++k)
		bin[k] = rand()%2;

	// Convert binary private key to hexadecimal.
	do{	item = (bin[i] + 1) * 1000 + (bin[i+1] + 1) * 100 + (bin[i+2] + 1) * 10 + (bin[i+3] + 1);
		priv_str[i/4] = selector(item);
		i += 4;
	}while(i < 256);

	hexstr_to_bytearr(priv_str, 64, priv);
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
	else return -1;
}

int32_t hex_to_wif(BYTE *private_key, uint8_t *wif, int32_t cmpr_flag, BYTE ver_byte)
{
	BYTE *extended, extended_key[33], extended_key_cmpr[34];

	BYTE first_sha256[32], second_sha256[32];

	BYTE *to_base58, ready_to_base58[37], ready_to_base58_cmpr[38];

	if (ver_byte != 0x80 && ver_byte != 0xef)
		return -1;

	// Step 1 : Extend the private key with network byte prefix and compress byte ending.
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
		extended[i] = private_key[i-1];

	// Step 2 : Double sha256 the extended key for checksum.
	SHA256(extended, (cmpr_flag ? 34 : 33), first_sha256);

	SHA256(first_sha256, 32, second_sha256);

	// Step 3 : Append the first four byte checksum to the extended key.
	for (int32_t i = 0; i < (cmpr_flag ? 34 : 33); ++i)
		to_base58[i] = extended[i];

	for (int32_t i = 0; i < 4; ++i)
		to_base58[(cmpr_flag ? 38 : 37) - 4 + i] = second_sha256[i];

	// Step 4 : Base58 encode the 'to_base' for wallet import format.
	base58encode(to_base58, cmpr_flag ? 38 : 37, wif);

	return 0;
}

int32_t wif_to_hex(uint8_t *wif, BYTE *private_key)
{
	int32_t decoded_len = base58decode(wif, get_strlen((int8_t*)wif), NULL);
	BYTE to_sha256[decoded_len - 4], checksum_t[4];
	BYTE first_sha256[32], second_sha256[32];

	if (decoded_len != 37 && decoded_len != 38)
		return -1;
	else if (decoded_len == -1)
		return -2;

	BYTE decoded[decoded_len];
	base58decode(wif, get_strlen((int8_t*)wif), decoded);

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

int32_t b6_to_hex(uint8_t *b6, size_t b6_len, BYTE *private_key)
{
	int32_t raw_len;
	int32_t prefix_zero_count;

	raw_len = base6decode(b6, b6_len, NULL);
	prefix_zero_count = 32 - raw_len;

	if (private_key == NULL)
		return raw_len;

	BYTE copy[raw_len];
	base6decode(b6, b6_len, copy);

	for (int32_t i = 0; i < prefix_zero_count; ++i)
		private_key[i] = 0x00;
	for (int32_t i = 0; i < raw_len; ++i)
		private_key[i + prefix_zero_count] = copy[i];

	return 0;
}

int32_t pub_to_address(BYTE *public_key, int32_t cmpr_flag, BYTE network_byte, uint8_t *address)
{
	/* April 1st 2018 21:46
	** delete start
		if (network_byte != 0x00 && network_byte != 0x6f && network_byte != 0x34)
	** delete end
	*/
	if (network_byte != 0x00 && network_byte != 0x6f)
		return -1;
	if (cmpr_flag != 0 && cmpr_flag != 1)
		return -2;

	BYTE pub_sha256[32], pub_ripemd160[20];

	BYTE net_byte_added[21], first_checksum_sha[32], second_checksum_sha[32], to_base58[25];

	// Step 1 : hash160 the public key.
	SHA256(public_key, cmpr_flag ? 33 : 65, pub_sha256);

	RIPEMD160(pub_sha256, 32, pub_ripemd160);

	// Step 2 : Extend the hashed public key with network byte prefix.
	net_byte_added[0] = network_byte;
	for (int32_t i = 0; i < 20; ++i)
		net_byte_added[i + 1] = pub_ripemd160[i];

	// Step 3 : Double sha256 the extended byte array for checksum.
	SHA256(net_byte_added, 21, first_checksum_sha);

	SHA256(first_checksum_sha, 32, second_checksum_sha);

	// Step 4 : Append the checksum to the extended hashed public key.
	for (int32_t i = 0; i < 21; ++i)
		to_base58[i] = net_byte_added[i];
	for (int32_t i = 0; i < 4; ++i)
		to_base58[21 + i] = second_checksum_sha[i];

	// Step 5 : Base58 encode the 'to_base' payload, get an address.
	base58encode(to_base58, 25, address);

	return 0;
}

int32_t address_to_hash160(uint8_t *address, BYTE *hash160)
{
	int32_t decoded_len;
	BYTE hash160_t[21], checksum_t[4];
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
		hash160_t[i] = decoded[i];

	for (int32_t i = 0; i < 4; ++i)
		checksum_t[i] = decoded[decoded_len-4+i];

	// Double sha256 the hash160 and check the checksum.
	SHA256(hash160_t, 21, first_sha256);
	SHA256(first_sha256, 32, second_sha256);

	for (int32_t i = 0; i < 4; ++i)
	{
		if (checksum_t[i] != second_sha256[i])
			return -3;
	}

	// Get the hash160 value if checksum is correct.
	for (int32_t i = 0; i < 20; ++i)
		hash160[i] = hash160_t[i+1];

	return 0;
}

int32_t privkey_validation(int8_t *privkey, size_t length)
{
	const uint8_t base6table[6] =
		{'0', '1', '2', '3', '4', '5'};

	const uint8_t hextable[16] =
		{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	const uint8_t base58table[58] =
		{'1', '2', '3', '4', '5', '6', '7', '8', '9','A', 'B', 'C', 'D', 'E', 'F',
		'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
		'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm',
		'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

	int8_t copy[length];
	for (int32_t i = 0; i < length; ++i)
		copy[i] = privkey[i];

	// Check if WIF.
	if (length == 51 || length == 52)
	{
		for (int32_t i = 0; i < length; ++i)
		{
			int32_t j = 0;
			for ( ; j < 58; ++j)
			{
				if (copy[i] == base58table[j])
					break;
			}
			if (j == 58)
				return -2;
		}
		//Check WIF type.
		if (copy[0] == '5')
		{
			if (strcmp((const char*)privkey, (const char*)MAX_PRIVKEY_WIF_SM)>0)
				return -5;
			else
				return 1;
		}
		else if (copy[0] == 'K' || copy[0] == 'L') {
			BYTE decoded[32];
			int8_t decoded_str[65];
			wif_to_hex((uint8_t*)privkey, decoded);
			bytearr_to_hexstr(decoded, 32, decoded_str);
			if (strcmp((const char*)decoded_str, (const char*)MAX_PRIVKEY_HEX)>0)
				return -5;
			else
				return 2;
		}
		else if (copy[0] == '9') {
			if (strcmp((const char*)privkey, (const char*)MAX_PRIVKEY_WIF_ST)>0)
				return -5;
			else
				return 3;
		}
		else if (copy[0] == 'c') {
			if (strcmp((const char*)privkey, (const char*)MAX_PRIVKEY_WIF_CT)>0)
				return -5;
			else
				return 4;
		}
	}

	// Check if hexadecimal.
	else if (length == 64)
	{
		for (int32_t i = 0; i < length; ++i)
		{
			int32_t j = 0;
			for ( ; j < 16; ++j)
			{
				if (copy[i] == hextable[j])
					break;
			}
			if (j == 16)
				return -3;
		}
		if (strcmp((const char*)privkey, (const char*)MAX_PRIVKEY_HEX)>0)
			return -5;
		else
			return 5;
	}

	// Check if base6.
	else if (length == 99 || length == 100)
	{
		for (int32_t i = 0; i < length; ++i)
		{
			int32_t j = 0;
			for ( ; j < 6; ++j)
			{
				if (copy[i] == base6table[j])
					break;
			}
			if (j == 6)
				return -4;
		}
		if (strcmp((const char*)privkey, (const char*)MAX_PRIVKEY_B6)>0)
			return -5;
		else
			return 6;
	}
	
	return -1;
}

int32_t privkey_anyformat_to_hex(int8_t *key, int32_t *cmpr, BYTE *ver, BYTE *net, BYTE *hex)
{
	int32_t ret;
	ret = privkey_validation(key, get_strlen(key));

	// WIF private key?
	if (ret == 1) {
		wif_to_hex((uint8_t*)key, hex);
		*cmpr = 0;
		*ver = 0x80;
		*net = 0x00;
		return 0;
	}
	else if (ret == 2) {
		wif_to_hex((uint8_t*)key, hex);
		*cmpr = 1;
		*ver = 0x80;
		*net = 0x00;
		return 0;
	}
	else if (ret == 3) {
		wif_to_hex((uint8_t*)key, hex);
		*cmpr = 0;
		*ver = 0xEF;
		*net = 0x6F;
		return 0;
	}
	else if (ret == 4) {
		wif_to_hex((uint8_t*)key, hex);
		*cmpr = 1;
		*ver = 0xEF;
		*net = 0x6F;
		return 0;
	}

	/**** DELETE START Apr 7 23:33 2018 ****
	// Hex or B6 private key?
	else if (ret == 5 || ret == 6)
	{
		uint8_t byte;
		uint8_t flag;

		printf("You want a mainnet or testnet address? (m/t):");
		while(1) {
			while((byte = getchar()) != '\n' && byte != EOF);
			byte = getchar();
			if (byte == 0x6D) {// 'm'
				*ver = 0x80;
				*net = 0x00;
				break;
			}
			else if (byte == 0x74) {// 't'
				*ver = 0xEF;
				*net = 0x6F;
				break;
			}
			else {
				printf("Invalid Option!");
				continue;
			}
		}

		printf("You want a compressed address? (y/n):");
		while(1) {
			flag = getchar();
			if (flag == 0x6E) {// 'n'
				*cmpr = 0;
				break;
			}
			else if (flag == 0x79) {// 'y'
				*cmpr = 1;
				break;
			}
			else {
				printf("Invalid Option!");
				continue;
			}
		}

		if (ret == 5)
			hexstr_to_bytearr(key, get_strlen(key), hex);
		else if (ret == 6)
			b6_to_hex((uint8_t*)key, get_strlen(key), hex);
	}
	**** DELETE END ****/

	/**** ADD START Apr 7 23:33 2018 ****/
	// Hex or B6 private key?
	else if (ret == 5) {
		hexstr_to_bytearr(key, get_strlen(key), hex);
		return 1;
	}
	else if (ret == 6) {
		b6_to_hex((uint8_t*)key, get_strlen(key), hex);
		return 1;
	}
	/**** DELETE END ****/

	else if (ret == -1)
		return -1;
	else if (ret == -2)
		return -2;
	else if (ret == -3)
		return -3;
	else if (ret == -4)
		return -4;
	else if (ret == -5)
		return -5;

	return 0;
}

void ADDRESS_init(ADDRESS *addr)
{
	addr->cmpr_flag = 127;
	addr->ver_byte = 127;
	addr->net_byte = 127;
}

ADDRESS generate_address(int32_t cmpr, BYTE ver, BYTE net)
{
	ADDRESS new;
	ADDRESS_init(&new);
	if (cmpr != 0 && cmpr != 1)
		new.cmpr_flag = -1;
	else if (ver != 0x80 && ver != 0xEF)
		new.cmpr_flag = -2;
	else if (net != 0x00 && net != 0x6F)
		new.cmpr_flag = -3;
	else{
		new.cmpr_flag = cmpr;
		new.ver_byte = ver;
		new.net_byte = net;

		generate_ecdsa_secp256k1_private_key(new.private_key);
		ecdsa_secp256k1_privkey_to_pubkey(new.private_key, new.cmpr_flag?new.public_key_cmpr:new.public_key, new.cmpr_flag);
		hex_to_wif(new.private_key, new.cmpr_flag?new.priv_wif_cmpr:new.priv_wif, new.cmpr_flag, new.ver_byte);
		pub_to_address(new.cmpr_flag?new.public_key_cmpr:new.public_key, new.cmpr_flag, new.net_byte, new.address);

		return new;
	}
	return new;
}

ADDRESS generate_address_by_private_key(int32_t cmpr, BYTE ver, BYTE net, BYTE *hex)
{
	ADDRESS new;
	ADDRESS_init(&new);
	if (cmpr != 0 && cmpr != 1)
		new.cmpr_flag = -1;
	else if (ver != 0x80 && ver != 0xEF)
		new.cmpr_flag = -2;
	else if (net != 0x00 && net != 0x6F)
		new.cmpr_flag = -3;
	else{
		new.cmpr_flag = cmpr;
		new.ver_byte = ver;
		new.net_byte = net;
		for (int32_t i = 0; i < 32; ++i)
			new.private_key[i] = hex[i];

		ecdsa_secp256k1_privkey_to_pubkey(new.private_key, new.cmpr_flag?new.public_key_cmpr:new.public_key, new.cmpr_flag);
		hex_to_wif(new.private_key, new.cmpr_flag?new.priv_wif_cmpr:new.priv_wif, new.cmpr_flag, new.ver_byte);
		pub_to_address(new.cmpr_flag?new.public_key_cmpr:new.public_key, new.cmpr_flag, new.net_byte, new.address);

		return new;
	}
	return new;
}

void print_address(ADDRESS addr)
{
	printf("Address:\n");
	printf("%s\n\n", addr.address);

	printf("Private Key WIF:\n");
	printf("%s\n\n", addr.cmpr_flag?addr.priv_wif_cmpr:addr.priv_wif);

	printf("Public Key Hexadecimal:\n");
	if (addr.cmpr_flag == 0)
	{
		for (int32_t i = 0; i < 65; ++i)
		printf("%02X", addr.public_key[i]);
	}
	else if (addr.cmpr_flag == 1)
	{
		for (int32_t i = 0; i < 33; ++i)
		printf("%02X", addr.public_key_cmpr[i]);
	}
	printf("\n\n");

	printf("Private Key Hexadecimal:\n");
	for (int32_t i = 0; i < 32; ++i)
		printf("%02X", addr.private_key[i]);
	printf("\n");
}