/******************************** MIT License **********************************
Copyright (c) 2018 Yirain Suen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "codec/codec.h"
#include "common.h"
#include "address.h"
#include "script/script.h"

void usage(const char *version, const char *name)
{
	fprintf(stderr,
		"Bticoin-ToolKit %s\n"
		"Usage: %s <option> <argument>\n"
		" -s <numbers>                  Get mainnet standard P2PKH address.\n"
//		" -p <numbers>                  Get mainnet standard P2SH address.\n"
		" -c <numbers>                  Get mainnet compress P2PKH address.\n"
//		" -w <numbers>                  Get mainnet compress P2SH address.\n"
		" -t <numbers>                  Get testnet standard P2PKH address.\n"
//		" -h <numbers>                  Get testnet standard P2SH address.\n"
		" -k <numbers>                  Get testnet compress P2PKH address.\n"
//		" -f <numbers>                  Get testnet compress P2SH address.\n"
		" -a <address>                  Check addess validaion and get the hash160 value.\n"
		" -g <privkey>                  Check private key validaion and get the address.\n"
		"                               Supported format: [B6] [WIF] [Hexadecimal]\n"
		" --base6e  -S <string>         Base6  encode\n"
		" --base6d  -L <string>         Base6  decode\n"
		" --base58e -F <string>         Base58 encode\n"
		" --base58d -W <string>         Base58 decode\n"
		" --base64e -U <string>         Base64 encode\n"
		" --base64d -R <string>         Base64 decode\n"
		" --base58checke -B <string>    Base58Check encode\n"
		" --base58checkd -T <string>    Base58Check decode\n"
		, version, name);
}

int32_t main(int32_t argc, char* const* argv)
{
	char* const short_options = "s:c:t:k:a:g:S:L:F:W:B:T:U:R:";
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
			{	// Mainnet standard P2PKH address
				int64_t number = atoll((const char *)optarg);
				if (number <= 0)
				{
					printf("Invalid Number!\n");
					break;
				}

				ADDRESS addr;
				for (int64_t i = 0; i < number; ++i)
				{
					addr = generate_address(0, PRIVATE_KEY_MAINNET_BYTE_PREFIX, ADDRESS_MAINNET_PUBKEY_HASH_BYTE_PREFIX);
					print_address(addr);
				}
				
				break;
			}

			case 'c':
			{	// Mainnet compress P2PKH address
				int64_t number = atoll((const char *)optarg);
				if (number <= 0)
				{
					printf("Invalid Number!\n");
					break;
				}

				ADDRESS addr;
				for (int64_t i = 0; i < number; ++i)
				{
					addr = generate_address(1, PRIVATE_KEY_MAINNET_BYTE_PREFIX, ADDRESS_MAINNET_PUBKEY_HASH_BYTE_PREFIX);
					print_address(addr);
				}

				break;
			}

			case 't':
			{	// Testnet standard P2PKH address
				int64_t number = atoll((const char *)optarg);
				if (number <= 0)
				{
					printf("Invalid Number!\n");
					break;
				}

				ADDRESS addr;
				for (int64_t i = 0; i < number; ++i)
				{
					addr = generate_address(0, PRIVATE_KEY_TESTNET_BYTE_PREFIX, ADDRESS_TESTNET_PUBKEY_HASH_BYTE_PREFIX);
					print_address(addr);
				}
				
				break;
			}

			case 'k':
			{	// Testnet compress P2PKH address
				int64_t number = atoll((const char *)optarg);
				if (number <= 0)
				{
					printf("Invalid Number!\n");
					break;
				}

				ADDRESS addr;
				for (int64_t i = 0; i < number; ++i)
				{
					addr = generate_address(1, PRIVATE_KEY_TESTNET_BYTE_PREFIX, ADDRESS_TESTNET_PUBKEY_HASH_BYTE_PREFIX);
					print_address(addr);
				}

				break;
			}

			case 'a':
			{	
				BYTE hash160[20];
				switch (address_to_hash160((uint8_t*)optarg, hash160))
				{
					case 0:
					{
						printf("Correct Address!\n");
						printf("hash160 Value: ");
						for (int32_t i = 0; i < 20; ++i)
							printf("%02X", hash160[i]);
						printf("\n");
						break;
					}

					case -1:
					{
						printf("Invalid Charaters In The Address!\n");
						break;
					}

					case -2:
					{
						printf("Invalid Address Length!\n");
						break;
					}

					case -3:
					{
						printf("Invalid Checksum!\n");
						break;
					}
				}
				break;
			}

			case 'g':
			{
				int32_t optarg_len;
				int32_t cmpr_flag = 0;
				BYTE privkey_type = PRIVATE_KEY_MAINNET_BYTE_PREFIX,
					 address_type = ADDRESS_MAINNET_PUBKEY_HASH_BYTE_PREFIX,
					 privkey_raw[32];
				optarg_len = get_strlen((int8_t*)optarg);

				switch (privkey_validation((int8_t*)optarg, optarg_len))
				{
					case ECKEY_VALUE_OUT_RANGE:
					{
						printf("EC key value out range!\n");
						break;
					}

					case UNSUPPORTED_FORMAT:
					{
						printf("Unsupported format!\n");
						break;
					}

					case INVALID_BASE58_STRING:
					{
						printf("Invalid Base58 key string!\n");
						break;
					}

					case INVALID_WIF_CHECKSUM:
					{
						printf("Invalid checksum!\n");
						break;
					}

					case INVALID_HEX_STRING:
					{
						printf("Invalid HEX key string!\n");
						break;
					}

					case INVALID_BASE6_STRING:
					{
						printf("Invalid B6 key string!\n");
						break;
					}

					case INVALID_BASE64_STRING:
					{
						printf("Invalid B64 key string!\n");
						break;
					}

					default:
					{
						anyformat_to_raw((int8_t*)optarg, &cmpr_flag, &privkey_type, &address_type, privkey_raw);
						ADDRESS addr;
						addr = generate_address_by_private_key(cmpr_flag, privkey_type, address_type, privkey_raw);
						print_address(addr);
						break;
					}
				}
				break;
			}

			case 'S':
			{
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base6encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1)
				{
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				base6encode((BYTE*)optarg, payload_len, encoded);

				printf("%s\n", encoded);

				break;
			}

			case 'L':
			{
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base6decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1)
				{
					printf("Invalid charater(s) in the base6 string!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base6decode((uint8_t*)optarg, payload_len, decoded);

				printf("Decoded:\n");
				for (int i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				
				break;
			}

			case 'F':
			{
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base58encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1)
				{
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				encoded_len = base58encode((BYTE*)optarg, payload_len, encoded);

				printf("Encoded:\n");
				printf("%s\n", encoded);
				
				break;
			}

			case 'W':
			{
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base58decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1)
				{
					printf("Invalid charater(s) in the base58 string!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base58decode((uint8_t*)optarg, payload_len, decoded);

				printf("Decoded:\n");
				for (int32_t i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				
				break;
			}

			case 'U':
			{
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base64encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1)
				{
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				base64encode((BYTE*)optarg, payload_len, encoded);

				printf("Encoded:\n");
				for (int32_t i = 0; i < encoded_len; ++i)
					printf("%c", encoded[i]);
				printf("\n");
				
				break;
			}

			case 'R':
			{
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base64decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1)
				{
					printf("Invalid charater(s) in the base64 string!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base64decode((uint8_t*)optarg, payload_len, decoded);

				printf("Decoded:\n");
				for (int32_t i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				
				break;
			}

			case 'B':
			{
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t encoded_len = base58check_encode((BYTE*)optarg, payload_len, NULL);
				if (encoded_len == -1)
				{
					printf("Charater's value out range!\n");
					break;
				}

				uint8_t encoded[encoded_len];
				base58check_encode((BYTE*)optarg, payload_len, encoded);

				printf("Encoded:\n");
				for (int32_t i = 0; i < encoded_len; ++i)
					printf("%c", encoded[i]);
				printf("\n");
				
				break;
			}

			case 'T':
			{
				size_t payload_len = get_strlen((int8_t*)optarg);

				int32_t decoded_len = base58check_decode((uint8_t*)optarg, payload_len, NULL);
				if (decoded_len == -1)
				{
					printf("Invalid charater(s) in the base58 string!\n");
					break;
				}
				else if (decoded_len == -2)
				{
					printf("Invalid checksum!\n");
					break;
				}

				BYTE decoded[decoded_len];
				base58check_decode((uint8_t*)optarg, payload_len, decoded);

				printf("Decoded:\n");
				for (int32_t i = 0; i < decoded_len; ++i)
					printf("%c", decoded[i]);
				printf("\n");
				
				break;
			}

			default:
			{
				usage(version, argv[0]);
				exit(0);
			}
		}
	}

	return 0;
}