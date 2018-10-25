#include "internal/crypto/ntt.h"

const uint32_t N = 1 << 18; // 2^18 = 262144
const uint32_t P = (479 << 21) + 1; // 479 * 2^21 + 1 = 1004535809
const uint32_t G = 3;

// a^b (mod m)
uint32_t quick_power_mod(uint32_t a, uint32_t b, uint32_t m)
{
	uint32_t ans = 1;
	a = a % m;
	while (b)
	{
		if (b & 1)
		{
			ans = ans * a % m;
			b--;
		}
		b >>= 1;
		a = a * a % m;
	}
	return ans;
}