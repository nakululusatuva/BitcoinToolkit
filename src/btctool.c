#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "std.h"
#include "base.h"
#include "address.h"

void get_standard_mainnet_address();
void get_standard_testnet_address();
void get_compressed_mainnet_address();
void get_compressed_testnet_address();

int32_t main(int32_t argc, char const *argv[])
{
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	get_standard_mainnet_address();

	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("%ld ns\n", end.tv_nsec - start.tv_nsec);

	return 0;
}

void get_standard_mainnet_address()
{
	ADDRESS new;

	new.cmpr_flag = 0;
	new.ver_byte = 0x80;
	new.net_byte = 0x00;

	generate_ecdsa_secp256k1_private_key(new.private_key);
	ecdsa_secp256k1_privkey_to_pubkey(new.private_key, new.public_key, new.cmpr_flag);
	hex_to_wif(new.private_key, new.priv_wif, new.cmpr_flag, new.ver_byte);
	pub_to_address(new.public_key, new.cmpr_flag, new.net_byte, new.address);

	printf("Address:\n");
	printf("%s\n\n", new.address);

	printf("Private Key WIF:\n");
	printf("%s\n\n", new.priv_wif);

	printf("Public Key Hexadecimal:\n");
	for (int32_t i = 0; i < 65; ++i)
		printf("%02X", new.public_key[i]);
	printf("\n\n");

	printf("Private Key Hexadecimal:\n");
	for (int32_t i = 0; i < 32; ++i)
		printf("%02X", new.private_key[i]);
	printf("\n\n");
}

void get_compressed_mainnet_address()
{
	ADDRESS new;

	new.cmpr_flag = 1;
	new.ver_byte = 0x80;
	new.net_byte = 0x00;

	generate_ecdsa_secp256k1_private_key(new.private_key);
	ecdsa_secp256k1_privkey_to_pubkey(new.private_key, new.public_key_cmpr, new.cmpr_flag);
	hex_to_wif(new.private_key, new.priv_wif_cmpr, new.cmpr_flag, new.ver_byte);
	pub_to_address(new.public_key_cmpr, new.cmpr_flag, new.net_byte, new.address);

	printf("Address:\n");
	printf("%s\n\n", new.address);

	printf("Private Key WIF:\n");
	printf("%s\n\n", new.priv_wif_cmpr);

	printf("Public Key Hexadecimal:\n");
	for (int32_t i = 0; i < 33; ++i)
		printf("%02X", new.public_key_cmpr[i]);
	printf("\n\n");

	printf("Private Key Hexadecimal:\n");
	for (int32_t i = 0; i < 32; ++i)
		printf("%02X", new.private_key[i]);
	printf("\n\n");
}

void get_standard_testnet_address()
{
	ADDRESS new;

	new.cmpr_flag = 0;
	new.ver_byte = 0xEF;
	new.net_byte = 0x6F;

	generate_ecdsa_secp256k1_private_key(new.private_key);
	ecdsa_secp256k1_privkey_to_pubkey(new.private_key, new.public_key, new.cmpr_flag);
	hex_to_wif(new.private_key, new.priv_wif, new.cmpr_flag, new.ver_byte);
	pub_to_address(new.public_key, new.cmpr_flag, new.net_byte, new.address);

	printf("Address:\n");
	printf("%s\n\n", new.address);

	printf("Private Key WIF:\n");
	printf("%s\n\n", new.priv_wif);

	printf("Public Key Hexadecimal:\n");
	for (int32_t i = 0; i < 65; ++i)
		printf("%02X", new.public_key[i]);
	printf("\n\n");

	printf("Private Key Hexadecimal:\n");
	for (int32_t i = 0; i < 32; ++i)
		printf("%02X", new.private_key[i]);
	printf("\n\n");
}

void get_compressed_testnet_address()
{
	ADDRESS new;

	new.cmpr_flag = 1;
	new.ver_byte = 0xEF;
	new.net_byte = 0x6F;

	generate_ecdsa_secp256k1_private_key(new.private_key);
	ecdsa_secp256k1_privkey_to_pubkey(new.private_key, new.public_key_cmpr, new.cmpr_flag);
	hex_to_wif(new.private_key, new.priv_wif_cmpr, new.cmpr_flag, new.ver_byte);
	pub_to_address(new.public_key_cmpr, new.cmpr_flag, new.net_byte, new.address);

	printf("Address:\n");
	printf("%s\n\n", new.address);

	printf("Private Key WIF:\n");
	printf("%s\n\n", new.priv_wif_cmpr);

	printf("Public Key Hexadecimal:\n");
	for (int32_t i = 0; i < 33; ++i)
		printf("%02X", new.public_key_cmpr[i]);
	printf("\n\n");

	printf("Private Key Hexadecimal:\n");
	for (int32_t i = 0; i < 32; ++i)
		printf("%02X", new.private_key[i]);
	printf("\n\n");
}