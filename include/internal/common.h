#ifndef _COMMON_
#define _COMMON_

#include <stdbool.h> // For 'bool' type
#include <stdint.h>  // For integer types
#include <stddef.h>  // For 'size_t' type
#include <float.h>
typedef uint8_t BYTE;
typedef void* Status;

/* 0x0000 ~ 0x00ff : Common reserved */
#define FAILED                 (void *)0x0000
#define SUCCEEDED              (void *)0x0001
#define BYTE_TYPE              (void *)0x0002
#define INT8_TYPE              (void *)0x0003
#define INT16_TYPE             (void *)0x0004
#define INT32_TYPE             (void *)0x0005
#define INT64_TYPE             (void *)0x0006
#define UINT8_TYPE             (void *)0x0007
#define UINT16_TYPE            (void *)0x0008
#define UINT32_TYPE            (void *)0x0009
#define UINT64_TYPE            (void *)0x000A
#define FLOAT32_TYPE           (void *)0x000B
#define FLOAT64_TYPE           (void *)0x000C
#define BOOL_TYPE              (void *)0x000D
/* 0x0100 ~ 0x0fff : Global */
#define INDEX_OUT_RANGE        (void *)0x0100
#define INVALID_RIPEMD160_SIZE (void *)0x0101
#define INVALID_SHA256_SIZE    (void *)0x0102
#define INVALID_PUBKEY_SIZE    (void *)0x0103
#define INVALID_PRIVKEY_SIZE   (void *)0x0104
#define PASSING_NULL_POINTER   (void *)0x0105
#define MEMORY_ALLOCATE_FAILED (void *)0x0102

#endif