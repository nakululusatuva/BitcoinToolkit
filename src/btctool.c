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
		"Usage: %s [-sctk] [-a <address>] [-g <privkey>] [--base6e -S <string>]\n"
		"       [--base6d -L <string>] [--base58e -F <string>] [--base58d -W <string>]\n"
		"       [--base58checke -B <string>] [--base58checkd -T <string>]\n"
		"Options:\n"
		"  -s                           Get a standard mainnet address.\n"
		"  -c                           Get a compressed mainnet address.\n"
		"  -t                           Get a standard testnet address.\n"
		"  -k                           Get a compressed testnet address.\n"
		"  -a <address>                 Check addess validaion and get the hash160 value.\n"
		"  -g <privkey>                 Check private key validaion and get the address.\n"
		"                               Supported format: [B6] [WIF] [Hexadecimal]\n"
		"  --base6e  -S <string>             Base6  encode\n"
		"  --base6d  -L <string>             Base6  decode\n"
		"  --base58e -F <string>             Base58 encode\n"
		"  --base58d -W <string>             Base58 decode\n"
		"  --base58checke -B <string>        Base58Check encode\n"
		"  --base58checkd -T <string>        Base58Check decode\n"
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
				printf("--------------------------------------------------------------------------------\n");
				generate_address(0, 0x80, 0x00);
				printf("--------------------------------------------------------------------------------\n");
				break;

			case 'c':
				printf("--------------------------------------------------------------------------------\n");
				generate_address(1, 0x80, 0x00);
				printf("--------------------------------------------------------------------------------\n");
				break;

			case 't':
				printf("--------------------------------------------------------------------------------\n");
				generate_address(0, 0xEF, 0x6F);
				printf("--------------------------------------------------------------------------------\n");
				break;

			case 'k':
				printf("--------------------------------------------------------------------------------\n");
				generate_address(1, 0xEF, 0x6F);
				printf("--------------------------------------------------------------------------------\n");
				break;

			case 'a': {	
				int32_t ret;
				BYTE hash160[20];
				ret = address_to_hash160((uint8_t*)optarg, hash160);

				printf("--------------------------------------------------------------------------------\n");
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
				printf("--------------------------------------------------------------------------------\n");
				break;
			}

			case 'g': {
				int32_t cmpr;
				BYTE net, ver, hex[32];
				printf("--------------------------------------------------------------------------------\n");
				privkey_anyformat_to_hex((int8_t*)optarg, &cmpr, &ver, &net, hex);
				if (cmpr == -1)
					break;
				else {
					generate_address_by_private_key(cmpr, ver, net, hex);
					printf("--------------------------------------------------------------------------------\n");
					break;
				}
			}

			case 'S': {
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base6encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1) {
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				base6encode((BYTE*)optarg, payload_len, encoded);

				printf("--------------------------------------------------------------------------------\nEncoded:\n");
				printf("%s\n", encoded);
				printf("--------------------------------------------------------------------------------\n");
				break;
			}

			case 'L': {
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base6decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1) {
					printf("Invalid charater(s) in the base6 string!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base6decode((uint8_t*)optarg, payload_len, decoded);

				printf("--------------------------------------------------------------------------------\nDecoded:\n");
				for (int i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				printf("--------------------------------------------------------------------------------\n");
				break;
			}

			case 'F': {
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base58encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1) {
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				encoded_len = base58encode((BYTE*)optarg, payload_len, encoded);

				printf("--------------------------------------------------------------------------------\nEncoded:\n");
				printf("%s\n", encoded);
				printf("--------------------------------------------------------------------------------\n");
				break;
			}

			case 'W': {
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base58decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1) {
					printf("Invalid charater(s) in the base58 string!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base58decode((uint8_t*)optarg, payload_len, decoded);

				printf("--------------------------------------------------------------------------------\nDecoded:\n");
				for (int32_t i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				printf("--------------------------------------------------------------------------------\n");
				break;
			}

			case 'U': {
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base64encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1) {
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				base64encode((BYTE*)optarg, payload_len, encoded);

				printf("--------------------------------------------------------------------------------\nEncoded:\n");
				for (int32_t i = 0; i < encoded_len; ++i)
					printf("%c", encoded[i]);
				printf("\n");
				printf("--------------------------------------------------------------------------------\n");
				break;
			}

			case 'R': {
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base64decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1) {
					printf("Invalid charater(s) in the base64 string!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base64decode((uint8_t*)optarg, payload_len, decoded);

				printf("--------------------------------------------------------------------------------\nDecoded:\n");
				for (int32_t i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				printf("--------------------------------------------------------------------------------\n");
				break;
			}

			case 'B': {
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base58check_encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1) {
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				base58check_encode((BYTE*)optarg, payload_len, encoded);

				printf("--------------------------------------------------------------------------------\nEncoded:\n");
				for (int32_t i = 0; i < encoded_len; ++i)
					printf("%c", encoded[i]);
				printf("\n");
				printf("--------------------------------------------------------------------------------\n");
				break;
			}

			case 'T': {
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base58check_decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1) {
					printf("Invalid charater(s) in the base58 string!\n");
					break;
				}
				else if (decoded_len == -2) {
					printf("Invalid checksum!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base58check_decode((uint8_t*)optarg, payload_len, decoded);

				printf("--------------------------------------------------------------------------------\nDecoded:\n");
				for (int32_t i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				printf("--------------------------------------------------------------------------------\n");
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