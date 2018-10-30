/** AUTOHEADER TAG: DELETE BEGIN **/
#ifndef _CRYPTO_NTT_
#define _CRYPTO_NTT_

// P^2 < UINT64_T MAX
// P*2 < UINT32_T MAX
#define N 134217728 // 2^27
#define P 2013265921 // 15 * 2^27 + 1
#define G 31

#include "internal/common.h"


/** AUTOHEADER TAG: DELETE END **/
// a^b (mod m)
uint64_t quick_power_mod(uint64_t a, uint64_t b, uint64_t m);
// overflow when point > 0xaf45
void ntt(uint64_t *a, uint32_t len, bool on);

/** AUTOHEADER TAG: DELETE BEGIN **/
void get_Wn();
void rader(uint64_t *a, uint32_t len);
#endif
/** AUTOHEADER TAG: DELETE END **/