#define BITCOIN_TOOLKIT_VERSION "v0.0.1"

#ifndef _COMMON_
#define _COMMON_

#include <stdbool.h> // For 'bool' type
#include <stdint.h>  // For integer types
#include <stddef.h>  // For 'size_t' type
#include <float.h>
typedef uint8_t BYTE;

// 0x000000 ~ 0x000fff : Common reserved
// 0x001000 ~ 0x001fff : CLinkedlist
// 0x002000 ~ 0x002fff : CStack
// 0x003000 ~ 0x003fff : Script
// 0x004000 ~ 0x004fff : Interpreter

#define FAILED                 (void *)0x000000
#define SUCCESS                (void *)0x000001
#define MEMORY_ALLOCATE_FAILED (void *)0x000002

#define BYTE_TYPE    (void *)0x000003
#define INT8_TYPE    (void *)0x000004
#define INT16_TYPE   (void *)0x000005
#define INT32_TYPE   (void *)0x000006
#define INT64_TYPE   (void *)0x000007
#define UINT8_TYPE   (void *)0x000008
#define UINT16_TYPE  (void *)0x000009
#define UINT32_TYPE  (void *)0x00000A
#define UINT64_TYPE  (void *)0x00000B
#define FLOAT32_TYPE (void *)0x00000C
#define FLOAT64_TYPE (void *)0x00000D
#define BOOL_TYPE    (void *)0x00000E

#define INDEX_OUT_RANGE        (void *)0xFFF000
#define INVALID_RIPEMD160_SIZE (void *)0xFFF001
#define INVALID_SHA256_SIZE    (void *)0xFFF002
#define INVALID_PUBKEY_SIZE    (void *)0xFFF003
#define INVALID_PRIVKEY_SIZE   (void *)0xFFF004
#define PASSING_NULL_POINTER   (void *)0xFFF005

#endif