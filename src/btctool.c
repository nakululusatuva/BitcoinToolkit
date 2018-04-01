#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "base.h"
#include "strings.h"
#include "common.h"
#include "address.h"

void usage(const char *version, const char *name)
{
	fprintf(stderr,
		"Bticoin-ToolKit %s\n"
		"Usage: %s [-sctk] [-a <address>] [-g <privkey>]\n"
		"Options:\n"
		"  -s                            Get a standard mainnet address\n"
		"  -c                            Get a compressed mainnet address\n"
		"  -t                            Get a standard testnet address\n"
		"  -k                            Get a compressed testnet address\n"
		"  -a <address>                  Check addess validaion\n"
		"  -g <privkey>                  Get an address from private key\n"
		"                                Supported format: [B6] [WIF] [Hexadecimal]\n"
		"  --base6e  -S <string>             Base6  encode\n"
		"  --base6d  -L <string>             Base6  decode\n"
		"  --base58e -F <string>             Base58 encode\n"
		"  --base58d -W <string>             Base58 decode\n"
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
				if (cmpr == -1)
					break;
				else {
					generate_address_by_private_key(cmpr, ver, net, hex);
					break;
				}
			}

			case 'S': {
				size_t payload_len;
				payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base6encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1) {
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				base6encode((BYTE*)optarg, payload_len, encoded);

				printf("%s\n", encoded);
				break;
			}

			case 'L': {
				size_t payload_len;
				payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base6decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1) {
					printf("Invalid charater(s) in the base6 string!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base6decode((uint8_t*)optarg, payload_len, decoded);

				for (int i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				break;
			}

			case 'F': {
				size_t payload_len;
				payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base58encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1) {
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				encoded_len = base58encode((BYTE*)optarg, payload_len, encoded);

				printf("%s\n", encoded);
				break;
			}

			case 'W': {
				size_t payload_len;
				payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base58decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1) {
					printf("Invalid charater(s) in the base58 string!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base58decode((uint8_t*)optarg, payload_len, decoded);

				for (int32_t i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				break;
			}

			case 'U': {
				size_t payload_len;
				payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base64encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1) {
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				base64encode((BYTE*)optarg, payload_len, encoded);

				for (int32_t i = 0; i < encoded_len; ++i)
					printf("%c", encoded[i]);
				printf("\n");
				break;
			}

			default: {
				usage(version, argv[0]);
				exit(0);
			}
		}
	}

	return 0;
}