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
#include "common.h"



typedef struct Bigint_st Bigint;
struct Bigint_st {
	uint32_t *d; // Little-endian.
	uint32_t len;
	bool neg;
};

Bigint * new_Bigint(void);
void delete_Bigint(Bigint *bn);
// Receive a byte array in little-endian.
Bigint * Bigint_set_bytearr(BYTE *arr, size_t len, bool neg);
// Receive a hexadecimal string in little-endian, first charater '-' means negative.
Bigint Bigint_set_hexstr(Bigint *bn, uint8_t *str, size_t len);
Bigint * Bigint_add(Bigint *a, Bigint *b);
Bigint * Bigint_sub(Bigint *a, Bigint *b);
Bigint * Bigint_mul(Bigint *a, Bigint *b);
Bigint * Bigint_div(Bigint *a, Bigint *b);
Bigint * Bigint_pow(Bigint *a, Bigint *order);
Bigint * Bigint_ext(Bigint *a, Bigint *order);

#ifdef __cpluscplus
}
#endif
