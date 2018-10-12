#ifndef _BIGINT_
#define _BIGINT_

#include "../common.h"

typedef struct Bigint_st Bigint;
struct Bigint_st {
	uint32_t *d;
	uint32_t top;
	bool neg;
};

Bigint * new_Bigint(void);
void delete_Bigint(Bigint *bn);
Status Bigint_set_bytearr(Bigint *bn, BYTE *num, size_t length);
Status Bigint_set_hexstr(Bigint *bn, uint8_t *num, size_t length);
Bigint * Bigint_add(Bigint *a, Bigint *b);
Bigint * Bigint_sub(Bigint *a, Bigint *b);
Bigint * Bigint_mul(Bigint *a, Bigint *b);
Bigint * Bigint_div(Bigint *a, Bigint *b);
Bigint * Bigint_pow(Bigint *a, Bigint *order);
Bigint * Bigint_ext(Bigint *a, Bigint *order);

#endif