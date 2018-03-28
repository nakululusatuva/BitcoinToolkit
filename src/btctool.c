#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "base.h"
#include "string.h"
#include "common.h"
#include "address.h"

void usage(const char *version, const char *name)
{
	fprintf(stderr,
		"Bticoin-ToolKit %s\n"
		"Usage: %s [-sctk] [-a <address>] [-g <privkey>]\n"
		"Options:\n"
		"  -s                        Get a standard mainnet address\n"
		"  -c                        Get a compressed mainnet address\n"
		"  -t                        Get a standard testnet address\n"
		"  -k                        Get a compressed testnet address\n"
		"  -a <address>              Check addess validaion\n"
		"  -g <privkey>              Get an address from private key\n"
		"                            Supported format: [B6] [WIF] [Hexadecimal]\n"
		"  --base6e -S <string>      Base6 encode\n"
		, version, name);
}

int32_t main(int32_t argc, char* const* argv)
{
	char* const short_options = "sctka:g:S:L:F:W:B:T:U:R:";
	struct option long_options[] = {
		{"base6e", 1, NULL, 'S'},
		{"base6d", 1, NULL, 'L'},
		{"base58e", 1, NULL, 'F'},
		{"base58d", 1, NULL, 'W'},
		{"base58checke", 1, NULL, 'B'},
		{"base58checkd", 1, NULL, 'T'},
		{"base64e", 1, NULL, 'U'},
		{"base64d", 1, NULL, 'R'},
		{0, 0, 0, 0}
	};

	int32_t opt;
	const char *version = BITCOIN_TOOLKIT_VERSION;

	if (argc == 1) {
		usage(version, argv[0]);
		exit(0);
	}
	while ((opt = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
	{
		switch(opt)
		{
			case 's':
				generate_address(0, 0x80, 0x00);
				break;

			case 'c':
				generate_address(1, 0x80, 0x00);
				break;

			case 't':
				generate_address(0, 0xEF, 0x6F);
				break;

			case 'k':
				generate_address(1, 0xEF, 0x6F);
				break;

			case 'a': {	
				int32_t ret;
				BYTE hash160[20];
				ret = address_to_hash160((uint8_t*)optarg, hash160);

				if (ret == 0) {
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

			case 'g': {
				int32_t cmpr;
				BYTE net, ver, hex[32];
				privkey_anyformat_to_hex((int8_t*)optarg, &cmpr, &ver, &net, hex);
				generate_address_by_private_key(cmpr, ver, net, hex);
				break;
			}

			case 'S': {
				int32_t encoded_len;
				size_t copy_len;
				size_t optarg_len = get_strlen((int8_t*)optarg);

				if (optarg_len % 2 == 1)
					copy_len = optarg_len + 1;
				else
					copy_len = optarg_len;
				int8_t copy[copy_len];

				if (copy_len != optarg_len) {
					copy[0] = '0';
					for (int32_t i = 1; i < copy_len; ++i)
						copy[i] = optarg[i-1];
				}
				else {
					for (int32_t i = 0; i < copy_len; ++i)
						copy[i] = optarg[i];
				}

				BYTE byte_copy[copy_len/2];
				hexstr_to_bytearr(copy, copy_len, byte_copy);

				encoded_len = base6encode(byte_copy, copy_len/2, NULL);
				uint8_t encoded[encoded_len];
				base6encode(byte_copy, copy_len/2, encoded);

				printf("%s\n", encoded);
			}

			default: {
				usage(version, argv[0]);
				exit(0);
			}
		}
	}

	return 0;
}