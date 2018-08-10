#define BITCOIN_TOOLKIT_VERSION "v0.0.1"

#ifndef _COMMON_
#define _COMMON_

#include <stdbool.h> // For 'bool' type
#include <stdint.h>  // For integer types
#include <stddef.h>  // For 'size_t' type

typedef uint8_t BYTE;

#define FAILED                   (void *)0x000000
#define SUCCESS                  (void *)0x000001
#define MEMORY_ALLOCATE_FAILED   (void *)0x000002
#define INDEX_OUT_RANGE          (void *)0xFFF000
#define INVALID_RIPEMD160_SIZE   (void *)0xFFF001
#define INVALID_SHA256_SIZE      (void *)0xFFF002
#define INVALID_PUBKEY_SIZE      (void *)0xFFF003
#define INVALID_PRIVKEY_SIZE     (void *)0xFFF004
#define PASSING_NULL_POINTER     (void *)0xFFF005

#endif