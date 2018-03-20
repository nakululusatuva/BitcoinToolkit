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
		"  -s            Get a standard mainnet address\n"
		"  -c            Get a compressed mainnet address\n"
		"  -t            Get a standard testnet address\n"
		"  -k            Get a compressed testnet address\n"
		"  -a <address>  Check addess validaion\n"
		"  -g <privkey>  Get an address from private key\n"
		"                Supported format: [B6] [WIF] [Hexadecimal]\n"
		, version, name);
}

int32_t main(int32_t argc, char* const* argv)
{
	int32_t opt;
	const char *version = BITCOIN_TOOLKIT_VERSION;

	if (argc == 1) {
		usage(version, argv[0]);
		exit(0);
	}

	while ((opt = getopt(argc, argv, "sctka:g:")) != -1)
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
			case 'g':
				{
					int32_t cmpr;
					BYTE net, ver, hex[32];
					privkey_anyformat_to_hex((int8_t*)optarg, &cmpr, &ver, &net, hex);
					generate_address_by_private_key(cmpr, ver, net, hex);
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