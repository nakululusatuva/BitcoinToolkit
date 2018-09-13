#ifndef _OPERATION_
#define _OPERATION_

#include "script.h"
#include "interpreter.h"
#include "../container/CStack.h"

// EXC returns OPERATION_EXECUTED     : no error and executed,
//             OPERATION_NOT_EXECUTED : no error but not executed,
//             else                   : error while executing.

// Constants
void * EXC_OP_0_FALSE(CStack *stack);
void * EXC_OP_1_TRUE(CStack *stack);
void * EXC_OP_PUSHDATA(CStack *stack, Script *script, uint64_t *pos);
void * EXC_OP_PUSHDATAN(CStack *stack, Script *script, uint64_t *pos);
void * EXC_OP_1NEGATE(CStack *stack);
void * EXC_OP_2_TO_16(CStack *stack, BYTE number);

// Flow control
void * EXC_OP_NOP();
void * EXC_OP_IF(CStack *stack, Script *script, uint64_t *pos);
void * EXC_OP_NOTIF(CStack *stack, Script *script, uint64_t *pos);
void * EXC_OP_ELSE(CStack *stack, Script *script, uint64_t *pos, void *previous_status);
void * EXC_OP_ENDIF(CStack *stack, Script *script, uint64_t *pos);
void * EXC_OP_VERIFY(CStack *stack);
void * EXC_OP_RETURN(CStack *stack, Script *script, uint64_t *pos);

// Stack
void * EXC_OP_TOALTSTACK(CStack *data_stack, CStack *alt_stack);
void * EXC_OP_FROMALTSTACK(CStack *data_stack, CStack *alt_stack);
void * EXC_OP_IFDUP(CStack *stack);
void * EXC_OP_DEPTH(CStack *stack);
void * EXC_OP_DROP(CStack *stack);
void * EXC_OP_DUP(CStack *stack);
void * EXC_OP_NIP(CStack *stack);
void * EXC_OP_OVER(CStack *stack);
void * EXC_OP_PICK(CStack *stack, uint64_t index);
void * EXC_OP_ROLL(CStack *stack, uint64_t index);
void * EXC_OP_ROT(CStack *stack);
void * EXC_OP_SWAP(CStack *stack);
void * EXC_OP_TUCK(CStack *stack);
void * EXC_OP_2DROP(CStack *stack);
void * EXC_OP_2DUP(CStack *stack);
void * EXC_OP_3DUP(CStack *stack);
void * EXC_OP_2OVER(CStack *stack);
void * EXC_OP_2ROT(CStack *stack);
void * EXC_OP_2SWAP(CStack *stack);

// Splice
void * EXC_OP_CAT(CStack *stack);    // Disabled
void * EXC_OP_SUBSTR(CStack *stack); // Disabled
void * EXC_OP_LEFT(CStack *stack);   // Disabled
void * EXC_OP_RIGHT(CStack *stack);  // Disabled
void * EXC_OP_SIZE(CStack *stack);

// Bitwise logic
void * EXC_OP_INVERT(CStack *stack); // Disabled
void * EXC_OP_AND(CStack *stack);    // Disabled
void * EXC_OP_OR(CStack *stack);     // Disabled
void * EXC_OP_XOR(CStack *stack);    // Disabled
void * EXC_OP_EQUAL(CStack *stack);
void * EXC_OP_EQUALVERIFY(CStack *stack);

// Arithmetic
void * EXC_OP_1ADD(CStack *stack);

#endif