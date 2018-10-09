#include <stdlib.h>
#include <string.h>
#include <openssl/bn.h>
#include "script.h"
#include "operation.h"
#include "interpreter.h"
#include "../status.h"
#include "../codec/codec.h"
#include "../container/CStack.h"

Status EXC_OP_0_FALSE(CStack *stack)
{
	if (stack->is_full) return CSTACK_FULL;
	BYTE *num = NULL;
	stack->push(stack, num, 0, BYTE_TYPE);
	return OPERATION_EXECUTED;
}

Status EXC_OP_1_TRUE(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	BYTE *num = (BYTE *)malloc(1);
	if (num == NULL) return MEMORY_ALLOCATE_FAILED;
	num[0] = 0x01;
	stack->push(stack, num, 1, BYTE_TYPE);
	return OPERATION_EXECUTED;
}

Status EXC_OP_PUSHDATA(CStack *stack, Script *script, uint64_t *pos)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	// Get the expected size (current element).
	size_t expected = ((BYTE *)script->get_element(script, *pos, NULL))[0];

	// Get the actual size and check (next element).
	size_t size;
	BYTE *data = (BYTE *)(script->get_element(script, (*pos)+1, &size));
	if (expected != size) return SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED;

	// Copy the data.
	BYTE *copy = (BYTE *)malloc(size);
	if (copy == NULL) return MEMORY_ALLOCATE_FAILED;
	memcpy(copy, data, size);

	// Push to stack.
	stack->push(stack, copy, size, BYTE_TYPE);
	*pos = (*pos) + 2;
	return OPERATION_EXECUTED;
}

Status EXC_OP_PUSHDATAN(CStack *stack, Script *script, uint64_t *pos)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	// Check if data size equal to expected.
	size_t expected = 0;
	size_t next_size = 0;
	BYTE *next_element = (BYTE *)script->get_element(script, (*pos)+1, &next_size);

	// Get the expected size (next element).
	switch (((BYTE *)script->get_element(script, *pos, NULL))[0])
	{
		case OP_PUSHDATA1:
		{
			if (next_size != 1) return SCRIPT_CONTAINED_INVALID_ELEMENT;
			expected = next_element[0];
			break;
		}
		case OP_PUSHDATA2:
		{
			if (next_size != 2) return SCRIPT_CONTAINED_INVALID_ELEMENT;
			expected = next_element[1];
			expected = (expected << 8) + next_element[0];
			break;
		}
		case OP_PUSHDATA4:
		{
			if (next_size != 4) return SCRIPT_CONTAINED_INVALID_ELEMENT;
			expected = next_element[3];
			expected = (expected << 8) + next_element[2];
			expected = (expected << 8) + next_element[1];
			expected = (expected << 8) + next_element[0];
			break;
		}
	}

	// Get the actual size and check (next next element).
	size_t size;
	BYTE *data = (BYTE *)script->get_element(script, (*pos)+2, &size);
	if (expected != size) return SCRIPT_SIZE_TO_PUSH_NOT_EQUAL_EXPECTED;

	// Copy the data.
	BYTE *copy = (BYTE *)malloc(size);
	if (copy == NULL) return MEMORY_ALLOCATE_FAILED;
	memcpy(copy, data, size);

	// Push to stack.
	stack->push(stack, data, size, BYTE_TYPE);
	*pos = (*pos) + 3;
	return OPERATION_EXECUTED;
}

Status EXC_OP_1NEGATE(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	BYTE *num = (BYTE *)malloc(1);
	if (num == NULL) return MEMORY_ALLOCATE_FAILED;
	num[0] = 0x81;
	stack->push(stack, num, 1, BYTE_TYPE);
	return OPERATION_EXECUTED;
}

Status EXC_OP_2_TO_16(CStack *stack, BYTE number)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	BYTE *num = (BYTE *)malloc(1);
	if (num == NULL) return MEMORY_ALLOCATE_FAILED;
	num[0] = number;
	stack->push(stack, num, 1, BYTE_TYPE);
	return OPERATION_EXECUTED;
}

Status EXC_OP_NOP()
{
	return OPERATION_EXECUTED;
}

Status EXC_OP_IF(CStack *stack, Script *script, uint64_t *pos)
{
	// Get the statements length.
	uint64_t length = 0;
	for (uint64_t i = (*pos)+1; i < script->get_length(script); ++i)
	{
		size_t size = 0;
		BYTE *element = (BYTE *)script->get_element(script, i, &size);
		if (size == 1 && (element[0] == OP_IF || element[0] == OP_ELSE || \
		    element[0] == OP_NOTIF || element[0] == OP_ENDIF))
			break;
		else length += 1;
	}

	// If the top stack value is not False, execute the statments.
	size_t top_size = 0;
	BYTE *top = stack->pop(stack, &top_size, NULL);
	if (top_size == 0 && top == NULL)
	{	// Stack top element is NULL.
		*pos = (*pos) + length;
		return OPERATION_NOT_EXECUTED;
	}
	else if (top_size == 1 && top[0] != 0x00)
	{	// Stack top element is zero.
		free(top);
		return OPERATION_EXECUTED;
	}
	else
	{	// Stack top element more than one byte.
		uint32_t i;
		for (i = 0; i < top_size; ++i)
		{
			if (top[i] != 0x00)
			{
				free(top);
				return OPERATION_EXECUTED;
			}
		}
		*pos = (*pos) + length;
		free(top);
		return OPERATION_NOT_EXECUTED;
	}
}

Status EXC_OP_NOTIF(CStack *stack, Script *script, uint64_t *pos)
{
	// Get the statements length.
	uint64_t length = 0;
	for (uint64_t i = (*pos)+1; i < script->get_length(script); ++i)
	{
		size_t size = 0;
		BYTE *element = (BYTE *)script->get_element(script, i, &size);
		if (size == 1 && (element[0] == OP_IF || element[0] == OP_ELSE || \
		    element[0] == OP_NOTIF || element[0] == OP_ENDIF))
			break;
		else length += 1;
	}

	// If the top stack value is False, execute the statments.
	size_t top_size = 0;
	BYTE *top = stack->pop(stack, &top_size, NULL);
	if (top_size == 0 && top == NULL)
	{	// Stack top element is NULL.
		return OPERATION_EXECUTED;
	}
	else if (top_size == 1 && top[0] == 0x00)
	{	// Stack top element only one byte.
		free(top);
		return OPERATION_EXECUTED;
	}
	else
	{	// Stack top element more than one byte.
		uint32_t i;
		for (i = 0; i < top_size; ++i)
		{
			if (top[i] != 0x00)
			{
				*pos = (*pos) + length;
				return OPERATION_NOT_EXECUTED;
			}
		}
		free(top);
		return OPERATION_EXECUTED;
	}
}

Status EXC_OP_ELSE(CStack *stack, Script *script, uint64_t *pos, void *previous_status)
{
	// Check if without OP_IF / OP_NOTIF / OP_ELSE.
	int64_t i;
	for (i = (*pos)-1; i >= 0; --i)
	{
		size_t size;
		BYTE *element = script->get_element(script, i, &size);
		if (size != 1) continue;
		else if (size == 1 && (element[0] == OP_IF || element[0] == OP_ELSE || \
		         element[0] == OP_NOTIF || element[0] == OP_ENDIF))
			break;
	}
	if (i < 0) return INTERPRETER_OP_ELSE_WITHOUT_PREFIX;

	// Get the statements length.
	uint64_t length = 0;
	for (uint64_t i = (*pos)+1; i < script->get_length(script); ++i)
	{
		size_t size = 0;
		BYTE *element = (BYTE *)script->get_element(script, i, &size);
		if (size == 1 && (element[0] == OP_IF || element[0] == OP_ELSE || \
		    element[0] == OP_NOTIF || element[0] == OP_ENDIF))
			break;
		else length += 1;
	}

	// Previous not executed.
	if (previous_status == OPERATION_EXECUTED)
	{
		*pos = (*pos) + length;
		return OPERATION_NOT_EXECUTED;
	}
	else
	{
		return OPERATION_EXECUTED;
	}
}

Status EXC_OP_ENDIF(CStack *stack, Script *script, uint64_t *pos)
{
	
}

Status EXC_OP_VERIFY(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;
	// Marks transaction as invalid if top stack value is not true. The top stack value is removed.
	size_t size;
	BYTE *top = (BYTE *)stack->pop(stack, &size, NULL);

	if (size == 0 && top == NULL) return INTERPRETER_FALSE;
	else if (size == 1 && top[0] == 0x00)
	{
		free(top);
		return INTERPRETER_FALSE;
	}
	else
	{	// Stack top element more than one byte.
		for (uint32_t i = 0; i < size; ++i)
		{
			if (top[i] != 0x00) return OPERATION_EXECUTED;
		}
		free(top);
		return INTERPRETER_FALSE;
	}
}

Status EXC_OP_RETURN(Script *script, uint64_t pos)
{
	size_t size;
	script->get_element(script, pos+1, &size);
	if (size > 40) return INTERPRETER_OP_RETURN_OVERSIZE;
	else return INTERPRETER_FALSE;
}

Status EXC_OP_TOALTSTACK(CStack *data_stack, CStack *alt_stack)
{
	if (data_stack->is_empty(data_stack)) return CSTACK_EMPTY;
	else if (alt_stack->is_full(alt_stack)) return CSTACK_FULL;
	size_t size;
	void *type;
	BYTE *top = data_stack->pop(data_stack, &size, &type);
	alt_stack->push(alt_stack, top, size, type);
	return OPERATION_EXECUTED;
}

Status EXC_OP_FROMALTSTACK(CStack *data_stack, CStack *alt_stack)
{
	if (data_stack->is_full(data_stack)) return CSTACK_FULL;
	else if (alt_stack->is_empty(alt_stack)) return CSTACK_EMPTY;
	size_t size;
	void *type = NULL;
	BYTE *top = alt_stack->pop(alt_stack, &size, &type);
	data_stack->push(data_stack, top, size, type);
	return OPERATION_EXECUTED;
}

Status EXC_OP_IFDUP(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	else if (stack->is_empty(stack)) return CSTACK_EMPTY;
	// If the top stack value is not 0, duplicate it.
	size_t size;
	void *type;
	BYTE *top = (BYTE *)stack->pop(stack, &size, &type);

	if (size == 0 && top == NULL)
	{
		stack->push(stack, top, size, type);
		return OPERATION_NOT_EXECUTED;
	}
	else if (size == 1 && top[0] == 0x00)
	{
		stack->push(stack, top, size, type);
		return OPERATION_NOT_EXECUTED;
	}
	else
	{	// Stack top element more than one byte.
		for (uint32_t i = 0; i < size; ++i)
		{
			if (top[i] != 0x00)
			{
				BYTE *copy = (BYTE *)malloc(size);
				if (copy == NULL)
				{	
					free(top);
					return MEMORY_ALLOCATE_FAILED;
				}
				memcpy(copy, top, size);
				stack->push(stack, top, size, type);
				if (stack->is_full(stack))
				{
					free(copy);
					return CSTACK_FULL;
				}
				stack->push(stack, copy, size, type);
				return OPERATION_EXECUTED;
			}
		}
		stack->push(stack, top, size, type);
		return OPERATION_NOT_EXECUTED;
	}
}

Status EXC_OP_DEPTH(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	uint64_t depth = stack->get_depth(stack);
	BYTE little_endian[8];
	uint8_t bytes_len = 0;

	// Convert a uint64_t number to an 8-byte length byte array.
	for (uint8_t i = 0; i < 8; ++i)
	{
		little_endian[i] = (depth << (56-i*8)) >> 56;
		if (little_endian[i] != 0x00) bytes_len += 1;
	}

	// Depth is zero.
	if (bytes_len == 0)
	{
		BYTE *num = (BYTE *)malloc(1);
		if (num == NULL) return MEMORY_ALLOCATE_FAILED;
		num[0] = 0x00;
		stack->push(stack, num, 1, BYTE_TYPE);
		return OPERATION_EXECUTED;
	}

	// The highest byte is greater than 0x7f(01111111), 0x80 is negative 0, append a 0x00.
	else if (bytes_len > 0 && little_endian[bytes_len-1] >= 0x80)
	{
		bytes_len += 1;
		BYTE *num = (BYTE *)malloc(bytes_len);
		if (num == NULL) return MEMORY_ALLOCATE_FAILED;
		memcpy(num, little_endian, bytes_len-1);
		num[bytes_len-1] = 0x00;
		stack->push(stack, num, bytes_len, BYTE_TYPE);
		return OPERATION_EXECUTED;
	}

	// The highest byte is smaller than 0x80(11111111).
	else
	{
		BYTE *num = (BYTE *)malloc(bytes_len);
		if (num == NULL) return MEMORY_ALLOCATE_FAILED;
		memcpy(num, little_endian, bytes_len);
		stack->push(stack, num, bytes_len, BYTE_TYPE);
		return OPERATION_EXECUTED;
	}
}

Status EXC_OP_DROP(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;
	void *popped = stack->pop(stack, NULL, NULL);
	if (popped != NULL) free(popped);
	return OPERATION_EXECUTED;
}

Status EXC_OP_DUP(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	else if (stack->is_empty(stack)) return CSTACK_EMPTY;
	size_t size;
	void *type;
	void *top = stack->pop(stack, &size, &type);
	void *copy = NULL;
	if (top != NULL)
	{
		copy = malloc(size);
		if (copy == NULL)
		{
			free(top);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(copy, top, size);
	}
	stack->push(stack, top, size, type);
	stack->push(stack, copy, size, type);
	return OPERATION_EXECUTED;
}

Status EXC_OP_NIP(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;
	size_t top_size;
	void *top_type;
	void *top = stack->pop(stack, &top_size, &top_type);
	void *second = stack->pop(stack, NULL, NULL);
	if (second != NULL) free(second);
	stack->push(stack, top, top_size, top_type);
	return OPERATION_EXECUTED;
}

Status EXC_OP_OVER(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	else if (stack->is_empty(stack)) return CSTACK_EMPTY;

	void *ptrs[3];
	size_t sizes[3];
	void *types[3];
	
	// Pop the elements.
	for (uint64_t i = 1; i < 3; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Copy the second-top element.
	ptrs[0] = NULL;
	if (ptrs[2] != NULL)
	{
		ptrs[0] = malloc(sizes[2]);
		if (ptrs[0] == NULL)
		{
			free(ptrs[1]); free(ptrs[2]);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(ptrs[0], ptrs[2], sizes[2]);
	}

	// Push back.
	for (uint64_t i = 0; i < 3; ++i)
		stack->push(stack, ptrs[i], sizes[i], types[i]);

	return OPERATION_EXECUTED;
}

Status EXC_OP_PICK(CStack *stack, uint64_t index)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	else if (stack->is_empty(stack)) return CSTACK_EMPTY;

	void *ptrs[index+1];
	size_t sizes[index+1];
	void *types[index+1];

	// Pop the elements.
	for (uint64_t i = 0; i <= index; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Copy the target element.
	void *copy = NULL;
	if (ptrs[index] != NULL)
	{
		copy = malloc(sizes[index]);
		if (copy == NULL)
		{
			for (uint64_t i = 0; i <= index; ++i) free(ptrs[i]);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(copy, ptrs[index], sizes[index]);
	}

	// Push back.
	for (uint64_t i = index; i > 0; --i)
		stack->push(stack, ptrs[i], sizes[i], types[i]);
	stack->push(stack, ptrs[0], sizes[0], types[0]);
	stack->push(stack, copy, sizes[index], types[index]);

	return OPERATION_EXECUTED;
}

Status EXC_OP_ROLL(CStack *stack, uint64_t index)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;
	void *ptrs[index+1];
	size_t sizes[index+1];
	void *types[index+1];

	for (uint64_t i = 0; i <= index; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);
	for (uint64_t i = index-1; i >= 0; --i)
		stack->push(stack, ptrs[i], sizes[i], types[i]);
	stack->push(stack, ptrs[0], sizes[0], types[0]);
	stack->push(stack, ptrs[index], sizes[index], types[index]);

	return OPERATION_EXECUTED;
}

Status EXC_OP_ROT(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;
	void *ptrs[3];
	size_t sizes[3];
	void *types[3];
	for (uint64_t i = 0; i < 3; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);
	stack->push(stack, ptrs[1], sizes[1], types[1]);
	stack->push(stack, ptrs[0], sizes[0], types[0]);
	stack->push(stack, ptrs[2], sizes[2], types[2]);
	return OPERATION_EXECUTED;
}

Status EXC_OP_SWAP(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;
	void *ptrs[2];
	size_t sizes[2];
	void *types[2];
	for (uint64_t i = 0; i < 2; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);
	stack->push(stack, ptrs[1], sizes[1], types[1]);
	stack->push(stack, ptrs[0], sizes[0], types[0]);
	return OPERATION_EXECUTED;
}

Status EXC_OP_TUCK(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	else if (stack->is_empty(stack)) return CSTACK_EMPTY;
	void *ptrs[3];
	size_t sizes[3];
	void *types[3];

	// Pop the elements.
	for (uint64_t i = 0; i < 2; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Copy the top element.
	ptrs[2] = NULL;
	if (ptrs[0] != NULL)
	{
		ptrs[2] = malloc(sizes[0]);
		if (ptrs[2] == NULL)
		{
			free(ptrs[0]); free(ptrs[1]);
			return MEMORY_ALLOCATE_FAILED;
		}
		memcpy(ptrs[2], ptrs[0], sizes[0]);
	}

	// Push back.
	for (uint64_t i = 0; i < 3; ++i)
		stack->push(stack, ptrs[i], sizes[i], types[i]);

	return OPERATION_EXECUTED;
}

Status EXC_OP_2DROP(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;
	void *ptrs[3];
	size_t sizes[3];
	void *types[3];
	for (uint64_t i = 0; i < 2; ++i)
	{
		ptrs[i] = stack->pop(stack, sizes+i, types+i);
		if (ptrs[i] != NULL) free(ptrs[i]);
	}
	return OPERATION_EXECUTED;
}

Status EXC_OP_2DUP(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	else if (stack->is_empty(stack)) return CSTACK_EMPTY;
	void *ptrs[4];
	size_t sizes[4];
	void *types[4];

	// Pop the elements.
	for (uint64_t i = 0; i < 2; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Duplicate the elements.
	for (uint64_t i = 2; i < 4; ++i)
	{
		sizes[i] = sizes[i-2];
		types[i] = types[i-2];
		ptrs[i] = NULL;
		if (ptrs[i-2] != NULL)
		{
			ptrs[i] = malloc(sizes[i]);
			if (ptrs[i] == NULL)
			{
				for (uint8_t j = 0; j < i; ++j) free(ptrs[j]);
				return MEMORY_ALLOCATE_FAILED;
			}
			memcpy(ptrs[i], ptrs[i-2], sizes[i-2]);
		}
	}

	// Push back.
	for (uint64_t i = 0; i < 4; ++i)
		stack->push(stack, ptrs[i], sizes[i], types[i]);
	return OPERATION_EXECUTED;
}

Status EXC_OP_3DUP(CStack *stack)
{
	// Same as EXC_OP_2DUP(), just change some numbers
	if (stack->is_full(stack)) return CSTACK_FULL;
	else if (stack->is_empty(stack)) return CSTACK_EMPTY;
	void *ptrs[6];
	size_t sizes[6];
	void *types[6];

	// Pop the elements.
	for (uint64_t i = 0; i < 3; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Duplicate the elements.
	for (uint64_t i = 3; i < 6; ++i)
	{
		sizes[i] = sizes[i-3];
		types[i] = types[i-3];
		ptrs[i] = NULL;
		if (ptrs[i-3] != NULL)
		{
			ptrs[i] = malloc(sizes[i]);
			if (ptrs[i] == NULL)
			{
				for (uint8_t j = 0; j < i; ++j) free(ptrs[j]);
				return MEMORY_ALLOCATE_FAILED;
			}
			memcpy(ptrs[i], ptrs[i-3], sizes[i-3]);
		}
	}

	// Push back.
	for (uint64_t i = 0; i < 6; ++i)
		stack->push(stack, ptrs[i], sizes[i], types[i]);
	return OPERATION_EXECUTED;
}

Status EXC_OP_2OVER(CStack *stack)
{
	if (stack->is_full(stack)) return CSTACK_FULL;
	else if (stack->is_empty(stack)) return CSTACK_EMPTY;

	void *ptrs[6];
	size_t sizes[6];
	void *types[6];

	// Pop the elements.
	for (uint64_t i = 2; i < 6; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Copy the elements.
	for (uint64_t i = 0; i < 2; ++i)
	{
		sizes[i] = sizes[i+4];
		types[i] = types[i+4];
		ptrs[i] = NULL;
		if (ptrs[i+4] != NULL)
		{
			ptrs[i] = malloc(sizes[i]);
			if (ptrs[i] == NULL)
			{
				for (uint8_t j = 2; j < 6; ++j) free(ptrs[j]);
				if (i == 1) free(ptrs[0]);
				return MEMORY_ALLOCATE_FAILED;
			}
			memcpy(ptrs[i], ptrs[i+4], sizes[i+4]);
		}
	}

	// Push back.
	for (uint64_t i = 5; i > 0; --i)
	{
		if (stack->is_full(stack)) return CSTACK_FULL;
		stack->push(stack, ptrs[i], sizes[i], types[i]);
	}
	stack->push(stack, ptrs[0], sizes[0], types[0]);

	return OPERATION_EXECUTED;
}

Status EXC_OP_2ROT(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;

	void *ptrs[6];
	size_t sizes[6];
	void *types[6];

	// Pop the elements.
	for (uint64_t i = 2; i < 6; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);
	for (uint64_t i = 0; i < 2; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Push back.
	for (uint64_t i = 0; i < 6; ++i)
		stack->push(stack, ptrs[i], sizes[i], types[i]);

	return OPERATION_EXECUTED;
}

Status EXC_OP_2SWAP(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;

	void *ptrs[4];
	size_t sizes[4];
	void *types[4];

	// Pop the elements.
	for (uint64_t i = 2; i < 4; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);
	for (uint64_t i = 0; i < 2; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Push back.
	for (uint64_t i = 0; i < 4; ++i)
		stack->push(stack, ptrs[i], sizes[i], types[i]);

	return OPERATION_EXECUTED;
}

Status EXC_OP_CAT(CStack *stack)
{	// Disabled
	return OPERATION_NOT_EXECUTED;
}

Status EXC_OP_SUBSTR(CStack *stack)
{	// Disabled
	return OPERATION_NOT_EXECUTED;
}

Status EXC_OP_LEFT(CStack *stack)
{	// Disabled
	return OPERATION_NOT_EXECUTED;
}

Status EXC_OP_RIGHT(CStack *stack)
{	// Disabled
	return OPERATION_NOT_EXECUTED;
}

Status EXC_OP_SIZE(CStack *stack)
{
	uint32_t size = stack->size[stack->get_depth(stack) - 1];
	BYTE little_endian[4];
	uint8_t bytes_len = 0;

	// Convert a uint32_t number to an 8-byte length byte array.
	for (uint8_t i = 0; i < 4; ++i)
	{
		little_endian[i] = (size << (24-i*8)) >> 24;
		if (little_endian[i] != 0x00) bytes_len += 1;
	}

	// Size is zero.
	if (bytes_len == 0)
	{
		BYTE *num = (BYTE *)malloc(1);
		if (num == NULL) return MEMORY_ALLOCATE_FAILED;
		num[0] = 0x00;
		stack->push(stack, num, 1, BYTE_TYPE);
		return OPERATION_EXECUTED;
	}

	// The highest byte is greater than 0x7f(01111111), 0x80 is negative 0, append a 0x00.
	else if (bytes_len > 0 && little_endian[bytes_len-1] >= 0x80)
	{
		bytes_len += 1;
		BYTE *num = (BYTE *)malloc(bytes_len);
		if (num == NULL) return MEMORY_ALLOCATE_FAILED;
		memcpy(num, little_endian, bytes_len-1);
		num[bytes_len-1] = 0x00;
		stack->push(stack, num, bytes_len, BYTE_TYPE);
		return OPERATION_EXECUTED;
	}

	// The highest byte is smaller than 0x80(11111111).
	else
	{
		BYTE *num = (BYTE *)malloc(bytes_len);
		if (num == NULL) return MEMORY_ALLOCATE_FAILED;
		memcpy(num, little_endian, bytes_len);
		stack->push(stack, num, bytes_len, BYTE_TYPE);
		return OPERATION_EXECUTED;
	}
}

Status EXC_OP_INVERT(CStack *stack)
{	// Disabled
	return OPERATION_NOT_EXECUTED;
}

Status EXC_OP_AND(CStack *stack)
{	// Disabled
	return OPERATION_NOT_EXECUTED;
}

Status EXC_OP_OR(CStack *stack)
{	// Disabled
	return OPERATION_NOT_EXECUTED;
}

Status EXC_OP_XOR(CStack *stack)
{	// Disabled
	return OPERATION_NOT_EXECUTED;
}

Status EXC_OP_EQUAL(CStack *stack)
{
	if (stack->is_empty(stack)) return CSTACK_EMPTY;

	void *ptrs[2];
	size_t sizes[2];
	void *types[2];

	// Pop the elements.
	for (uint64_t i = 0; i < 2; ++i)
		ptrs[i] = stack->pop(stack, sizes+i, types+i);

	// Size not equal.
	if (sizes[0] != sizes[1])
	{
		free(ptrs[0]); free(ptrs[1]);
		stack->push(stack, NULL, 0, BYTE_TYPE);
		return OPERATION_EXECUTED;
	}

	// Size equal.
	else
	{
		for (uint64_t i = 0; i < sizes[0]; ++i)
		{	// Elements not equal.
			if ( ((BYTE *)ptrs[0])[i] != ((BYTE *)ptrs[1])[i] )
			{
				free(ptrs[0]); free(ptrs[1]);
				stack->push(stack, NULL, 0, BYTE_TYPE);
				return OPERATION_EXECUTED;
			}
		}
		// Elements are equal.
		free(ptrs[0]); free(ptrs[1]);
		BYTE *num = (BYTE *)malloc(1);
		if (num == NULL) return MEMORY_ALLOCATE_FAILED;
		num[0] = 0x01;
		stack->push(stack, num, 1, BYTE_TYPE);
		return OPERATION_EXECUTED;
	}
}

Status EXC_OP_EQUALVERIFY(CStack *stack)
{
	void *ret1 = EXC_OP_EQUAL(stack);
	if (ret1 != OPERATION_EXECUTED) return ret1;
	void *ret2 = EXC_OP_VERIFY(stack);
	if (ret2 != OPERATION_EXECUTED) return ret2;
	else return OPERATION_EXECUTED;
}

Status EXC_OP_1ADD(CStack *stack)
{
	size_t size;
	BYTE *top = (BYTE *)stack->pop(stack, &size, NULL);

	
}