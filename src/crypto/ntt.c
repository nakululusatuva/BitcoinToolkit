#include "internal/codec/strings.h"
#include "internal/crypto/ntt.h"

uint64_t Wn[20];

// a^b (mod m)
uint64_t quick_power_mod(uint64_t a, uint64_t b, uint64_t m)
{
	uint64_t ans = 1;
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

// overflow when point > 0xaf45
void ntt(uint64_t *a, uint32_t len, bool on)
{
	get_Wn();
	rader(a, len);
	uint32_t id = 0;

	for(uint32_t h = 2; h <= len; h <<= 1)
	{
		id++;
		for(uint32_t j = 0; j < len; j += h)
		{
			uint64_t w = 1;
			for(uint32_t k = j; k < j + h / 2; k++)
			{
				uint64_t u = a[k] % P;
				uint64_t t = w * a[k + h / 2] % P;
				a[k] = (u + t) % P;
				a[k + h / 2] = (u - t + P) % P;
				w = w * Wn[id] % P;
			}
		}
	}

	if(on == true)
	{
		for(uint32_t i = 1; i < len / 2; i++)
			integer_swap(&a[i], &a[len - i], UINT64_TYPE);

		uint64_t inv = quick_power_mod(len, P - 2, P);

		for(uint32_t i = 0; i < len; i++)
			a[i] = a[i] * inv % P;
	}
}

void get_Wn()
{
	for(uint32_t i = 0; i < 20; ++i)
	{
		uint32_t t = 1 << i;
		Wn[i] = quick_power_mod(G, (P - 1) / t, P);
	}
}

void rader(uint64_t *a, uint32_t len)
{
	uint32_t j = len >> 1;
	for (uint32_t i = 1; i < len - 1; ++i)
	{
		if (i < j) integer_swap(&a[i], &a[j], UINT64_TYPE);
		uint32_t k = len >> 1;
		while (j >= k)
		{
			j -= k;
			k >>= 1;
		}
		if (j < k) j += k;
	}
}