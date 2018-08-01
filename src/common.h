#define BITCOIN_TOOLKIT_VERSION "v0.0.1"

#ifndef _COMMON_
#define _COMMON_

#include <stdbool.h> // For 'bool' type
#include <stdint.h>  // For integer types
#include <stddef.h>  // For 'size_t' type

typedef uint8_t BYTE;

#define MEMORY_ALLOCATE_FAILED   (void *)0x000000
#define INDEX_OUT_RANGE          (void *)0xFFF001
#define INVALID_PUBKEY_HASH_SIZE (void *)0xFFF002
#define INVALID_PUBKEY_SIZE      (void *)0xFFF003

#endif