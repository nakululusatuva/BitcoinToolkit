#ifndef _INTERPRETER_
#define _INTERPRETER_

#include "../common.h"
#include "script.h"

// Opcode execution status.
#define OPERATION_EXECUTED     (void *)0x004000 // No error and executed.
#define OPERATION_NOT_EXECUTED (void *)0x004001 // No error but not executed.

// Interpretation result.
#define INTERPRETER_CORRECT (void *)0x004002
#define INTERPRETER_BREAK   (void *)0x004003

// Error types.
#define INTERPRETER_ERROR                  (void *)0x004004
#define INTERPRETER_OP_ELSE_WITHOUT_PREFIX (void *)0x004005
#define INTERPRETER_IF_WITHOUT_ENDIF       (void *)0x004006
#define INTERPRETER_ENDIF_WITHOUT_IF       (void *)0x004007
#define INTERPRETER_OP_RETURN_OVERSIZE     (void *)0x004008
#define INTERPRETER_OPCODE_DISABLED        (void *)0x004009

#endif