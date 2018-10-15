#include <stdlib.h>
#include <string.h>
#include "internal/machine/script.h"
#include "internal/machine/interpreter.h"
#include "internal/machine/operation.h"

Interpreter * new_Interpreter()
{
	Interpreter *new = (Interpreter *)calloc(1, sizeof(Interpreter));
	if (new == NULL) return MEMORY_ALLOCATE_FAILED;
	new->script = NULL;

	new->data_stack = new_CStack(MAX_SCRIPT_STACK_SIZE);
	if (new->data_stack == MEMORY_ALLOCATE_FAILED)
	{
		free(new);
		return MEMORY_ALLOCATE_FAILED;
	}

	new->alt_stack = new_CStack(MAX_SCRIPT_STACK_SIZE);
	if (new->alt_stack == MEMORY_ALLOCATE_FAILED)
	{
		free(new->data_stack);
		free(new);
		return MEMORY_ALLOCATE_FAILED;
	}

	new->dump_data_stack = &Interpreter_dump_data_stack;
	new->dump_alt_stack  = &Interpreter_dump_alt_stack;
	new->launch          = &Interpreter_launch;
	new->load_script     = &Interpreter_load_script;
	new->unload_script   = &Interpreter_unload_script;

	return new;
}

Status delete_Interpreter(Interpreter *self)
{
	if (self->data_stack != NULL) delete_CStack(self->data_stack);
	else if (self->alt_stack != NULL) delete_CStack(self->alt_stack);
	self->script = NULL;
	free(self);
	return SUCCEEDED;
}

Status Interpreter_dump_data_stack(Interpreter *self)
{
	delete_CStack(self->data_stack);
	self->data_stack = NULL;
	return SUCCEEDED;
}

Status Interpreter_dump_alt_stack(Interpreter *self)
{
	delete_CStack(self->alt_stack);
	self->alt_stack = NULL;
	return SUCCEEDED;
}

Status Interpreter_launch(Interpreter *self, uint64_t start_point)
{
	void *status = NULL;
	BYTE *element = NULL;
	size_t size = 0;
	uint64_t cursor = start_point;
	
while (cursor < self->script->get_length(self->script))
//for (uint64_t cursor = start_point; cursor < self->script->get_length(self->script); ++cursor)
{
	element = self->script->get_element(self->script, cursor, &size);
	if (size != 1) return SCRIPT_CONTAINED_INVALID_ELEMENT;

	switch (element[0])
	{
	// Constants
		case OP_0: // OP_FALSE
		{
			status = EXC_OP_0_FALSE(self->data_stack);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_1: // OP_TRUE
		{
			status = EXC_OP_1_TRUE(self->data_stack);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case 0x01:case 0x02:case 0x03:case 0x04:case 0x05:case 0x06:case 0x07:case 0x08:case 0x09: \
		case 0x0a:case 0x0b:case 0x0c:case 0x0d:case 0x0e:case 0x0f:case 0x10:case 0x11:case 0x12: \
		case 0x13:case 0x14:case 0x15:case 0x16:case 0x17:case 0x18:case 0x19:case 0x1a:case 0x1b: \
		case 0x1c:case 0x1d:case 0x1e:case 0x1f:case 0x20:case 0x21:case 0x22:case 0x23:case 0x24: \
		case 0x25:case 0x26:case 0x27:case 0x28:case 0x29:case 0x2a:case 0x2b:case 0x2c:case 0x2d: \
		case 0x2e:case 0x2f:case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36: \
		case 0x37:case 0x38:case 0x39:case 0x3a:case 0x3b:case 0x3c:case 0x3d:case 0x3e:case 0x3f: \
		case 0x40:case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:case 0x48: \
		case 0x49:case 0x4a:case 0x4b:
		{
			status = EXC_OP_PUSHDATA(self->data_stack, self->script, &cursor);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_PUSHDATA1:case OP_PUSHDATA2:case OP_PUSHDATA4:
		{
			status = EXC_OP_PUSHDATAN(self->data_stack, self->script, &cursor);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_1NEGATE:
		{
			status = EXC_OP_1NEGATE(self->data_stack);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_2:case OP_3:case OP_4:case OP_5:case OP_6:case OP_7:case OP_8:case OP_9: \
		case OP_10:case OP_11:case OP_12:case OP_13:case OP_14:case OP_15:case OP_16:
		{
			status = EXC_OP_2_TO_16(self->data_stack, element[0]-0x50);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}

	// Flow control
		case OP_NOP:
		{
			status = EXC_OP_NOP();
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_IF:
		{
			status = EXC_OP_IF(self->data_stack, self->script, &cursor);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_NOTIF:
		{
			status = EXC_OP_NOTIF(self->data_stack, self->script, &cursor);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_ELSE:
		{
			status = EXC_OP_ELSE(self->data_stack, self->script, &cursor, status);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_ENDIF:
		{
			status = EXC_OP_ENDIF(self->data_stack, self->script, &cursor);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_VERIFY:
		{
			status = EXC_OP_VERIFY(self->data_stack);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
		case OP_RETURN:
		{
			status = EXC_OP_RETURN(self->script, cursor);
			if (status == OPERATION_EXECUTED ||
				status == OPERATION_NOT_EXECUTED) break;
			else return status;
		}
	}
	cursor++;
}
	size_t top_size;
	BYTE *top = (BYTE *)self->data_stack->pop(self->data_stack, &top_size, NULL);
	if ( (top_size == 1 && top == 0x00) || (top == NULL) )
	{
		free(top);
		return INTERPRETER_FALSE;
	}
	else
	{
		free(top);
		return INTERPRETER_TRUE;
	}
}

Status Interpreter_load_script(Interpreter *self, Script *feed)
{
	if (self->script != NULL) return INTERPRETER_ALREADY_LOADED;
	self->script = feed;
	return SUCCEEDED;
}

Script * Interpreter_unload_script(Interpreter *self)
{
	if (self->script == NULL) return INTERPRETER_NO_SCRIPT_LOADED;
	Script *buffer = self->script;
	self->script = NULL;
	return buffer;
}