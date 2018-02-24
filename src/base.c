#include <openssl/bn.h>
#include "std.h"

int32_t base6encode(BYTE *payload, size_t payload_len, uint8_t *encoded)
{
	const uint8_t *base6table = "012345";

	BYTE payload_hexstr[payload_len*2];

	BIGNUM *bn  = BN_new();		BIGNUM *bn0 = BN_new();		BIGNUM *bn6 = BN_new();
	BN_init(bn);				BN_init(bn0);				BN_init(bn6);

	BIGNUM *dv  = BN_new();		BIGNUM *rem = BN_new();
	BN_init(dv);				BN_init(rem);

	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_init(ctx);

	/* Get leading 0x00 byte count. */
	int32_t leading_zero_count = 0;
	for (int32_t i = 0; i < payload_len; ++i)
	{
		if (payload[i] == 0x00)
			leading_zero_count += 1;
		else break;
	}

	/* Convert the payload from string to a big number. */
	if (bytearr_to_hexstr(payload, payload_len, payload_hexstr) != 0)
		return -2;

	BN_hex2bn(&bn, payload_hexstr);
	BN_set_word(bn0,0);
	BN_set_word(bn6,6);
	BN_set_word(dv,1);

	BN_clear_free(bn);		BN_clear_free(bn0);		BN_clear_free(bn6);
	BN_clear_free(dv);		BN_clear_free(rem);
	BN_CTX_free(ctx);
}

int32_t base6decode(uint8_t *payload, size_t payload_len, BYTE *decoded)
{
	const uint8_t *base6table = "012345";

	BYTE raw_payload[payload_len];

	/* Get b6 value of each charaters in payload string, and check the validation. */
	for (int32_t i = 0; i < payload_len; ++i)
	{
		int32_t j = 0;
		for ( ; j < 6; ++j)
		{
			if (payload[i] == base6table[j])
			{
				raw_payload[i] = j;
				break;
			}
		}
		if (j == 6)
			return -1;
	}
}

int32_t base32encode(BYTE *payload, size_t payload_len, uint8_t *encoded)
{

}

int32_t base32decode(uint8_t *payload, size_t payload_len, BYTE *decoded)
{

}

int32_t base58encode(BYTE *payload, size_t payload_len, uint8_t *encoded)
{
	const uint8_t *base58table = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

	uint8_t payload_hexstr[payload_len*2];
	uint8_t raw_encoded[payload_len*2];

	BIGNUM *bn  = BN_new();		BIGNUM *bn0 = BN_new();		BIGNUM *bn58 = BN_new();
	BN_init(bn);				BN_init(bn0);				BN_init(bn58);

	BIGNUM *dv  = BN_new();		BIGNUM *rem = BN_new();
	BN_init(dv);				BN_init(rem);

	BN_CTX *ctx = BN_CTX_new();
	BN_CTX_init(ctx);

	/* Get leading 0x00 byte count. */
	int32_t leading_zero_count = 0;
	for (int32_t i = 0; i < payload_len; ++i)
	{
		if (payload[i] == 0x00)
			leading_zero_count += 1;
		else break;
	}

	/* Convert the payload from string to a big number. */
	if (bytearr_to_hexstr(payload, payload_len, payload_hexstr) != 0)
		return -1;

	BN_hex2bn(&bn, payload_hexstr);
	BN_set_word(bn0,0);
	BN_set_word(bn58,58);
	BN_set_word(dv,1);

	/* Get the raw encoded payload (need to be reversed). */
	int32_t raw_encoded_len = 0;
	while(BN_cmp(dv, bn0) > 0)
	{	
		BN_div(dv, rem, bn, bn58, ctx);
		BN_copy(bn, dv);
		raw_encoded[raw_encoded_len] = base58table[BN_get_word(rem)];
		raw_encoded_len++;
	}

	/* Add the leading '1' charater and reverse the raw encoded data. */
	int32_t encoded_len = raw_encoded_len + leading_zero_count;

	if (encoded == NULL)
		return encoded_len;

	for (int32_t i = 0; i < leading_zero_count; ++i)
		encoded[i] = '1';

	for (int32_t i = 0; i < raw_encoded_len; ++i)
		encoded[encoded_len - 1 -i] = raw_encoded[i];
	encoded[encoded_len] = '\0';

	BN_clear_free(bn);		BN_clear_free(bn0);		BN_clear_free(bn58);
	BN_clear_free(dv);		BN_clear_free(rem);
	BN_CTX_free(ctx);

	return 0;
}

int32_t base58decode(uint8_t *payload, size_t payload_len, BYTE *decoded)
{
	const uint8_t *base58table = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

	/* Get the count of leading '1' charater. */
	int32_t leading_one_count = 0;

	for (int32_t i = 0; ; ++i)
	{
		if (payload[i] == '1')
			leading_one_count++;
		else break;
	}

	int32_t raw_payload_len = payload_len - leading_one_count;
	uint8_t raw_payload[raw_payload_len];

	/* Get the leading '1' striped raw payload. */
	for (int32_t i = 0; i < payload_len; ++i)
		raw_payload[i] = payload[leading_one_count + i];

	/* Get b58 value of each charaters in raw payload string, and check the validation. */
	for (int32_t i = 0; i < raw_payload_len; ++i)
	{
		int32_t j = 0;
		for ( ; j < 58; ++j)
		{
			if (raw_payload[i] == base58table[j])
			{
				raw_payload[i] = j;
				break;
			}
		}
		if (j == 58)
			return -1;
	}

	/* Convert b58 value array to a big number. */
	// to_add = b58_value * 58 ^ power;
	// to_add = b58_value * powered;
	// bignum = bignum + to_add;
	BIGNUM *bn 	   = BN_new();	 BIGNUM *bn58 	 = BN_new();		BIGNUM *b58value = BN_new();
	BN_init(bn);				 BN_init(bn58);						BN_init(b58value);
	BN_is_zero(bn);				 BN_set_word(bn58, 58);

	BIGNUM *power  = BN_new(); 	 BIGNUM *powered = BN_new();
	BN_init(power);				 BN_init(powered);

	BIGNUM *to_add = BN_new();	 BIGNUM *buffer  = BN_new();
	BN_init(to_add);			 BN_init(buffer);
	BN_is_zero(to_add);			 BN_is_zero(buffer);

	BN_CTX *ctx1 = BN_CTX_new(); BN_CTX *ctx2 = BN_CTX_new();

	for (int32_t i = 0; i < raw_payload_len; ++i)
	{
		BN_set_word(b58value, raw_payload[i]);
		BN_set_word(power, raw_payload_len - 1 - i);
		BN_exp(powered, bn58, power, ctx1);
		BN_mul(to_add, b58value, powered, ctx2);
		BN_add(bn, buffer, to_add);
		BN_swap(bn, buffer);
	}
	BN_swap(bn, buffer);

	/* Convert the big number to hexadecimal string. */
	char *raw_decoded_hexstr;
	raw_decoded_hexstr = BN_bn2hex(bn);

	/* Convert raw decoded hexadecimal string to byte array. */
	int32_t decoded_len = get_strlen(raw_decoded_hexstr) / 2 + leading_one_count;
	if (decoded == NULL)
		return decoded_len;

	hexstr_to_bytearr(raw_decoded_hexstr, get_strlen(raw_decoded_hexstr), decoded);
	OPENSSL_free(raw_decoded_hexstr);

	/* Add the leading 0x00 byte. */
	for (int32_t i = 0; i < decoded_len; ++i)
		decoded[decoded_len - 1 -i] = decoded[decoded_len - 1 - leading_one_count -i];
	for (int32_t i = 0; i < leading_one_count; ++i)
		decoded[i] = 0x00;

	BN_clear_free(bn);			BN_clear_free(bn58);		BN_clear_free(b58value);
	BN_clear_free(power);		BN_clear_free(powered);
	BN_clear_free(to_add);		BN_clear_free(buffer);
	BN_CTX_free(ctx1);			BN_CTX_free(ctx2);

	return 0;
}

int32_t base58check_encode(BYTE *payload, size_t payload_len, uint8_t *encoded)
{

}

int32_t base58check_decode(BYTE *payload, size_t payload_len, uint8_t *encoded)
{

}

int32_t base64encode(BYTE *payload, size_t payload_len, uint8_t *encoded)
{
	const uint8_t *base64table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int32_t encoded_len;
	int32_t ending = payload_len % 3;

	if (encoded == NULL)
	{
		if (ending != 0)
		{
			encoded_len = payload_len / 3 * 4 + 4;
			return encoded_len;
		}
		else{
			encoded_len = payload_len / 3 * 4;
			return encoded_len;
		}
	}

//	00 01 02     03 04 05     06 07 08     09 10 11     12 13 14    15 16
//	00 01 02 03  04 05 06 07  08 09 10 11  12 13 14 15  16 17 18 19 20 21 22 00
	for (int32_t i = 0; i < payload_len - ending; i+=3)
	{
		int32_t num = i + i / 3;
		encoded[num] = base64table[payload[i] >> 2];
		encoded[num+1] = base64table[((payload[i] & 0x03) << 4) | (payload[i+1] >> 4)];
		encoded[num+2] = base64table[((payload[i+1] & 0x0f) << 2) | (payload[i+2] >> 6)];
		encoded[num+3] = base64table[payload[i+2] & 0x3f];
	}

	if (ending != 0)
	{	
		int32_t start = (payload_len - ending)/3 + (payload_len - ending);
		BYTE buffer[3] = {0x00, 0x00, 0x00};
		for (int32_t i = 0; i < ending; ++i)
			buffer[i] = payload[payload_len - ending + i];

		encoded[start] = base64table[buffer[0] >> 2] == 'A' ? '=' : base64table[buffer[0] >> 2];
		encoded[start+1] = base64table[((buffer[0] & 0x03) << 4) | (buffer[1] >> 4)] == 'A' ? '=' : base64table[((buffer[0] & 0x03) << 4) | (buffer[1] >> 4)];
		encoded[start+2] = base64table[((buffer[1] & 0x0f) << 2) | (buffer[2] >> 6)] == 'A' ? '=' : base64table[((buffer[1] & 0x0f) << 2) | (buffer[2] >> 6)];
		encoded[start+3] = base64table[buffer[2] & 0x3f] == 'A' ? '=' : base64table[buffer[2] & 0x3f];
		encoded[start+4] = '\0';

		return 0;
	}
	else return 0;
}

int32_t base64decode(uint8_t *payload, size_t payload_len, BYTE *decoded)
{
	const uint8_t *base64table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int32_t ending = 0;
	for (int32_t i = payload_len - 1; i >= 0; --i )
	{
		if (payload[i] == '=')
			ending++;
		else break;
	}

	int32_t decoded_len = ending ? (payload_len - 4) / 4 * 3 + 3 - ending : payload_len / 4 * 3;
	if (decoded == NULL)
		return decoded_len;
	int32_t payload_copy_len = (ending == 0 ? payload_len : payload_len - 4);
	BYTE payload_copy[payload_copy_len];

	/* Get b64 value of each charaters in payload string, and check the validation. */
	for (int32_t i = 0; i < payload_copy_len; ++i)
	{
		int32_t j = 0;
		for ( ; j < 64; ++j)
		{
			if (payload[i] == base64table[j])
			{
				payload_copy[i] = j;
				break;
			}
		}
		if (j == 64)
			return -1;
	}

//	00 01 02 03  04 05 06 07  08 09 10 11  12 13 14 15  16 17 18 19 20 21 22 00
//	00 01 02     03 04 05     06 07 08     09 10 11     12 13 14    15 16
	for (int32_t i = 0; i < payload_copy_len; i+=4)
	{
		int32_t num = i - i / 4;
	}

	if (ending != 0)
	{
		BYTE payload_copy_end[4] = {0x00, 0x00, 0x00, 0x00};
		for (int32_t i = 0; i < 4; ++i)
			payload_copy_end[i] = payload[payload_copy_len + i];
	}
}