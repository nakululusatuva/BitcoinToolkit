/** 
*  MIT LICENSE
*  Copyright (c) 2018 Yirain Suen
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to
*  deal in the Software without restriction, including without limitation the
*  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
*  sell copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*  
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*  
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
*  IN THE SOFTWARE.
**/
/** 
*  AUTO-GENERATED CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
*  Maintainers: Do not include this header internal to this library.
**/
#ifdef __cpluscplus
extern "C" {
#endif
#ifndef _COMMON_
#define _COMMON_

// For 'bool' type
#if !defined(bool)
#include <stdbool.h>
#endif

// For integer types
#if !defined(int8_t) || !defined(uint8_t) || \
	!defined(int16_t) || !defined(uint16_t) || \
	!defined(int32_t) || !defined(uint32_t) || \
	!defined(int64_t) || !defined(uint64_t) || \
	!defined(intptr_t) || !defined(uintptr_t)
#include <stdint.h>
#endif

// For 'size_t' type
#if !defined(size_t)
#include <stddef.h>
#endif

typedef uint8_t byte;
typedef void* Status;

/* 0x0000 ~ 0x00ff : Common reserved */
#define FAILED                 (Status)0x0000
#define SUCCEEDED              (Status)0x0001
#define BYTE_TYPE              (Status)0x0002
#define INT8_TYPE              (Status)0x0003
#define INT16_TYPE             (Status)0x0004
#define INT32_TYPE             (Status)0x0005
#define INT64_TYPE             (Status)0x0006
#define UINT8_TYPE             (Status)0x0007
#define UINT16_TYPE            (Status)0x0008
#define UINT32_TYPE            (Status)0x0009
#define UINT64_TYPE            (Status)0x000A
#define FLOAT32_TYPE           (Status)0x000B
#define FLOAT64_TYPE           (Status)0x000C
#define BOOL_TYPE              (Status)0x000D
/* 0x0100 ~ 0x0fff : Global */
#define INDEX_OUT_RANGE        (Status)0x0100
#define INVALID_RIPEMD160_SIZE (Status)0x0101
#define INVALID_SHA256_SIZE    (Status)0x0102
#define INVALID_PUBKEY_SIZE    (Status)0x0103
#define INVALID_PRIVKEY_SIZE   (Status)0x0104
#define PASSING_NULL_POINTER   (Status)0x0105
#define MEMORY_ALLOCATE_FAILED (Status)0x0102
#define UNSUPPORTED_DATATYPE   (Status)0x0103

#endif
#ifdef __cpluscplus
}
#endif
