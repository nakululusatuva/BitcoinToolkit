#include <stdlib.h>
#include <string.h>
#include "internal/common.h"
#include "internal/container/CStack.h"
#include "internal/container/CLinkedlist.h"
#include "internal/machine/script.h"

Opcode * new_Opcode(BYTE value)
{
	Opcode *new = (Opcode *)malloc(sizeof(Opcode));
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;
	else *new = value;
	return new;
}

void delete_Opcode(Opcode *op)
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
		new->add_opcode  = &Script_add_opcode;
		new->add_data    = &Script_add_data;
		new->to_string   = &Script_to_string;
		new->to_bytes    = &Script_to_bytes;
		new->is_p2pkh    = &Script_is_p2pkh;
		new->is_p2pk     = &Script_is_p2pk;
		new->is_p2sh     = &Script_is_p2sh;
		new->is_p2sh_multisig = &Script_is_p2sh_multisig;
		new->is_p2wsh    = &Script_is_p2wsh;
		new->is_p2wpkh   = &Script_is_p2wpkh;
		new->is_null_data= &Script_is_null_data;
		new->is_empty    = &Script_is_empty;
		new->get_length  = &Script_get_length;
		new->get_element = &Script_get_element;
		new->total_size  = &Script_total_size;
		new->check_element_size = &Script_check_element_size;
		return new;
	}
}

Script * new_Script_from_bytes(BYTE *bytes, size_t size)
{
	Script *new = new_Script();
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;
	uint32_t pos = 0;

while (true)
{
	if (pos == size)
		break;
	else if (BYTE_IS_NONAME_PUSHDATA(bytes[pos]))
	{
		// Check if enough bytes remain.
		uint32_t expected = bytes[pos];
		uint32_t remain = size-pos-1;
		if (remain < expected)
			return SCRIPT_REMAIN_BYTES_LESS_THAN_PUSH;

		// Add PUSHDATA
		Opcode *op = new_Opcode(expected);
		if (op == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			return MEMORY_ALLOCATE_FAILED;
		}
		if (new->add_opcode(new, op) == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			delete_Opcode(op);
			return MEMORY_ALLOCATE_FAILED;
		}

		// Add bytes.
		BYTE *data = (BYTE *)malloc(expected);
		if (data == NULL)
		{
			delete_Script(new);
			return MEMORY_ALLOCATE_FAILED;
		}
		for (uint16_t i = 0; i < expected; ++i)
		{
			data[i] = bytes[pos+1+i];
		}
		if (new->add_data(new, data, expected) == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			free(data);
			return MEMORY_ALLOCATE_FAILED;
		}

		// Shift pos.
		pos = pos + expected + 1;
		continue;
	}
	else if (BYTE_IS_124_PUSHDATA(bytes[pos]))
	{
		switch (bytes[pos])
		{
			case OP_PUSHDATA1:
			{
				// Check if enough bytes remain.
				uint32_t remain = size-pos-2;
				uint32_t expected = bytes[pos+1];
				if (remain < expected)
					return SCRIPT_REMAIN_BYTES_LESS_THAN_PUSH;

				// Add PUSHDATA1
				Opcode *op = new_Opcode(OP_PUSHDATA1);
				if (op == MEMORY_ALLOCATE_FAILED)
				{
					delete_Script(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				if (new->add_opcode(new, op) == MEMORY_ALLOCATE_FAILED)
				{
					delete_Script(new);
					delete_Opcode(op);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Add next one byte.
				BYTE *next_one_byte = (BYTE *)malloc(1);
				if (next_one_byte == NULL)
				{
					delete_Script(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				next_one_byte[0] = bytes[pos+1];
				if (new->add_data(new, next_one_byte, 1) == MEMORY_ALLOCATE_FAILED)
				{
					delete_Script(new);
					free(next_one_byte);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Add bytes.
				BYTE *data = (BYTE *)malloc(expected);
				if (data == NULL)
				{
					delete_Script(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				for (uint16_t i = 0; i < expected; ++i)
				{
					data[i] = bytes[pos+2+i];
				}
				if (new->add_data(new, data, expected) == MEMORY_ALLOCATE_FAILED)
				{
					delete_Script(new);
					free(data);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Shift pos.
				pos = pos + expected + 2;
				break;
			}
			case OP_PUSHDATA2:
			{
				// Check if enough bytes remain.
				uint32_t remain = size-pos-3;
				uint32_t expected = bytes[pos+2];
				expected = (expected << 8) + bytes[pos+1];
				if (remain < expected)
					return SCRIPT_REMAIN_BYTES_LESS_THAN_PUSH;

				// Add PUSHDATA2
				Opcode *op = new_Opcode(OP_PUSHDATA2);
				if (op == MEMORY_ALLOCATE_FAILED)
				{
					free(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				if (new->add_opcode(new, op) == MEMORY_ALLOCATE_FAILED)
				{
					free(new);
					delete_Opcode(op);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Add next two bytes.
				BYTE *next_two_bytes = (BYTE *)malloc(2);
				if (next_two_bytes == NULL)
				{
					delete_Script(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				next_two_bytes[0] = bytes[pos+1];
				next_two_bytes[1] = bytes[pos+2];
				if (new->add_data(new, next_two_bytes, 2) == MEMORY_ALLOCATE_FAILED)
				{
					delete_Script(new);
					free(next_two_bytes);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Add bytes.
				BYTE *data = (BYTE *)malloc(expected);
				if (data == NULL)
				{
					delete_Script(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				for (uint16_t i = 0; i < expected; ++i)
				{
					data[i] = bytes[pos+3+i];
				}
				if (new->add_data(new, data, expected) == MEMORY_ALLOCATE_FAILED)
				{
					delete_Script(new);
					free(data);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Shift pos.
				pos = pos + expected + 3;
				break;
			}
			case OP_PUSHDATA4:
			{
				// Check if enough bytes remain.
				uint32_t remain = size-pos-5;
				uint32_t expected = bytes[pos+4];
				expected = (expected << 8) + bytes[pos+3];
				expected = (expected << 8) + bytes[pos+2];
				expected = (expected << 8) + bytes[pos+1];
				if (remain < expected)
					return SCRIPT_REMAIN_BYTES_LESS_THAN_PUSH;

				// Add PUSHDATA4
				Opcode *op = new_Opcode(OP_PUSHDATA4);
				if (op == MEMORY_ALLOCATE_FAILED)
				{
					free(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				if (new->add_opcode(new, op) == MEMORY_ALLOCATE_FAILED)
				{
					free(new);
					delete_Opcode(op);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Add next four bytes.
				BYTE *next_four_bytes = (BYTE *)malloc(4);
				if (next_four_bytes == NULL)
				{
					delete_Script(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				next_four_bytes[0] = bytes[pos+1];
				next_four_bytes[1] = bytes[pos+2];
				next_four_bytes[2] = bytes[pos+3];
				next_four_bytes[3] = bytes[pos+4];
				if (new->add_data(new, next_four_bytes, 4) == MEMORY_ALLOCATE_FAILED)
				{
					delete_Script(new);
					free(next_four_bytes);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Add bytes.
				BYTE *data = (BYTE *)malloc(expected);
				if (data == NULL)
				{
					delete_Script(new);
					return MEMORY_ALLOCATE_FAILED;
				}
				for (uint16_t i = 0; i < expected; ++i)
				{
					data[i] = bytes[pos+5+i];
				}
				if (new->add_data(new, data, expected) == MEMORY_ALLOCATE_FAILED)
				{
					delete_Script(new);
					free(data);
					return MEMORY_ALLOCATE_FAILED;
				}

				// Shift pos.
				pos = pos + expected + 5;
				break;
			}
		}
		continue;
	}
	else if (BYTE_IS_OPCODE(bytes[pos]) &&
	        !BYTE_IS_NONAME_PUSHDATA(bytes[pos]) &&
			!BYTE_IS_124_PUSHDATA(bytes[pos]))
	{
		// Add opcode.
		Opcode *op = new_Opcode(bytes[pos]);
		if (op == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			return MEMORY_ALLOCATE_FAILED;
		}
		if (new->add_opcode(new, op) == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			delete_Opcode(op);
			return MEMORY_ALLOCATE_FAILED;
		}

		// Shift pos.
		pos = pos + 1;
		continue;
	}
	else if (!BYTE_IS_OPCODE(bytes[pos]))
	{
		// Add OP_INVALIDOPCODE.
		Opcode *op = new_Opcode(OP_INVALIDOPCODE);
		if (op == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			return MEMORY_ALLOCATE_FAILED;
		}
		if (new->add_opcode(new, op) == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			delete_Opcode(op);
			return MEMORY_ALLOCATE_FAILED;
		}

		// Shift pos.
		pos = pos + 1;
		continue;
	}
}
	return new;
}

Script * new_Script_assembled(Script *p1, Script *p2)
{
	if (p1->get_length(p1) == 0 || p2->get_length(p2) == 0)
		return SCRIPT_HAS_NO_ELEMENTS;
	else if (p1 == NULL || p2 == NULL)
		return PASSING_NULL_POINTER;

	Script *new = new_Script();
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;

	CLinkedlistNode **p1_list = p1->script->forward_iter(p1->script);
	if (p1_list == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		return MEMORY_ALLOCATE_FAILED;
	}

	CLinkedlistNode **p2_list = p2->script->forward_iter(p2->script);
	if (p2_list == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		free(p1_list);
		return MEMORY_ALLOCATE_FAILED;
	}

	for (uint32_t i = 0; i < p1->get_length(p1); ++i)
	{
		size_t size = p1_list[i]->size;
		void *data = (void *)malloc(size);
		if (data == NULL)
		{
			delete_Script(new);
			free(p1_list);
			free(p2_list);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(data, p1_list[i]->data, size);
		if (new->script->add(new->script, data, size, BYTE_TYPE) == false)
		{
			delete_Script(new);
			free(p1_list);
			free(p2_list);
			free(data);
			return MEMORY_ALLOCATE_FAILED;
		}
	}

	for (uint32_t i = 0; i < p2->get_length(p2); ++i)
	{
		size_t size = p2_list[i]->size;
		void *data = (void *)malloc(size);
		if (data == NULL)
		{
			delete_Script(new);
			free(p1_list);
			free(p2_list);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(data, p2_list[i]->data, size);
		if (new->script->add(new->script, data, size, BYTE_TYPE) == false)
		{
			delete_Script(new);
			free(p1_list);
			free(p2_list);
			free(data);
			return MEMORY_ALLOCATE_FAILED;
		}
	}

	free(p1_list);
	free(p2_list);

	return new;
}

Script * new_Script_p2pkh(BYTE *pubkey_hash, size_t size)
{
	if (size != 20)
		return INVALID_RIPEMD160_SIZE;
	else if (pubkey_hash == NULL)
		return PASSING_NULL_POINTER;

	Script *new = new_Script();
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;

	Opcode *op_dup = new_Opcode(OP_DUP);
	if (op_dup == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		return MEMORY_ALLOCATE_FAILED;
	}

	Opcode *op_hash160 = new_Opcode(OP_HASH160);
	if (op_hash160 == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_dup);
		return MEMORY_ALLOCATE_FAILED;
	}

	Opcode *op_pushdata = new_Opcode(size);
	if (op_pushdata == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_dup);
		delete_Opcode(op_hash160);
		return MEMORY_ALLOCATE_FAILED;
	}

	BYTE *data = (BYTE *)malloc(size);
	if (data == NULL)
	{
		delete_Script(new);
		delete_Opcode(op_dup);
		delete_Opcode(op_hash160);
		delete_Opcode(op_pushdata);
		return MEMORY_ALLOCATE_FAILED;
	}

	Opcode *op_equalverify = new_Opcode(OP_EQUALVERIFY);
	if (op_equalverify == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_dup);
		delete_Opcode(op_hash160);
		delete_Opcode(op_pushdata);
		free(data);
		return MEMORY_ALLOCATE_FAILED;
	}

	Opcode *op_checksig = new_Opcode(OP_CHECKSIG);
	if (op_checksig == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_dup);
		delete_Opcode(op_hash160);
		delete_Opcode(op_pushdata);
		free(data);
		delete_Opcode(op_equalverify);
		return MEMORY_ALLOCATE_FAILED;
	}

	memcpy(data, pubkey_hash, size);

	if (new->add_opcode(new, op_dup) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_dup);
		delete_Opcode(op_hash160);
		delete_Opcode(op_pushdata);
		free(data);
		delete_Opcode(op_equalverify);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_hash160) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_hash160);
		delete_Opcode(op_pushdata);
		free(data);
		delete_Opcode(op_equalverify);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_pushdata) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_pushdata);
		free(data);
		delete_Opcode(op_equalverify);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_data(new, data, size) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		free(data);
		delete_Opcode(op_equalverify);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_equalverify) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_equalverify);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_checksig) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}

	return NULL;
}

Script * new_Script_p2pk(BYTE *pubkey, size_t size)
{
	if (size != 65 || size != 33)
		return INVALID_PUBKEY_SIZE;
	else if (pubkey == NULL)
		return PASSING_NULL_POINTER;

	Script *new = new_Script();
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;

	Opcode *op_pushdata = new_Opcode(size);
	if (op_pushdata == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		return MEMORY_ALLOCATE_FAILED;
	}

	BYTE *pub = (BYTE *)malloc(size);
	if (pub == NULL)
	{
		delete_Script(new);
		delete_Opcode(op_pushdata);
		return MEMORY_ALLOCATE_FAILED;
	}

	Opcode *op_checksig = new_Opcode(OP_CHECKSIG);
	if (op_checksig == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_pushdata);
		free(pub);
		return MEMORY_ALLOCATE_FAILED;
	}

	memcpy(pub, pubkey, size);

	if (new->add_opcode(new, op_pushdata) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_pushdata);
		free(pub);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_data(new, pub, size) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		free(pub);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_checksig) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_checksig);
		return MEMORY_ALLOCATE_FAILED;
	}

	return new;
}

Script * new_Script_p2sh(BYTE *hash, size_t size)
{
	if (size != 20)
		return INVALID_RIPEMD160_SIZE;
	else if (hash == NULL)
		return PASSING_NULL_POINTER;

	Script *new = new_Script();
	if (new == NULL)
		return MEMORY_ALLOCATE_FAILED;

	Opcode *op_hash160 = new_Opcode(OP_HASH160);
	if (op_hash160 == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		return MEMORY_ALLOCATE_FAILED;
	}

	Opcode *op_pushdata = new_Opcode(size);
	if (op_pushdata == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_hash160);
		return MEMORY_ALLOCATE_FAILED;
	}

	BYTE *script_hash = (BYTE *)malloc(size);
	if (script_hash == NULL)
	{
		delete_Script(new);
		delete_Opcode(op_hash160);
		delete_Opcode(op_pushdata);
		return MEMORY_ALLOCATE_FAILED;
	}

	Opcode *op_equal = new_Opcode(OP_EQUAL);
	if (op_equal == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_hash160);
		delete_Opcode(op_pushdata);
		free(script_hash);
		return MEMORY_ALLOCATE_FAILED;
	}

	memcpy(script_hash, hash, size);

	if (new->add_opcode(new, op_hash160) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_hash160);
		delete_Opcode(op_pushdata);
		free(script_hash);
		delete_Opcode(op_equal);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_pushdata) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_pushdata);
		free(script_hash);
		delete_Opcode(op_equal);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_data(new, script_hash, size) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		free(script_hash);
		delete_Opcode(op_equal);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_equal) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_equal);
		return MEMORY_ALLOCATE_FAILED;
	}

	return new;
}

Script * new_Script_p2sh_multisig(uint8_t m, CLinkedlist *pubkeys)
{
	if (pubkeys == NULL)
		return PASSING_NULL_POINTER;
	uint8_t n = pubkeys->get_length(pubkeys);
	if (pubkeys->is_empty(pubkeys))
		return CLINKEDLIST_EMPTY;
	else if ( !(m > 0 && m <= n) )
		return SCRIPT_MULTISIG_M_BIGGER_N;
	else if (pubkeys->total_size(pubkeys) >= 517)
		return SCRIPT_ELEMENT_SIZE_OVERLIMIT;

	Script *new = new_Script();
	if (new == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

	// Add first Opcode: op_m.
	Opcode *op_m = new_Opcode(0x50 + m);
	if (op_m == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_m) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_m);
		return MEMORY_ALLOCATE_FAILED;
	}

	// Add public keys.
	CLinkedlistNode **keys = pubkeys->forward_iter(pubkeys);
	if (keys == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		return MEMORY_ALLOCATE_FAILED;
	}
	for (uint32_t i = 0; i < n; ++i)
	{
		// Add PUSHDATA().
		BYTE *op_pushdata = (BYTE *)calloc(1, sizeof(BYTE));
		if (op_pushdata == NULL)
		{
			delete_Script(new);
			free(keys);
			return MEMORY_ALLOCATE_FAILED;
		}
		*op_pushdata = keys[i]->size;
		if (new->add_data(new, op_pushdata, 1) == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			free(keys);
			free(op_pushdata);
			return MEMORY_ALLOCATE_FAILED;
		}

		// Add public key bytes.
		BYTE *pub = (BYTE *)malloc(keys[i]->size);
		if (pub == NULL)
		{
			delete_Script(new);
			free(keys);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(pub, keys[i]->data, keys[i]->size);
		if (new->add_data(new, pub, keys[i]->size) == MEMORY_ALLOCATE_FAILED)
		{
			delete_Script(new);
			free(keys);
			free(pub);
			return MEMORY_ALLOCATE_FAILED;
		}
	}
	free(keys);

	// Add op_n.
	Opcode *op_n = new_Opcode(0x50+n);
	if (op_n == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_n) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_n);
		return MEMORY_ALLOCATE_FAILED;
	}

	// Add OP_CHECKMULTISIG
	Opcode *op_checkmultisig = new_Opcode(OP_CHECKMULTISIG);
	if (op_checkmultisig == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		return MEMORY_ALLOCATE_FAILED;
	}
	if (new->add_opcode(new, op_checkmultisig) == MEMORY_ALLOCATE_FAILED)
	{
		delete_Script(new);
		delete_Opcode(op_checkmultisig);
		return MEMORY_ALLOCATE_FAILED;
	}

	return new;
}

Script * new_Script_p2wsh(BYTE ver, BYTE *sha256, size_t size)
{
	/*
	if (size != 32)
		return INVALID_SHA256_SIZE;
	else if (sha256 == NULL)
		return PASSING_NULL_POINTER;
	*/
	return NULL;
}

Script * new_Script_p2wpkh(BYTE ver, BYTE *hash160, size_t size)
{
	/*
	if (size != 20)
		return INVALID_SHA256_SIZE;
	else if (hash160 == NULL)
		return PASSING_NULL_POINTER;
	*/
	return NULL;
}

Script * new_Script_null_data(BYTE *data, size_t size)
{
	/*
	if (data == NULL)
		return PASSING_NULL_POINTER;
	*/
	return NULL;
}

void delete_Script(Script *this)
{
	delete_CLinkedlist(this->script);
	free(this);
}

const char * get_op_name(Opcode op)
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

Status Script_add_opcode(Script *this, Opcode *op)
{
	if (this == NULL || op == NULL)
		return PASSING_NULL_POINTER;
	else if ( (this->script->add(this->script, op, 1, BYTE_TYPE)) == SUCCEEDED )
		return SUCCEEDED;
	else return MEMORY_ALLOCATE_FAILED;
}

Status Script_add_data(Script *this, BYTE *data, size_t size)
{
	if (this == NULL || data == NULL)
		return PASSING_NULL_POINTER;
	else if ( (this->script->add(this->script, data, size, BYTE_TYPE)) == SUCCEEDED )
		return SUCCEEDED;
	else return MEMORY_ALLOCATE_FAILED;
}

uint8_t * Script_to_string(Script *this, size_t *size)
{
	if (this == NULL || size == NULL)
		return PASSING_NULL_POINTER;
	else if (this->is_empty(this))
		return SCRIPT_HAS_NO_ELEMENTS;

	CLinkedlistNode **elements = this->script->forward_iter(this->script);
	if (elements == MEMORY_ALLOCATE_FAILED)
		return MEMORY_ALLOCATE_FAILED;

	CLinkedlist *elements_str = new_CLinkedlist();
	if (elements_str == MEMORY_ALLOCATE_FAILED)
	{
		free(elements);
		return MEMORY_ALLOCATE_FAILED;
	}

// Script elements loop, convert one element to one string and add to linked list.
// Data bytes element will be processed and skip.
// Data bytes element which doesn't have a PUSHDATA element before will cause and return an error code.
for (uint32_t i = 0; i < this->get_length(this); ++i)
{
	BYTE buffer = ((BYTE *)(elements[i]->data))[0];
	size_t buffer_size = elements[i]->size;

	if (buffer_size == 1 && BYTE_IS_NONAME_PUSHDATA(buffer))
	{	// PUSHDATA BYTE
		uint8_t size_str[3];
		BYTE  size_byte[1];
		size_byte[0] = buffer;
		bytearr_to_hexstr(size_byte, 1, size_str);

		uint8_t *PUSHDATA = (uint8_t *)calloc(14, sizeof(uint8_t));
		if (PUSHDATA == NULL)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return MEMORY_ALLOCATE_FAILED;
		}
		PUSHDATA[0] = 'P'; PUSHDATA[1] = 'U'; PUSHDATA[2] = 'S'; PUSHDATA[3] = 'H';
		PUSHDATA[4] = 'D'; PUSHDATA[5] = 'A'; PUSHDATA[6] = 'T'; PUSHDATA[7] = 'A';
		PUSHDATA[8] = '('; PUSHDATA[9] = '0'; PUSHDATA[10] = 'x';
		PUSHDATA[11] = size_str[0]; PUSHDATA[12] = size_str[1]; PUSHDATA[13] = ')';

		if (elements_str->add(elements_str, PUSHDATA, 14 * sizeof(uint8_t), BYTE_TYPE) == false)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			free(PUSHDATA);
			return MEMORY_ALLOCATE_FAILED;
		}

		// DATA BYTES
		uint32_t expected = elements[i+1]->size;
		uint32_t str_len  = expected * 2 + 3;
		uint8_t  str[str_len];
		if (expected != buffer)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED;
		}
		else if (expected > MAX_SCRIPT_ELEMENT_SIZE)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return SCRIPT_ELEMENT_SIZE_OVERLIMIT;
		}

		bytearr_to_hexstr((BYTE *)(elements[i+1]->data), expected, str);
		uint8_t *bracketed = (uint8_t *)calloc(str_len, sizeof(uint8_t));
		if (bracketed == NULL)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return MEMORY_ALLOCATE_FAILED;
		}
		bracketed[0] = '[';
		memcpy(bracketed+1, str, str_len-3);
		bracketed[str_len - 2] = ']';
		bracketed[str_len - 1] = ' ';
		if (elements_str->add(elements_str, bracketed, str_len * sizeof(uint8_t), BYTE_TYPE) == false)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			free(bracketed);
			return MEMORY_ALLOCATE_FAILED;
		}
		i+=1;
	}
	else if (buffer_size == 1 && BYTE_IS_124_PUSHDATA(buffer))
	{	// PUSHDATA
		const char * op_name = get_op_name(buffer);
		size_t opname_len = strlen(op_name);
		uint8_t *PUSHDATA = (uint8_t *)calloc(opname_len, sizeof(uint8_t));
		if (PUSHDATA == NULL)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(PUSHDATA, op_name, opname_len);
		if (elements_str->add(elements_str, PUSHDATA, opname_len * sizeof(uint8_t), BYTE_TYPE) == false)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			free(PUSHDATA);
			return MEMORY_ALLOCATE_FAILED;
		}

		// DATA BYTES
		size_t  expected = 0;
		size_t  str_len  = 0;
		uint8_t *str     = NULL;
		switch (buffer)
		{
			case OP_PUSHDATA1:
			{
				expected = ((BYTE *)(elements[i+1]->data))[0];
				str_len = expected * 2 + 3;
				str = (uint8_t *)malloc(str_len);
				break;
			}
			case OP_PUSHDATA2:
			{
				expected = ((BYTE *)(elements[i+1]->data))[1];
				expected = (expected << 8) + ((BYTE *)(elements[i+1]->data))[0];
				str_len = expected * 2 + 3;
				str = (uint8_t *)malloc(str_len);
				break;
			}
			case OP_PUSHDATA4:
			{
				expected = ((BYTE *)(elements[i+1]->data))[3];
				expected = (expected << 8) + ((BYTE *)(elements[i+1]->data))[2];
				expected = (expected << 8) + ((BYTE *)(elements[i+1]->data))[1];
				expected = (expected << 8) + ((BYTE *)(elements[i+1]->data))[0];
				str_len = expected * 2 + 3;
				str = (uint8_t *)malloc(str_len * sizeof(uint8_t));
				break;
			}
		}

		if (expected != elements[i+2]->size)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED;
		}
		if (str == NULL)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return MEMORY_ALLOCATE_FAILED;
		}

		bytearr_to_hexstr((BYTE *)(elements[i+2]->data), expected, str);
		uint8_t *bracketed = (uint8_t *)calloc(str_len, sizeof(uint8_t));
		if (bracketed == NULL)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return MEMORY_ALLOCATE_FAILED;
		}

		bracketed[0] = '[';
		memcpy(bracketed+1, str, str_len-3);
		bracketed[str_len - 2] = ']';
		bracketed[str_len - 1] = ' ';
		if (elements_str->add(elements_str, bracketed, str_len * sizeof(uint8_t), BYTE_TYPE) == false)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			free(bracketed);
			return MEMORY_ALLOCATE_FAILED;
		}
		i+=2;
	}
	else if (buffer_size == 1 && BYTE_IS_OPCODE(buffer) &&
	        !BYTE_IS_124_PUSHDATA(buffer) && !BYTE_IS_NONAME_PUSHDATA(buffer))
	{	// NON-PUSH OPCODE
		const char * op_name = get_op_name(buffer);
		size_t len = strlen(op_name) + 1;
		uint8_t *OPCODE = (uint8_t *)calloc(len, sizeof(uint8_t));
		if (OPCODE == NULL)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(OPCODE, op_name, len);
		OPCODE[len - 1] = ' ';
		if (elements_str->add(elements_str, OPCODE, len * sizeof(uint8_t), BYTE_TYPE) == false)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			free(OPCODE);
			return MEMORY_ALLOCATE_FAILED;
		}
	}
	else if (buffer_size == 1 && !BYTE_IS_OPCODE(buffer))
	{
		const char * op_name = get_op_name(OP_INVALIDOPCODE);
		size_t len = strlen(op_name) + 1;
		uint8_t *OPCODE = (uint8_t *)calloc(len, sizeof(uint8_t));
		if (OPCODE == NULL)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(OPCODE, op_name, len);
		OPCODE[len - 1] = ' ';
		if (elements_str->add(elements_str, OPCODE, len * sizeof(uint8_t), BYTE_TYPE) == false)
		{
			free(elements);
			delete_CLinkedlist(elements_str);
			free(OPCODE);
			return MEMORY_ALLOCATE_FAILED;
		}
	}
	else if (buffer_size > 1)
	{
		free(elements);
		delete_CLinkedlist(elements_str);
		return SCRIPT_CONTAINED_INVALID_ELEMENT;
	}
}
	// Linked list to a single string.
	*size = elements_str->total_size(elements_str);
	uint8_t *string = (uint8_t *)malloc(elements_str->total_size(elements_str));
	if (string == NULL)
	{
		free(elements);
		delete_CLinkedlist(elements_str);
		return MEMORY_ALLOCATE_FAILED;
	}
	CLinkedlistNode **list = elements_str->forward_iter(elements_str);
	if (list == MEMORY_ALLOCATE_FAILED)
	{
		free(elements);
		delete_CLinkedlist(elements_str);
		return MEMORY_ALLOCATE_FAILED;
	}
	uint32_t pos = 0;
	for (uint32_t i = 0; i < elements_str->get_length(elements_str); ++i)
	{
		memcpy(string+pos, list[i]->data, list[i]->size);
		pos = pos + list[i]->size;
	}
	free(list);
	free(elements);
	delete_CLinkedlist(elements_str);
	return string;
}

BYTE * Script_to_bytes(Script *this, size_t *size)
{
	if (this == NULL || size == NULL)
		return PASSING_NULL_POINTER;
	else if (this->is_empty(this))
		return SCRIPT_HAS_NO_ELEMENTS;

	uint32_t script_len = this->get_length(this);
	size_t   total_size = this->total_size(this);
	size[0] = total_size;

	BYTE *bytes = (BYTE *)malloc(total_size);
	if (bytes == NULL)
		return MEMORY_ALLOCATE_FAILED;
	CLinkedlistNode **list = this->script->forward_iter(this->script);
	if (list == NULL)
	{
		free(bytes);
		return MEMORY_ALLOCATE_FAILED;
	}

	// Script elements loop.
	uint32_t pos = 0;
	for (size_t i = 0; i < script_len; ++i)
	{
		memcpy(bytes+pos, list[i]->data, list[i]->size);
		pos = pos + list[i]->size; // Shift the posistion.
	}
	free(list);
	return bytes;
}

Status Script_is_p2pkh(Script *this)
{
	// OP_DUP + OP_HASH160 + pushdata(20) + pubkey_hash(20) + OP_EQUALVERIFY + OP_CHECKSIG
	if (this == NULL)
		return PASSING_NULL_POINTER;
	else if (this->get_length(this) != 6)
		return FAILED;

	BYTE op_dup      = ((BYTE *)(this->script->get_node(this->script, 0)->data))[0];
	BYTE op_hash160  = ((BYTE *)(this->script->get_node(this->script, 1)->data))[0];
	BYTE pushdata    = ((BYTE *)(this->script->get_node(this->script, 2)->data))[0];
	size_t data_size = this->script->get_node(this->script, 3)->size;
	BYTE op_equalverify = ((BYTE *)(this->script->get_node(this->script, 4)->data))[0];
	BYTE op_checksig = ((BYTE *)(this->script->get_node(this->script, 5)->data))[0];

	if (pushdata != 0x14 || data_size != 0x14)
		return INVALID_RIPEMD160_SIZE;

	else if (pushdata != data_size)
		return SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED;

	else if (op_dup == OP_DUP && op_hash160 == OP_HASH160 && op_equalverify == OP_EQUALVERIFY && op_checksig == OP_CHECKSIG)
		return SUCCEEDED;

	else return FAILED;
}

Status Script_is_p2pk(Script *this)
{
	// pushdata(65/33) + pubkey(65/33) + OP_CHECKSIG
	if (this == NULL)
		return PASSING_NULL_POINTER;
	else if (this->get_length(this) != 3)
		return FAILED;

	BYTE pushdata = ((BYTE *)(this->script->get_node(this->script, 0)->data))[0];
	size_t data_size = this->script->get_node(this->script, 1)->size;
	BYTE op_checksig = ((BYTE *)(this->script->get_node(this->script, 2)->data))[0];

	if (pushdata != 65 || pushdata != 33 || data_size != 65 || data_size != 33)
		return INVALID_PUBKEY_SIZE;

	else if (pushdata != data_size)
		return SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED;

	else if (op_checksig == OP_CHECKSIG && pushdata == data_size)
		return SUCCEEDED;

	else return FAILED;
}

Status Script_is_p2sh(Script *this)
{
	// OP_HASH160 + pushdata(20) + script_hash(20) + OP_EQUAL
	if (this == NULL)
		return PASSING_NULL_POINTER;
	else if (this->get_length(this) != 4)
		return FAILED;

	BYTE op_hash160 = ((BYTE *)(this->script->get_node(this->script, 0)->data))[0];
	BYTE pushdata = ((BYTE *)(this->script->get_node(this->script, 1)->data))[0];
	size_t data_size = this->script->get_node(this->script, 2)->size;
	BYTE op_equal = ((BYTE *)(this->script->get_node(this->script, 3)->data))[0];

	if (pushdata != 0x14 || data_size != 0x14)
		return INVALID_RIPEMD160_SIZE;

	else if (pushdata != data_size)
		return SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED;

	else if (op_hash160 == OP_HASH160 && op_equal == OP_EQUAL)
		return SUCCEEDED;

	else return FAILED;
}

Status Script_is_p2sh_multisig(Script *this)
{
	// OP_N + pushdata(65/33) + pubkey1(65/33) +...+ pushdata(65/33) + pubkeyM(65/33) + OP_M + OP_CHECKMULTISIG
	if (this == NULL)
		return PASSING_NULL_POINTER;
	uint8_t n = ((Opcode *)(this->script->last_node(this->script)->previous->data))[0] - 0x50;
	if (this->get_length(this) != 3 + n * 2 )
		return FAILED;

	BYTE op_n = ((BYTE *)(this->script->get_node(this->script, 0)->data))[0];
	BYTE op_m = ((BYTE *)(this->script->last_node(this->script)->previous->data))[0];
	BYTE op_checkmultisig = ((BYTE *)(this->script->last_node(this->script)->data))[0];

	// Check op_n/op_m/op_checkmultisig.
	if (op_n > OP_0 && op_n < OP_16 && op_m > OP_0 && op_m < OP_16 &&
		op_n <= op_m && op_checkmultisig == OP_CHECKMULTISIG)
	{
		// If correct, check pushdatas and pubkeys.
		uint32_t i;
		for (i = 1; i <= n * 2; ++i)
		{
			BYTE pushdata = ((BYTE *)(this->script->get_node(this->script, i)->data))[0];
			size_t data_size = this->script->get_node(this->script, i+1)->size;

			if (pushdata != 65 || pushdata != 33 || data_size != 65 || data_size != 33)
				return INVALID_PUBKEY_SIZE;

			else if (pushdata != data_size)
				return SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED;

			else i++;
		}
		// pushdatas and pubkeys correct.
		if (i > n * 2)
			return SUCCEEDED;
		else return FAILED;
	}
	// op_n/op_m/op_checksig incorrect.
	else return FAILED;
}

Status Script_is_p2wsh(Script *this)
{
	/*
	// ver + <32 bytes sha256 of redeem script>
	if (this == NULL)
		return PASSING_NULL_POINTER;
	*/
	return NULL;
}

Status Script_is_p2wpkh(Script *this)
{
	/*
	// ver + <20 bytes hash160 of pubkey>
	if (this == NULL)
		return PASSING_NULL_POINTER;
	*/
	return NULL;
}

Status Script_is_null_data(Script *this)
{
	/*
	// OP_RETURN + <0 to 40 bytes data>
	if (this == NULL)
		return PASSING_NULL_POINTER;
	*/
	return NULL;
}

bool Script_is_empty(Script *this)
{
	if (this->script->is_empty(this->script))
		return true;
	else return false;
}

uint64_t Script_get_length(Script *this)
{
	if (this == NULL)
		return (uint64_t)PASSING_NULL_POINTER;
	return this->script->get_length(this->script);
}

Status Script_get_element(Script *this, uint64_t index, size_t *size)
{
	if (this == NULL)
		return PASSING_NULL_POINTER;
	else if (this->is_empty(this))
		return SCRIPT_HAS_NO_ELEMENTS;
	else if (this->get_length(this) <= index || index == 0)
		return INDEX_OUT_RANGE;
	else
	{
		size_t size_buff = this->script->get_node(this->script, index)->size;
		if (size != NULL) size[0] = size_buff;
		void *element_copy = (void *)malloc(size_buff);
		if (element_copy == NULL)
			return MEMORY_ALLOCATE_FAILED;
		memcpy(element_copy, this->script->get_node(this->script, index)->data, *size);
		return element_copy;
	}
}

size_t Script_total_size(Script *this)
{
	if (this == NULL)
		return (size_t)PASSING_NULL_POINTER;
	else if (this->is_empty(this))
		return (size_t)SCRIPT_HAS_NO_ELEMENTS;

	return this->script->total_size(this->script);
}

uint64_t Script_check_element_size(Script *this)
{
	if (this->is_empty(this))
		return (uint64_t)SCRIPT_HAS_NO_ELEMENTS;

	uint32_t i;
	for (i = 0; i < this->get_length(this); ++i)
	{
		size_t element_size = 0;
		void *element = (this->get_element(this, i, &element_size));
		free(element);
		if (element_size > 520)
			return i;
	}

	return (uint64_t)FAILED;
}

bool Script_is_standard(Script *this)
{
	if (this->is_p2pkh(this) == SUCCEEDED)
		return true;
	else if (this->is_p2pk(this) == SUCCEEDED)
		return true;
	else if (this->is_p2sh(this) == SUCCEEDED)
		return true;
	else if (this->is_p2sh_multisig(this) == SUCCEEDED)
		return true;
	/*
	else if (this->is_p2wsh(this) == SUCCEEDED)
		return true;
	else if (this->is_p2wpkh(this) == SUCCEEDED)
		return true;
	else if (this->is_null_data(this) == SUCCEEDED)
		return true;
	*/
	else return false;
}