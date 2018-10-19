/** AUTOHEADER TAG: DELETE BEGIN **/
#ifndef _BIGINT_
#define _BIGINT_

#include "internal/common.h"
/** AUTOHEADER TAG: DELETE END **/

typedef struct Bigint_st Bigint;
struct Bigint_st {
	uint32_t *d;
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

/** AUTOHEADER TAG: DELETE BEGIN **/
#endif
/** AUTOHEADER TAG: DELETE END **/