#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "base.h"
#include "string.h"
#include "common.h"
#include "address.h"

void usage(const char *name);
void get_standard_mainnet_address();
void get_standard_testnet_address();
void get_compressed_mainnet_address();
void get_compressed_testnet_address();

int32_t main(int32_t argc, char * const* argv)
{
	int32_t opt;

	if (argc == 1)
		usage(argv[0]);

	while ((opt = getopt(argc, argv, "sctka:p:")) != -1)
	{
		switch(opt)
		{
			case 's':
				get_standard_mainnet_address();
				break;
			case 'c':
				get_compressed_mainnet_address();
				break;
			case 't':
				get_standard_testnet_address();
				break;
			case 'k':
				get_compressed_testnet_address();
				break;
			case 'a':
				{
					int32_t ret;
					BYTE hash160[20];
					ret = address_to_hash160((uint8_t*)optarg, hash160);
					if (ret == 0)
					{
						printf("Correct Address!\n");
						printf("hash160 Value: ");
						for (int32_t i = 0; i < 20; ++i)
							printf("%02X", hash160[i]);
						printf("\n");
					}
					else if (ret == -1)
						printf("Invalid Charaters In The Address!\n");
					else if (ret == -2)
						printf("Invalid Address Length!\n");
					else if (ret == -3)
						printf("Invalid Checksum!\n");
				}
			default:
				usage(argv[0]);
		}
	}

	return 0;
}

void usage(const char *name)
{
	fprintf(stderr,
"Bticoin-ToolKit v0.0.1\n"
"Usage: %s [-sctka] [-a <address>] [-p <privkey>]\n"
"Options:\n"
"-s            Get a standard mainnet address\n"
"-c            Get a compressed mainnet address\n"
"-t            Get a standard testnet address\n"
"-k            Get a compressed testnet address\n"
"-a <address>  Check addess validaion\n"
, name);
}

int32_t check_privkey_format(int8_t *privkey)
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

	int32_t len = get_strlen(privkey);

	// Check if WIF.
	if (len > 50 && len < 53)
	{
		for (int32_t i = 0; i < len; ++i)
		{
			int32_t j = 0;
			for ( ; j < 58; ++j)
			{
				if (privkey[i] == base58table[j])
				{
					privkey[i] = j;
					break;
				}
			}
			if (j == 58)
				return -1;
		}

		//Check WIF type.
		if (privkey[0] == '5')
			return 1;
		else if (privkey[0] == 'K' || privkey[0] == 'L')
			return 2;
		else if (privkey[0] == '9')
			return 3;
		else if (privkey[0] == 'c')
			return 4;
	}

	// Check if hexadecimal.
	if (len == 64)
	{
		for (int32_t i = 0; i < len; ++i)
		{
			int32_t j = 0;
			for ( ; j < 16; ++j)
			{
				if (privkey[i] == hextable[j])
				{
					privkey[i] = j;
					break;
				}
			}
			if (j == 16)
				return -2;
		}
		return 5;
	}

	// Check if base6.
	if (len == 99)
	{
		for (int32_t i = 0; i < len; ++i)
		{
			int32_t j = 0;
			for ( ; j < 6; ++j)
			{
				if (privkey[i] == base6table[j])
				{
					privkey[i] = j;
					break;
				}
			}
			if (j == 6)
				return -3;
		}
		return 6;
	}

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