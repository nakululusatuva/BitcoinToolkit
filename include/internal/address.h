#ifndef _BTC_ADDRESS_
#define _BTC_ADDRESS_

#include "common.h"

// Type defines.
typedef enum prefix {
	PREFIX_PRIV_MAINNET = 0x80,
	PREFIX_PRIV_TESTNET = 0xef,
	PREFIX_ADDR_MAINNET_P2PKH = 0x00,
	PREFIX_ADDR_MAINNET_P2SH  = 0x05,
	PREFIX_ADDR_TESTNET_P2PKH = 0x6f,
	PREFIX_ADDR_TESTNET_P2SH  = 0xc4,
} PREFIX;
typedef enum suffix {
	SUFFIX_PRIV_COMPRESS = 0x01,
} SUFFIX;

typedef enum address_type {
	MAINNET_P2PKH = 0x01,
	MAINNET_P2SH = 0x02,
	TESTNET_P2PKH = 0x03,
	TESTNET_P2SH = 0x04,
} ADDRESS_TYPE;
typedef enum network_type {
	MAINNET = 0x01,
	TESTNET = 0x02,
} NETWORK_TYPE;
typedef enum privkey_format {
	RAW = 0x01,
	HEX = 0x02,
	WIF = 0x03,
	BASE6 = 0x04,
	BASE64 = 0x05,
} PRIVKEY_FORMAT;

// Basic functions.
void ecdsa_secp256k1_generate_private_key(BYTE *priv_raw);
Status ecdsa_secp256k1_privkey_to_pubkey(BYTE *priv_raw, BYTE *pub_raw, bool compress);
void raw_to_wif(BYTE *priv_raw, uint8_t *priv_wif, bool compress, NETWORK_TYPE type);
Status wif_to_raw(uint8_t *priv_wif, BYTE *priv_raw);
Status b6_to_hex(uint8_t *b6, size_t b6_len, BYTE *priv_raw);
void pub_to_address(BYTE *pub_raw, uint8_t *address, bool compress, ADDRESS_TYPE addr_type);
Status address_to_hash160(uint8_t *address, BYTE *hash160);
Status privkey_validation(uint8_t *key, size_t len, PRIVKEY_FORMAT format);
uint8_t selector(uint16_t item);

/******************** Father ********************/
typedef struct root_address_st root_Address;
struct root_address_st {
	BYTE priv_raw[32];
	BYTE pub_raw[65];
	BYTE pubc_raw[33];
	BYTE * (*get_priv_raw)(root_Address *);
	BYTE * (*get_pub_raw)(root_Address *);
};

/******************** P2PKH Mainnet Address ********************/
typedef struct p2pkh_main_address_st P2PKH_Main_Address;
struct p2pkh_main_address_st {
	root_Address root;
	uint8_t b58_address[35]; // 34(33) charaters long plus '\0'
	BYTE * (*get_priv_raw)(P2PKH_Main_Address *);
	BYTE * (*get_pub_raw)(P2PKH_Main_Address *);
	BYTE * (*get_hash160)(P2PKH_Main_Address *);
	uint8_t * (*get_wif)(P2PKH_Main_Address *);
	bool compress;
};
// Allocate on heap memory.
P2PKH_Main_Address * new_P2PKH_Main(bool compress);
P2PKH_Main_Address * new_P2PKH_Main_from_key(const uint8_t *anyformat, bool compress);
Status delete_P2PKH_Main(P2PKH_Main_Address *self);
// Allocate on stack memory.
Status Initialize_P2PKH_Main(P2PKH_Main_Address *addr, bool compress);
Status Abandon_P2PKH_Main(P2PKH_Main_Address *addr);

/******************** P2PKH Testnet Address ********************/
typedef struct p2pkh_test_address_st P2PKH_Test_Address;
struct p2pkh_test_address_st {
	root_Address root;
	uint8_t b58_address[35]; // 34(33) charaters long plus '\0'
	BYTE * (*get_priv_raw)(P2PKH_Test_Address *);
	BYTE * (*get_pub_raw)(P2PKH_Test_Address *);
	BYTE * (*get_hash160)(P2PKH_Test_Address *);
	uint8_t * (*get_wif)(P2PKH_Test_Address *);
	bool compress;
};
// Allocate on heap memory.
P2PKH_Test_Address * new_P2PKH_Test(bool compress);
P2PKH_Test_Address * new_P2PKH_Test_from_key(const uint8_t *anyformat, bool compress);
Status delete_P2PKH_Test(P2PKH_Test_Address *self);
// ALlocate on stack memory.
Status Initialize_P2PKH_Test(P2PKH_Test_Address *addr, bool compress);
Status Abandon_P2PKH_Test(P2PKH_Test_Address *addr);

/******************** P2SH Mainnet Address ********************/
typedef struct p2sh_main_address_st P2SH_Main_Address;
struct p2sh_main_address_st {
	root_Address root;
	uint8_t b58_address[35]; // 34(33) charaters long plus '\0'
	BYTE * (*get_priv_raw)(P2SH_Main_Address *);
	BYTE * (*get_pub_raw)(P2SH_Main_Address *);
	BYTE * (*get_hash160)(P2SH_Main_Address *);
	uint8_t * (*get_wif)(P2SH_Main_Address *);
	bool compress;
};
// Allocate on heap memory.
P2SH_Main_Address * new_P2SH_Main(bool compress);
P2SH_Main_Address * new_P2SH_Main_from_key(const uint8_t *anyformat, bool compress);
Status delete_P2SH_Main(P2SH_Main_Address *self);
// ALlocate on stack memory.
Status Initialize_P2SH_Main(P2SH_Main_Address *addr, bool compress);
Status Abandon_P2SH_Main(P2SH_Main_Address *addr);

/******************** P2SH Testnet Address ********************/
typedef struct p2sh_test_address_st P2SH_Test_Address;
struct p2sh_test_address_st {
	root_Address root;
	uint8_t b58_address[36]; // 35 charaters long plus '\0'
	BYTE * (*get_priv_raw)(P2SH_Test_Address *);
	BYTE * (*get_pub_raw)(P2SH_Test_Address *);
	BYTE * (*get_hash160)(P2SH_Test_Address *);
	uint8_t * (*get_wif)(P2SH_Test_Address *);
	bool compress;
};
// Allocate on heap memory.
P2SH_Test_Address * new_P2SH_Test(bool compress);
P2SH_Test_Address * new_P2SH_Test_from_key(const uint8_t *anyformat, bool compress);
Status delete_P2SH_Test(P2SH_Test_Address *self);
// ALlocate on stack memory.
Status Initialize_P2SH_Test(P2SH_Test_Address *addr, bool compress);
Status Abandon_P2SH_Test(P2SH_Test_Address *addr);

#endif