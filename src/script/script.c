#include <stdlib.h>
#include <string.h>
#include "../common.h"
#include "../codec/codec.h"
#include "../container/CStack.h"
#include "../container/CLinkedlist.h"
#include "script.h"

Script * new_Script()
{
	Script *new = (Script *)calloc(1, sizeof(Script));
	if (new == NULL)
		return NULL;

	new->script = new_CLinkedlist();
	if (new->script == NULL)
	{
		free(new);
		return NULL;
	}
	else
	{
		new->add_opcode = &Script_add_opcode;
		new->add_data   = &Script_add_data;
		new->to_string  = &Script_to_string;
		new->to_bytes   = &Script_to_bytes;
		return new;
	}
}

Script * new_Script_from_bytes(BYTE *bytes, size_t size)
{
	Script *new = new_Script();
	if (new == NULL)
		return NULL;
	uint32_t pos = 0;

while (true) // Can't use break and continue to control the loop if I use switch-case.
{
// Check if opcode.
	if (is_opcode(bytes[pos]))
	{
		// Check if PUSHDATA1.
		if (bytes[pos] == OP_PUSHDATA1)
		{
			// Check if enough bytes remain.
			uint32_t remain = size-pos-1;
			uint32_t length = bytes[pos+1];
			if (remain < length)
				return NULL;

			// Add opcode.
			opcode *op = (opcode *)calloc(1, sizeof(opcode));
			if (op == NULL)
				return NULL;
			*op = bytes[pos];
			new->add_opcode(new, op);

			// Add data.
			BYTE *data = (BYTE *)calloc(length + 1, sizeof(BYTE));
			if (data == NULL)
				return NULL;
			data[0] = bytes[pos+1];
			for (uint32_t j = 0; j < length; ++j)
				data[j+1] = bytes[pos + 2 + j];
			new->add_data(new, data, length + 1);

			// Shift pos, break the loop if pos at the end.
			pos = pos + 1 + length + 1;
			if (pos == size)
				break;
			else continue;
		}

		// Check if PUSHDATA2.
		else if (bytes[pos] == OP_PUSHDATA2)
		{
			// Check if enough bytes remain.
			uint32_t remain = size-pos-1;
			uint32_t length = bytes[pos+1];
			         length = (length << 8) + bytes[pos+2];
			if (remain < length)
				return NULL;

			// Add opcode.
			opcode *op = (opcode *)calloc(1, sizeof(opcode));
			if (op == NULL)
				return NULL;
			*op = bytes[pos];
			new->add_opcode(new, op);

			// Add data.
			BYTE *data = (BYTE *)calloc(length + 2, sizeof(BYTE));
			if (data == NULL)
				return NULL;
			data[0] = bytes[pos+1];
			data[1] = bytes[pos+2];
			for (uint32_t j = 0; j < length; ++j)
				data[j+2] = bytes[pos + 3 + j];
			new->add_data(new, data, length + 2);

			// Shift pos, break the loop if pos at the end.
			pos = pos + 2 + length + 1;
			if (pos == size)
				break;
			else continue;
		}

		// Check if PUSHDATA4.
		else if (bytes[pos] == OP_PUSHDATA4)
		{
			// Check if enough bytes remain.
			uint32_t remain = size-pos-1;
			uint32_t length = bytes[pos+1];
			         length = (length << 8) + bytes[pos+2];
			         length = (length << 8) + bytes[pos+3];
			         length = (length << 8) + bytes[pos+4];
			if (remain < length)
				return NULL;

			// Add opcode.
			opcode *op = (opcode *)calloc(1, sizeof(opcode));
			if (op == NULL)
				return NULL;
			*op = bytes[pos];
			new->add_opcode(new, op);

			// Add data.
			BYTE *data = (BYTE *)calloc(length + 4, sizeof(BYTE));
			if (data == NULL)
				return NULL;
			data[0] = bytes[pos+1];
			data[1] = bytes[pos+2];
			data[2] = bytes[pos+3];
			data[3] = bytes[pos+4];
			for (uint32_t j = 0; j < length; ++j)
				data[j+4] = bytes[pos + 5 + j];
			new->add_data(new, data, length + 4);

			// Shift pos, break the loop if pos at the end.
			pos = pos + 4 + length + 1;
			if (pos == size)
				break;
			else continue;
		}

		// Not PUSHDATA1 / PUSHDATA2 / PUSHDATA4.
		else
		{
			// Add opcode.
			opcode *op = (opcode *)calloc(1, sizeof(opcode));
			if (op == NULL)
				return NULL;
			*op = bytes[pos];
			new->add_opcode(new, op);

			// Shift pos, break the loop if pos at the end.
			pos = pos + 1;
			if (pos == size)
				break;
			else continue;
		}
	}

	// Check if PUSHDATA (1 to 75).
	else if (bytes[pos] >= 0x01 && bytes[pos] <= 0x4b)
	{
		// Check if enough bytes remain.
		uint32_t remain = size-pos-1;
		uint32_t length = bytes[pos];
		if (remain < length)
			return NULL;

		// Add PUSHDATA byte.
		opcode *op = (opcode *)calloc(1, sizeof(opcode));
		if (op == NULL)
			return NULL;
		*op = length;
		new->add_opcode(new, op);

		// Add data.
		BYTE *data = (BYTE *)calloc(length, sizeof(BYTE));
		if (data == NULL)
			return NULL;
		for (uint32_t j = 0; j < length; ++j)
			data[j] = bytes[pos + 1 + j];
		new->add_data(new, data, length);

		// Shift pos, break the loop if pos at the end.
		pos = pos + 0 + length + 1;
		if (pos == size)
			break;
		else continue;
	}

	// Unknow opcode.
	else
	{
		// Add OP_INVALIDOPCODE.
		opcode *op = (opcode *)calloc(1, sizeof(opcode));
		if (op == NULL)
			return NULL;
		*op = OP_INVALIDOPCODE;
		new->add_opcode(new, op);

		// Shift pos, break the loop if pos at the end.
		pos = pos + 1;
		if (pos == size)
			break;
		else continue;
	}
}
	return new;
}

Script * new_Script_assembled(Script *p1, Script *p2)
{
	Script *new = new_Script();
	CLinkedlistNode **p1_list = p1->script->forward_traversing(p1->script);
	CLinkedlistNode **p2_list = p2->script->forward_traversing(p2->script);
	if (new == NULL || p1_list == NULL || p2_list == NULL)
		return NULL;

	for (uint32_t i = 0; i < p1->script->length; ++i)
	{
		if (p1_list[i]->size == 1)
		{
			opcode *op = (opcode *)malloc(sizeof(opcode));
			memcpy(op, p1_list[i]->data, 1);
			bool ret = new->add_opcode(new, op);
			if (ret == false)
				return NULL;
		}
		else
		{
			size_t data_size = p1_list[i]->size;
			BYTE *data = (BYTE *)malloc(data_size);
			memcpy(data, p1_list[i]->data, data_size);
			bool ret = new->add_data(new, data, data_size);
			if (ret == false)
				return NULL;
		}
	}

	for (uint32_t i = 0; i < p2->script->length; ++i)
	{
		if (p2_list[i]->size == 1)
		{
			opcode *op = (opcode *)malloc(sizeof(opcode));
			memcpy(op, p2_list[i]->data, 1);
			bool ret = new->add_opcode(new, op);
			if (ret == false)
				return NULL;
		}
		else
		{
			size_t data_size = p2_list[i]->size;
			BYTE *data = (BYTE *)malloc(data_size);
			memcpy(data, p2_list[i]->data, data_size);
			bool ret = new->add_data(new, data, data_size);
			if (ret == false)
				return NULL;
		}
	}

	free(p1_list);
	free(p2_list);

	return new;
}

Script * new_Script_p2pkh(BYTE *pubkey_hash, size_t size)
{
	Script *new = new_Script();
	if (new == NULL || size <= 0)
		return NULL;

	opcode *dup = (opcode *)malloc(sizeof(opcode));
	opcode *hash160 = (opcode *)malloc(sizeof(opcode));
	BYTE *pushdata = (BYTE *)malloc(sizeof(BYTE));
	BYTE *data = (BYTE *)malloc(size);
	opcode *equalverify = (opcode *)malloc(sizeof(opcode));
	opcode *checksig = (opcode *)malloc(sizeof(opcode));

	if (dup == NULL || hash160 == NULL || pushdata == NULL ||
		data == NULL || equalverify == NULL || checksig == NULL)
		return NULL;

	*dup         = OP_DUP;
	*hash160     = OP_HASH160;
	*pushdata    = size;
	*equalverify = OP_EQUALVERIFY;
	*checksig    = OP_CHECKSIG;
	memcpy(data, pubkey_hash, size);

	bool ret1 = new->add_opcode(new, dup);
	bool ret2 = new->add_opcode(new, hash160);
	bool ret3 = new->add_opcode(new, pushdata);
	bool ret4 = new->add_data(new, data, size);
	bool ret5 = new->add_opcode(new, equalverify);
	bool ret6 = new->add_opcode(new, checksig);

	if (ret1 == false || ret2 == false || ret3 == false ||
		ret4 == false || ret5 == false || ret6 == false)
		return NULL;

	return NULL;
}

Script * new_Script_p2pk(BYTE *pubkey, size_t size)
{

}

Script * new_Script_p2sh(BYTE *script_hash, size_t size)
{

}

Script * new_Script_multisig(uint8_t m, CLinkedlist *pubkeys)
{
	if ( !(m > 0 && m <= pubkeys->length) )
		return NULL;

	Script *script = new_Script();
	if (script == NULL)
		return NULL;

	// Add first opcode: OP_M.
	opcode *OP_M = (opcode *)calloc(1, sizeof(opcode));
	if (OP_M == NULL)
		return NULL;
	*OP_M = 0x50 + m;
	bool ret1 = script->add_opcode(script, OP_M);
	if (ret1 == false)
		return NULL;

	// Add public keys.
	CLinkedlistNode **keys = pubkeys->forward_traversing(pubkeys);
	if (keys == NULL)
		return NULL;
	for (uint32_t i = 0; i < pubkeys->length; ++i)
	{
		// Add PUSHDATA().
		BYTE *PUSHDATA = (BYTE *)calloc(1, sizeof(BYTE));
		if (PUSHDATA == NULL)
			return NULL;
		*PUSHDATA = keys[i]->size;
		bool ret2 = script->add_data(script, PUSHDATA, 1);
		if (ret2 == false)
			return NULL;

		// Add public key bytes.
		BYTE *pub = (BYTE *)malloc(keys[i]->size);
		if (pub == NULL)
			return NULL;
		memcpy(pub, keys[i]->data, keys[i]->size);
		bool ret3 = script->add_data(script, pub, keys[i]->size);
		if (ret3 == false)
			return NULL;
	}
	free(keys);

	// Add OP_N.
	opcode *OP_N = (opcode *)calloc(1, sizeof(opcode));
	if (OP_N == NULL)
		return NULL;
	*OP_N = 0x50 + (pubkeys->length);
	bool ret4 = script->add_opcode(script, OP_N);
	if (ret4 == false)
		return NULL;

	// Add OP_CHECKMULTISIG
	opcode *CHECKMULTISIG = (opcode *)calloc(1, sizeof(opcode));
	if (CHECKMULTISIG == NULL)
		return NULL;
	*CHECKMULTISIG = OP_CHECKMULTISIG;
	bool ret5 = script->add_opcode(script, CHECKMULTISIG);
	if (ret5 == false)
		return NULL;

	return script;
}

void delete_Script(Script *self)
{
	delete_CLinkedlist(self->script);
	free(self);
}

bool is_opcode(BYTE byte)
{	
	if ( byte == 0x00 || (byte >= 0x4c && byte <= 0xb9) || byte == 0xff)
		return true;
	else return false;
}

const char * get_op_name(opcode op)
{
    switch (op)
    {
    // push value
    case OP_0                      : return "0";
    case OP_PUSHDATA1              : return "OP_PUSHDATA1";
    case OP_PUSHDATA2              : return "OP_PUSHDATA2";
    case OP_PUSHDATA4              : return "OP_PUSHDATA4";
    case OP_1NEGATE                : return "-1";
    case OP_RESERVED               : return "OP_RESERVED";
    case OP_1                      : return "1";
    case OP_2                      : return "2";
    case OP_3                      : return "3";
    case OP_4                      : return "4";
    case OP_5                      : return "5";
    case OP_6                      : return "6";
    case OP_7                      : return "7";
    case OP_8                      : return "8";
    case OP_9                      : return "9";
    case OP_10                     : return "10";
    case OP_11                     : return "11";
    case OP_12                     : return "12";
    case OP_13                     : return "13";
    case OP_14                     : return "14";
    case OP_15                     : return "15";
    case OP_16                     : return "16";

    // control
    case OP_NOP                    : return "OP_NOP";
    case OP_VER                    : return "OP_VER";
    case OP_IF                     : return "OP_IF";
    case OP_NOTIF                  : return "OP_NOTIF";
    case OP_VERIF                  : return "OP_VERIF";
    case OP_VERNOTIF               : return "OP_VERNOTIF";
    case OP_ELSE                   : return "OP_ELSE";
    case OP_ENDIF                  : return "OP_ENDIF";
    case OP_VERIFY                 : return "OP_VERIFY";
    case OP_RETURN                 : return "OP_RETURN";

    // stack ops
    case OP_TOALTSTACK             : return "OP_TOALTSTACK";
    case OP_FROMALTSTACK           : return "OP_FROMALTSTACK";
    case OP_2DROP                  : return "OP_2DROP";
    case OP_2DUP                   : return "OP_2DUP";
    case OP_3DUP                   : return "OP_3DUP";
    case OP_2OVER                  : return "OP_2OVER";
    case OP_2ROT                   : return "OP_2ROT";
    case OP_2SWAP                  : return "OP_2SWAP";
    case OP_IFDUP                  : return "OP_IFDUP";
    case OP_DEPTH                  : return "OP_DEPTH";
    case OP_DROP                   : return "OP_DROP";
    case OP_DUP                    : return "OP_DUP";
    case OP_NIP                    : return "OP_NIP";
    case OP_OVER                   : return "OP_OVER";
    case OP_PICK                   : return "OP_PICK";
    case OP_ROLL                   : return "OP_ROLL";
    case OP_ROT                    : return "OP_ROT";
    case OP_SWAP                   : return "OP_SWAP";
    case OP_TUCK                   : return "OP_TUCK";

    // splice ops
    case OP_CAT                    : return "OP_CAT";
    case OP_SUBSTR                 : return "OP_SUBSTR";
    case OP_LEFT                   : return "OP_LEFT";
    case OP_RIGHT                  : return "OP_RIGHT";
    case OP_SIZE                   : return "OP_SIZE";

    // bit logic
    case OP_INVERT                 : return "OP_INVERT";
    case OP_AND                    : return "OP_AND";
    case OP_OR                     : return "OP_OR";
    case OP_XOR                    : return "OP_XOR";
    case OP_EQUAL                  : return "OP_EQUAL";
    case OP_EQUALVERIFY            : return "OP_EQUALVERIFY";
    case OP_RESERVED1              : return "OP_RESERVED1";
    case OP_RESERVED2              : return "OP_RESERVED2";

    // numeric
    case OP_1ADD                   : return "OP_1ADD";
    case OP_1SUB                   : return "OP_1SUB";
    case OP_2MUL                   : return "OP_2MUL";
    case OP_2DIV                   : return "OP_2DIV";
    case OP_NEGATE                 : return "OP_NEGATE";
    case OP_ABS                    : return "OP_ABS";
    case OP_NOT                    : return "OP_NOT";
    case OP_0NOTEQUAL              : return "OP_0NOTEQUAL";
    case OP_ADD                    : return "OP_ADD";
    case OP_SUB                    : return "OP_SUB";
    case OP_MUL                    : return "OP_MUL";
    case OP_DIV                    : return "OP_DIV";
    case OP_MOD                    : return "OP_MOD";
    case OP_LSHIFT                 : return "OP_LSHIFT";
    case OP_RSHIFT                 : return "OP_RSHIFT";
    case OP_BOOLAND                : return "OP_BOOLAND";
    case OP_BOOLOR                 : return "OP_BOOLOR";
    case OP_NUMEQUAL               : return "OP_NUMEQUAL";
    case OP_NUMEQUALVERIFY         : return "OP_NUMEQUALVERIFY";
    case OP_NUMNOTEQUAL            : return "OP_NUMNOTEQUAL";
    case OP_LESSTHAN               : return "OP_LESSTHAN";
    case OP_GREATERTHAN            : return "OP_GREATERTHAN";
    case OP_LESSTHANOREQUAL        : return "OP_LESSTHANOREQUAL";
    case OP_GREATERTHANOREQUAL     : return "OP_GREATERTHANOREQUAL";
    case OP_MIN                    : return "OP_MIN";
    case OP_MAX                    : return "OP_MAX";
    case OP_WITHIN                 : return "OP_WITHIN";

    // crypto
    case OP_RIPEMD160              : return "OP_RIPEMD160";
    case OP_SHA1                   : return "OP_SHA1";
    case OP_SHA256                 : return "OP_SHA256";
    case OP_HASH160                : return "OP_HASH160";
    case OP_HASH256                : return "OP_HASH256";
    case OP_CODESEPARATOR          : return "OP_CODESEPARATOR";
    case OP_CHECKSIG               : return "OP_CHECKSIG";
    case OP_CHECKSIGVERIFY         : return "OP_CHECKSIGVERIFY";
    case OP_CHECKMULTISIG          : return "OP_CHECKMULTISIG";
    case OP_CHECKMULTISIGVERIFY    : return "OP_CHECKMULTISIGVERIFY";

    // expansion
    case OP_NOP1                   : return "OP_NOP1";
    case OP_CHECKLOCKTIMEVERIFY    : return "OP_CHECKLOCKTIMEVERIFY";
    case OP_CHECKSEQUENCEVERIFY    : return "OP_CHECKSEQUENCEVERIFY";
    case OP_NOP4                   : return "OP_NOP4";
    case OP_NOP5                   : return "OP_NOP5";
    case OP_NOP6                   : return "OP_NOP6";
    case OP_NOP7                   : return "OP_NOP7";
    case OP_NOP8                   : return "OP_NOP8";
    case OP_NOP9                   : return "OP_NOP9";
    case OP_NOP10                  : return "OP_NOP10";

    case OP_INVALIDOPCODE          : return "OP_INVALIDOPCODE";

    default:
        return "OP_UNKNOWN";
    }
}

bool Script_add_opcode(Script *self, opcode *op)
{
	if ( (self->script->add(self->script, op, 1)) )
		return true;
	else return false;
}

bool Script_add_data(Script *self, BYTE *data, size_t size)
{
	if ( (self->script->add(self->script, data, size)) )
		return true;
	else return false;
}

uint8_t * Script_to_string(Script *self, size_t *size)
{
	CLinkedlistNode **statements = self->script->forward_traversing(self->script);
	CLinkedlist *statements_str = new_CLinkedlist();
	if (statements == NULL || statements_str == NULL)
		return NULL;

// Script statements loop, convert one statement to one string and add to linked list.
for (uint32_t i = 0; i < self->script->length; ++i)
{
	switch (statements[i]->size)
	{	// If opcode.
		case 1:
		{
		/**	
		*	Cause error in valgrind if use 'opcode' type, error message:
		*	Conditional jump or move depends on uninitialised value(s)
		*
		*	opcode op_buffer = ((opcode *)(statements[i]->data))[0];
		**/
			BYTE op_buffer = ((BYTE *)(statements[i]->data))[0];

			// Opcode is PUSHDATA (1 ~ 75): "PUSHDATA(size)"
			if (op_buffer >= 0x01 && op_buffer <= 0x4b)
			{
				int8_t size_str[3];
				BYTE  size_byte[1];
				size_byte[0] = op_buffer;
				bytearr_to_hexstr(size_byte, 1, size_str);

				uint8_t *str = (uint8_t *)calloc(12, sizeof(uint8_t));
				if (str == NULL)
					return NULL;
				str[0] = 'P'; str[1] = 'U'; str[2] = 'S'; str[3] = 'H';
				str[4] = 'D'; str[5] = 'A'; str[6] = 'T'; str[7] = 'A';
				str[8] = '(';
				str[9] = size_str[0]; str[10] = size_str[1];
				str[11] = ')';

				bool ret1 = statements_str->add(statements_str, str, 12 * sizeof(uint8_t));
				if (ret1 == false)
					return NULL;
				break;
			}

			// Opcode is PUSHDATA1/2/4: "OP_PUSHDATA1" "OP_PUSHDATA2" "OP_PUSHDATA4"
			else if (op_buffer == OP_PUSHDATA1 || op_buffer == OP_PUSHDATA2 || op_buffer == OP_PUSHDATA4)
			{
				const char * op_name = get_op_name(op_buffer);
				size_t len = strlen(op_name);
				uint8_t *str = (uint8_t *)calloc(len, sizeof(uint8_t));
				if (str == NULL)
					return NULL;

				memcpy(str, op_name, len);
				bool ret2 = statements_str->add(statements_str, str, len * sizeof(uint8_t));
				if (ret2 == false)
					return NULL;
				break;
			}

			// Opcode is not PUSHDATA(1 ~ 75)/1/2/4: "OP_XXXXX "
			else
			{
				const char * op_name = get_op_name(op_buffer);
				size_t len = strlen(op_name) + 1;
				uint8_t *str = (uint8_t *)calloc(len, sizeof(uint8_t));
				if (str == NULL)
					return NULL;

				memcpy(str, op_name, len);
				str[len - 1] = ' ';
				bool ret3 = statements_str->add(statements_str, str, len * sizeof(uint8_t));
				if (ret3 == false)
					return NULL;
				break;
			}
		}

		// If data bytes: "[bytes] "
		default:
		{
			uint32_t data_byte_len = statements[i]->size;
			uint32_t data_str_len  = data_byte_len * 2;
			uint8_t  data_str[data_str_len + 1];

			bytearr_to_hexstr((BYTE *)(statements[i]->data), data_byte_len, (int8_t *)data_str);
			uint8_t *str = (uint8_t *)calloc(data_str_len + 3, sizeof(uint8_t));
			if (str == NULL)
				return NULL;

			str[0] = '[';
			memcpy(str+1, data_str, data_str_len);
			str[data_str_len + 1] = ']';
			str[data_str_len + 2] = ' ';

			bool ret4 = statements_str->add(statements_str, str, (data_str_len + 3) * sizeof(uint8_t));
			if (ret4 == false)
				return NULL;
			break;
		}
	}
}	free(statements);

	// Linked list to a single string.
	*size = statements_str->total_size(statements_str);
	uint8_t *string = (uint8_t *)malloc(statements_str->total_size(statements_str));
	if (string == NULL)
		return NULL;
	CLinkedlistNode **list = statements_str->forward_traversing(statements_str);
	if (list == NULL)
		return NULL;

	uint32_t pos = 0;
	for (uint32_t i = 0; i < statements_str->length; ++i)
	{
		memcpy(string+pos, list[i]->data, list[i]->size);
		pos = pos + list[i]->size;
	}
	free(list);
	delete_CLinkedlist(statements_str);
	return string;
}

BYTE * Script_to_bytes(Script *self, size_t *size)
{
	uint32_t script_len = self->script->length;
	size_t   total_size = self->script->total_size(self->script);
	*size = total_size;

	BYTE *bytes = (BYTE *)malloc(total_size);
	CLinkedlistNode **list = self->script->forward_traversing(self->script);
	if (bytes == NULL || list == NULL)
		return NULL;

	uint32_t pos = 0;
	// Script statements loop.
	for (size_t i = 0; i < script_len; ++i)
	{
		memcpy(bytes+pos, list[i]->data, list[i]->size);
		pos = pos + list[i]->size; // Shift the posistion.
	}
	free(list);
	return bytes;
}

bool Script_is_p2pkh(Script *self)
{

}

bool Script_is_p2pk(Script *self)
{

}

bool Script_is_p2sh(Script *self)
{

}

bool Script_is_multisig(Script *self)
{

}