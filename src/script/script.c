#include <stdlib.h>
#include <string.h>
#include "../common.h"
#include "../codec/codec.h"
#include "../container/CStack.h"
#include "../container/CLinkedlist.h"
#include "script.h"

opcode * new_opcode(BYTE value)
{
	opcode *new = (opcode *)malloc(sizeof(opcode));
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;
	else *new = value;
	return new;
}

void delete_opcode(opcode *op)
{
	free(op);
}

Script * new_Script()
{
	Script *new = (Script *)calloc(1, sizeof(Script));
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;

	new->script = new_CLinkedlist();
	if (new->script == MEMORY_ALLOCATE_FAILED)
	{
		free(new);
		return MEMORY_ALLOCATE_FAILED;
	}
	else
	{
		new->add_opcode    = &Script_add_opcode;
		new->add_data      = &Script_add_data;
		new->to_string     = &Script_to_string;
		new->to_bytes      = &Script_to_bytes;
		new->is_p2pkh      = &Script_is_p2pkh;
		new->is_p2pk       = &Script_is_p2pk;
		new->is_p2sh       = &Script_is_p2sh;
		new->is_multisig   = &Script_is_multisig;
		new->is_empty      = &Script_is_empty;
		new->get_length    = &Script_get_length;
		new->get_statement = &Script_get_statement;
		return new;
	}
}

Script * new_Script_from_bytes(BYTE *bytes, size_t size)
{
	Script *new = new_Script();
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;
	uint32_t pos = 0;

while (true) // Can't use break and continue to control the loop if I use switch-case.
{
// Check if opcode.
	if (IS_OPCODE(bytes[pos]))
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
			opcode *op = new_opcode(bytes[pos]);
			if (op == MEMORY_ALLOCATE_FAILED)
				return MEMORY_ALLOCATE_FAILED;
			new->add_opcode(new, op);

			// Add data.
			BYTE *data = (BYTE *)calloc(length + 1, sizeof(BYTE));
			if (data == NULL)
				return MEMORY_ALLOCATE_FAILED;
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
			opcode *op = new_opcode(bytes[pos]);
			if (op == MEMORY_ALLOCATE_FAILED)
				return MEMORY_ALLOCATE_FAILED;
			new->add_opcode(new, op);

			// Add data.
			BYTE *data = (BYTE *)calloc(length + 2, sizeof(BYTE));
			if (data == NULL)
				return MEMORY_ALLOCATE_FAILED;
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
			opcode *op = new_opcode(bytes[pos]);
			if (op == MEMORY_ALLOCATE_FAILED)
				return MEMORY_ALLOCATE_FAILED;
			new->add_opcode(new, op);

			// Add data.
			BYTE *data = (BYTE *)calloc(length + 4, sizeof(BYTE));
			if (data == NULL)
				return MEMORY_ALLOCATE_FAILED;
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
			opcode *op = new_opcode(bytes[pos]);
			if (op == MEMORY_ALLOCATE_FAILED)
				return MEMORY_ALLOCATE_FAILED;
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
		opcode *op = new_opcode(length);
		if (op == MEMORY_ALLOCATE_FAILED)
			return MEMORY_ALLOCATE_FAILED;
		new->add_opcode(new, op);

		// Add data.
		BYTE *data = (BYTE *)calloc(length, sizeof(BYTE));
		if (data == NULL)
			return MEMORY_ALLOCATE_FAILED;
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
		opcode *op = new_opcode(OP_INVALIDOPCODE);
		if (op == MEMORY_ALLOCATE_FAILED)
			return MEMORY_ALLOCATE_FAILED;
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
	if (p1->get_length(p1) == 0 || p2->get_length(p2) == 0)
		return SCRIPT_HAS_NO_STATEMENTS;

	Script *new = new_Script();
	CLinkedlistNode **p1_list = p1->script->forward_traversing(p1->script);
	CLinkedlistNode **p2_list = p2->script->forward_traversing(p2->script);
	if (new == MEMORY_ALLOCATE_FAILED || p1_list == MEMORY_ALLOCATE_FAILED || p2_list == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

	for (uint32_t i = 0; i < p1->get_length(p1); ++i)
	{
		if (p1_list[i]->size == 1)
		{
			opcode *op = new_opcode(0);
			if (op == MEMORY_ALLOCATE_FAILED)
				return MEMORY_ALLOCATE_FAILED;
			memcpy(op, p1_list[i]->data, 1);
			bool ret = new->add_opcode(new, op);
			if (ret == false)
				return SCRIPT_ADD_OPCODE_FAILED;
		}
		else
		{
			size_t data_size = p1_list[i]->size;
			BYTE *data = (BYTE *)malloc(data_size);
			memcpy(data, p1_list[i]->data, data_size);
			bool ret = new->add_data(new, data, data_size);
			if (ret == false)
				return SCRIPT_ADD_DATA_FAILED;
		}
	}

	for (uint32_t i = 0; i < p2->get_length(p2); ++i)
	{
		if (p2_list[i]->size == 1)
		{
			opcode *op = new_opcode(0);
			if (op == MEMORY_ALLOCATE_FAILED)
				return MEMORY_ALLOCATE_FAILED;
			memcpy(op, p2_list[i]->data, 1);
			bool ret = new->add_opcode(new, op);
			if (ret == false)
				return SCRIPT_ADD_OPCODE_FAILED;
		}
		else
		{
			size_t data_size = p2_list[i]->size;
			BYTE *data = (BYTE *)malloc(data_size);
			memcpy(data, p2_list[i]->data, data_size);
			bool ret = new->add_data(new, data, data_size);
			if (ret == false)
				return SCRIPT_ADD_DATA_FAILED;
		}
	}

	free(p1_list);
	free(p2_list);

	return new;
}

Script * new_Script_p2pkh(BYTE *pubkey_hash, size_t size)
{
	if (size != 20)
		return INVALID_PUBKEY_HASH_SIZE;
	Script *new = new_Script();
	opcode *dup = new_opcode(OP_DUP);
	opcode *hash160 = new_opcode(OP_HASH160);
	opcode *pushdata = new_opcode(size);
	BYTE *data = (BYTE *)malloc(size);
	opcode *equalverify = new_opcode(OP_EQUALVERIFY);
	opcode *checksig = new_opcode(OP_CHECKSIG);

	if (new == NULL || dup == NULL || hash160 == NULL || pushdata == NULL ||
		data == NULL || equalverify == NULL || checksig == NULL)
		return MEMORY_ALLOCATE_FAILED;
	memcpy(data, pubkey_hash, size);

	bool ret1 = new->add_opcode(new, dup);
	bool ret2 = new->add_opcode(new, hash160);
	bool ret3 = new->add_opcode(new, pushdata);
	bool ret4 = new->add_data(new, data, size);
	bool ret5 = new->add_opcode(new, equalverify);
	bool ret6 = new->add_opcode(new, checksig);

	if (ret1 == false || ret2 == false || ret3 == false ||
		ret4 == false || ret5 == false || ret6 == false)
		return MEMORY_ALLOCATE_FAILED;

	return NULL;
}

Script * new_Script_p2pk(BYTE *pubkey, size_t size)
{
	if (size != 65 || size != 33)
		return INVALID_PUBKEY_SIZE;
	Script *new = new_Script();
	opcode *pushdata = new_opcode(size);
	BYTE *pub = (BYTE *)malloc(size);
	opcode *checksig = new_opcode(OP_CHECKSIG);

	if (new == NULL || pushdata == NULL || pub == NULL || checksig == NULL)
		return MEMORY_ALLOCATE_FAILED;
	memcpy(pub, pubkey, size);

	bool ret1 = new->add_opcode(new, pushdata);
	bool ret2 = new->add_data(new, pub, size);
	bool ret3 = new->add_opcode(new, checksig);

	if (ret1 == false || ret2 == false || ret3 == false)
		return MEMORY_ALLOCATE_FAILED;

	return new;
}

Script * new_Script_p2sh(BYTE *hash, size_t size)
{
	Script *new = new_Script();
	opcode *hash160 = new_opcode(OP_HASH160);
	opcode *pushdata = new_opcode(size);
	BYTE *script_hash = (BYTE *)malloc(size);
	opcode *equal = new_opcode(OP_EQUAL);

	if (new == NULL || hash160 == NULL || pushdata == NULL ||
		script_hash == NULL || equal == NULL)
		return MEMORY_ALLOCATE_FAILED;
	memcpy(script_hash, hash, size);

	bool ret1 = new->add_opcode(new, hash160);
	bool ret2 = new->add_opcode(new, pushdata);
	bool ret3 = new->add_data(new, script_hash, size);
	bool ret4 = new->add_opcode(new, equal);

	if (ret1 == false || ret2 == false || ret3 == false || ret4 == false)
		return MEMORY_ALLOCATE_FAILED;

	return new;
}

Script * new_Script_multisig(uint8_t m, CLinkedlist *pubkeys)
{
	if (pubkeys->is_empty(pubkeys))
		return CLINKEDLIST_EMPTY;
	else if ( !(m > 0 && m <= pubkeys->get_length(pubkeys)) )
		return SCRIPT_MULTISIG_M_BIGGER_N;
	else if (pubkeys->get_length(pubkeys) > MAX_PUBKEYS_PER_MULTISIG)
		return SCRIPT_MULTISIG_PUBKEYS_OVER_LIMIT;

	Script *script = new_Script();
	if (script == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

	// Add first opcode: OP_M.
	opcode *OP_M = new_opcode(0x50 + m);
	if (OP_M == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;
	bool ret1 = script->add_opcode(script, OP_M);
	if (ret1 == false)
		return SCRIPT_ADD_OPCODE_FAILED;

	// Add public keys.
	CLinkedlistNode **keys = pubkeys->forward_traversing(pubkeys);
	if (keys == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;
	for (uint32_t i = 0; i < pubkeys->get_length(pubkeys); ++i)
	{
		// Add PUSHDATA().
		BYTE *PUSHDATA = (BYTE *)calloc(1, sizeof(BYTE));
		if (PUSHDATA == NULL)
			return MEMORY_ALLOCATE_FAILED;
		*PUSHDATA = keys[i]->size;
		bool ret2 = script->add_data(script, PUSHDATA, 1);
		if (ret2 == false)
			return SCRIPT_ADD_DATA_FAILED;

		// Add public key bytes.
		BYTE *pub = (BYTE *)malloc(keys[i]->size);
		if (pub == NULL)
			return MEMORY_ALLOCATE_FAILED;
		memcpy(pub, keys[i]->data, keys[i]->size);
		bool ret3 = script->add_data(script, pub, keys[i]->size);
		if (ret3 == false)
			return SCRIPT_ADD_DATA_FAILED;
	}
	free(keys);

	// Add OP_N.
	opcode *OP_N = new_opcode(0x50+(pubkeys->get_length(pubkeys)));
	if (OP_N == NULL)
		return MEMORY_ALLOCATE_FAILED;
	bool ret4 = script->add_opcode(script, OP_N);
	if (ret4 == false)
		return SCRIPT_ADD_OPCODE_FAILED;

	// Add OP_CHECKMULTISIG
	opcode *CHECKMULTISIG = new_opcode(OP_CHECKMULTISIG);
	if (CHECKMULTISIG == NULL)
		return MEMORY_ALLOCATE_FAILED;
	bool ret5 = script->add_opcode(script, CHECKMULTISIG);
	if (ret5 == false)
		return SCRIPT_ADD_OPCODE_FAILED;

	return script;
}

void delete_Script(Script *this)
{
	delete_CLinkedlist(this->script);
	free(this);
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

bool Script_add_opcode(Script *this, opcode *op)
{
	if ( (this->script->add(this->script, op, 1)) )
		return true;
	else return false;
}

bool Script_add_data(Script *this, BYTE *data, size_t size)
{
	if ( (this->script->add(this->script, data, size)) )
		return true;
	else return false;
}

uint8_t * Script_to_string(Script *this, size_t *size)
{
	if (this->is_empty(this))
		return SCRIPT_HAS_NO_STATEMENTS;

	CLinkedlistNode **statements = this->script->forward_traversing(this->script);
	CLinkedlist *statements_str = new_CLinkedlist();

	if (statements == MEMORY_ALLOCATE_FAILED || statements_str == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

// Script statements loop, convert one statement to one string and add to linked list.
for (uint32_t i = 0; i < this->get_length(this); ++i)
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
					return MEMORY_ALLOCATE_FAILED;
				str[0] = 'P'; str[1] = 'U'; str[2] = 'S'; str[3] = 'H';
				str[4] = 'D'; str[5] = 'A'; str[6] = 'T'; str[7] = 'A';
				str[8] = '(';
				str[9] = size_str[0]; str[10] = size_str[1];
				str[11] = ')';

				bool ret1 = statements_str->add(statements_str, str, 12 * sizeof(uint8_t));
				if (ret1 == false)
					return MEMORY_ALLOCATE_FAILED;
				break;
			}

			// Opcode is PUSHDATA1/2/4: "OP_PUSHDATA1" "OP_PUSHDATA2" "OP_PUSHDATA4"
			else if (op_buffer == OP_PUSHDATA1 || op_buffer == OP_PUSHDATA2 || op_buffer == OP_PUSHDATA4)
			{
				const char * op_name = get_op_name(op_buffer);
				size_t len = strlen(op_name);
				uint8_t *str = (uint8_t *)calloc(len, sizeof(uint8_t));
				if (str == NULL)
					return MEMORY_ALLOCATE_FAILED;

				memcpy(str, op_name, len);
				bool ret2 = statements_str->add(statements_str, str, len * sizeof(uint8_t));
				if (ret2 == false)
					return MEMORY_ALLOCATE_FAILED;
				break;
			}

			// Opcode is not PUSHDATA(1 ~ 75)/1/2/4: "OP_XXXXX "
			else
			{
				const char * op_name = get_op_name(op_buffer);
				size_t len = strlen(op_name) + 1;
				uint8_t *str = (uint8_t *)calloc(len, sizeof(uint8_t));
				if (str == NULL)
					return MEMORY_ALLOCATE_FAILED;

				memcpy(str, op_name, len);
				str[len - 1] = ' ';
				bool ret3 = statements_str->add(statements_str, str, len * sizeof(uint8_t));
				if (ret3 == false)
					return MEMORY_ALLOCATE_FAILED;
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
				return MEMORY_ALLOCATE_FAILED;

			str[0] = '[';
			memcpy(str+1, data_str, data_str_len);
			str[data_str_len + 1] = ']';
			str[data_str_len + 2] = ' ';

			bool ret4 = statements_str->add(statements_str, str, (data_str_len + 3) * sizeof(uint8_t));
			if (ret4 == false)
				return MEMORY_ALLOCATE_FAILED;
			break;
		}
	}
}	free(statements);

	// Linked list to a single string.
	*size = statements_str->total_size(statements_str);
	uint8_t *string = (uint8_t *)malloc(statements_str->total_size(statements_str));

	if (string == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;
	CLinkedlistNode **list = statements_str->forward_traversing(statements_str);
	if (list == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

	uint32_t pos = 0;
	for (uint32_t i = 0; i < statements_str->get_length(statements_str); ++i)
	{
		memcpy(string+pos, list[i]->data, list[i]->size);
		pos = pos + list[i]->size;
	}
	free(list);
	delete_CLinkedlist(statements_str);
	return string;
}

BYTE * Script_to_bytes(Script *this, size_t *size)
{
	if (this->is_empty(this))
		return SCRIPT_HAS_NO_STATEMENTS;

	uint32_t script_len = this->get_length(this);
	size_t   total_size = this->script->total_size(this->script);
	*size = total_size;

	BYTE *bytes = (BYTE *)malloc(total_size);
	CLinkedlistNode **list = this->script->forward_traversing(this->script);
	if (bytes == NULL || list == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

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

bool Script_is_p2pkh(Script *this)
{
	// OP_DUP + OP_HASH160 + pushdata(20) + pubkey_hash(20) + OP_EQUALVERIFY + OP_CHECKSIG
	if (this->get_length(this) != 6)
		return false;

	BYTE op_dup      = ((BYTE *)(this->script->specific_node(this->script, 0)->data))[0];
	BYTE op_hash160  = ((BYTE *)(this->script->specific_node(this->script, 1)->data))[0];
	BYTE pushdata    = ((BYTE *)(this->script->specific_node(this->script, 2)->data))[0];
	size_t data_size = this->script->specific_node(this->script, 3)->size;
	BYTE op_equalverify = ((BYTE *)(this->script->specific_node(this->script, 4)->data))[0];
	BYTE op_checksig = ((BYTE *)(this->script->specific_node(this->script, 5)->data))[0];

	if (op_dup == OP_DUP && op_hash160 == OP_HASH160 && pushdata == 0x14 &&
		data_size == 0x14 && op_equalverify == OP_EQUALVERIFY && op_checksig == OP_CHECKSIG)
		return true;

	else return false;
}

bool Script_is_p2pk(Script *this)
{
	// pushdata(65/33) + pubkey(65/33) + OP_CHECKSIG
	if (this->get_length(this) != 3)
		return false;

	BYTE pushdata = ((BYTE *)(this->script->specific_node(this->script, 0)->data))[0];
	size_t data_size = this->script->specific_node(this->script, 1)->size;
	BYTE op_checksig = ((BYTE *)(this->script->specific_node(this->script, 2)->data))[0];

	if ( (pushdata == 65 || pushdata == 33) && (data_size == 65 || data_size == 33) &&
		op_checksig == OP_CHECKSIG && pushdata == data_size)
		return true;

	else return false;
}

bool Script_is_p2sh(Script *this)
{
	// OP_HASH160 + pushdata(20) + script_hash(20) + OP_EQUAL
	if (this->get_length(this) != 4)
		return false;

	BYTE op_hash160 = ((BYTE *)(this->script->specific_node(this->script, 0)->data))[0];
	BYTE pushdata = ((BYTE *)(this->script->specific_node(this->script, 1)->data))[0];
	size_t data_size = this->script->specific_node(this->script, 2)->size;
	BYTE op_equal = ((BYTE *)(this->script->specific_node(this->script, 3)->data))[0];

	if (op_hash160 == OP_HASH160 && pushdata == 0x14 && data_size == 0x14 && op_equal == OP_EQUAL)
		return true;

	else return false;
}

bool Script_is_multisig(Script *this)
{
	// OP_N + pushdata(65/33) + pubkey1(65/33) +...+ pushdata(65/33) + pubkeyM(65/33) + OP_M + OP_CHECKMULTISIG
	uint8_t pub_num = ((opcode *)(this->script-> \
		last_node(this->script)->previous->data))[0] - 0x50;
	if (this->get_length(this) != 3 + pub_num * 2 )
		return false;

	BYTE op_n = ((BYTE *)(this->script->specific_node(this->script, 0)->data))[0];
	BYTE op_m = ((BYTE *)(this->script->last_node(this->script)->previous->data))[0];
	BYTE op_checkmultisig = ((BYTE *)(this->script->last_node(this->script)->data))[0];

	// Check op_n/op_m/op_checkmultisig.
	if (op_n > OP_0 && op_n <= OP_16 && op_m > OP_0 && op_m <= OP_16 &&
		op_n <= op_m && op_checkmultisig == OP_CHECKMULTISIG)
	{
		// If correct, check pushdatas and pubkeys.
		uint32_t i;
		for (i = 1; i <= pub_num * 2; ++i)
		{
			BYTE pushdata = ((BYTE *)(this->script->specific_node(this->script, i)->data))[0];
			size_t data_size = this->script->specific_node(this->script, i+1)->size;

			if ((pushdata == 65 || pushdata == 33) && (data_size == 65 || data_size == 33) && (pushdata == data_size))
				i++;
			else return false;
		}
		// pushdatas and pubkeys correct.
		if (i > pub_num * 2)
			return true;
		else return false;
	}
	// op_n/op_m/op_checksig incorrect.
	else return false;
}

bool Script_is_empty(Script *this)
{
	if (this->script->is_empty(this->script))
		return true;
	else return false;
}

uint32_t Script_get_length(Script *this)
{
	return this->get_length(this);
}

void * Script_get_statement(Script *this, uint64_t index, size_t *size)
{
	if (this->is_empty(this))
		return SCRIPT_HAS_NO_STATEMENTS;
	else if (this->get_length(this) <= index || index == 0)
		return INDEX_OUT_RANGE;
	else
	{
		*size = this->script->specific_node(this->script, index)->size;
		return this->script->specific_node(this->script, index)->data;
	}
}