#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "std.h"
#include "base.h"
#include "address.h"

int32_t get_new_address(int32_t compress, BYTE version)
{
	ADDRESS new;

	if (version == 0x80)
		new.net_byte = 0x00;
	else if (version == 0xEF)
		new.net_byte = 0x6F;
	else return -1;

	new.cmpr_flag = compress;
	new.ver_byte = version;

	if (new.cmpr_flag == 0)
	{
		generate_ecdsa_secp256k1_key_pair(new.private_key, new.public_key, new.cmpr_flag);
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

	else if (new.cmpr_flag == 1)
	{
		generate_ecdsa_secp256k1_key_pair(new.private_key, new.public_key_cmpr, new.cmpr_flag);
		hex_to_wif(new.private_key, new.priv_wif_cmpr, new.cmpr_flag, new.ver_byte);
		pub_to_address(new.public_key_cmpr, new.cmpr_flag, new.net_byte, new.address);

		printf("Address:\n");
		printf("%s\n\n", new.address);

		printf("Private Key WIF Compressed:\n");
		printf("%s\n\n", new.priv_wif_cmpr);

		printf("Public Key Hexadecimal Compressed:\n");
		for (int32_t i = 0; i < 33; ++i)
			printf("%02X", new.public_key_cmpr[i]);
		printf("\n\n");

		printf("Private Key Hexadecimal:\n");
		for (int32_t i = 0; i < 32; ++i)
			printf("%02X", new.private_key[i]);
		printf("\n\n");
	}
	else return -2;

	return 0;
}

int32_t main(int32_t argc, char const *argv[])
{
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	get_new_address(0, 0x80);

	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("%ld ns\n", end.tv_nsec - start.tv_nsec);

	return 0;
}
