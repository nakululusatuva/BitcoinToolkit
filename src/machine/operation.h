#ifndef _OPERATION_
#define _OPERATION_

#include "script.h"
#include "interpreter.h"
#include "../container/CStack.h"

// EXC returns OPERATION_EXECUTED     : no error and executed,
//             OPERATION_NOT_EXECUTED : no error but not executed,
//             else                   : error while executing.

// Constants
Status EXC_OP_0_FALSE(CStack *stack);
Status EXC_OP_1_TRUE(CStack *stack);
Status EXC_OP_PUSHDATA(CStack *stack, Script *script, uint64_t *pos);
Status EXC_OP_PUSHDATAN(CStack *stack, Script *script, uint64_t *pos);
Status EXC_OP_1NEGATE(CStack *stack);
Status EXC_OP_2_TO_16(CStack *stack, BYTE number);

// Flow control
Status EXC_OP_NOP();
Status EXC_OP_IF(CStack *stack, Script *script, uint64_t *pos);
Status EXC_OP_NOTIF(CStack *stack, Script *script, uint64_t *pos);
Status EXC_OP_ELSE(CStack *stack, Script *script, uint64_t *pos, void *previous_status);
Status EXC_OP_ENDIF(CStack *stack, Script *script, uint64_t *pos);
Status EXC_OP_VERIFY(CStack *stack);
Status EXC_OP_RETURN(Script *script, uint64_t pos);

// Stack
Status EXC_OP_TOALTSTACK(CStack *data_stack, CStack *alt_stack);
Status EXC_OP_FROMALTSTACK(CStack *data_stack, CStack *alt_stack);
Status EXC_OP_IFDUP(CStack *stack);
Status EXC_OP_DEPTH(CStack *stack);
Status EXC_OP_DROP(CStack *stack);
Status EXC_OP_DUP(CStack *stack);
Status EXC_OP_NIP(CStack *stack);
Status EXC_OP_OVER(CStack *stack);
Status EXC_OP_PICK(CStack *stack, uint64_t index);
Status EXC_OP_ROLL(CStack *stack, uint64_t index);
Status EXC_OP_ROT(CStack *stack);
Status EXC_OP_SWAP(CStack *stack);
Status EXC_OP_TUCK(CStack *stack);
Status EXC_OP_2DROP(CStack *stack);
Status EXC_OP_2DUP(CStack *stack);
Status EXC_OP_3DUP(CStack *stack);
Status EXC_OP_2OVER(CStack *stack);
Status EXC_OP_2ROT(CStack *stack);
Status EXC_OP_2SWAP(CStack *stack);

// Splice
Status EXC_OP_CAT(CStack *stack);    // Disabled
Status EXC_OP_SUBSTR(CStack *stack); // Disabled
Status EXC_OP_LEFT(CStack *stack);   // Disabled
Status EXC_OP_RIGHT(CStack *stack);  // Disabled
Status EXC_OP_SIZE(CStack *stack);

// Bitwise logic
Status EXC_OP_INVERT(CStack *stack); // Disabled
Status EXC_OP_AND(CStack *stack);    // Disabled
Status EXC_OP_OR(CStack *stack);     // Disabled
Status EXC_OP_XOR(CStack *stack);    // Disabled
Status EXC_OP_EQUAL(CStack *stack);
Status EXC_OP_EQUALVERIFY(CStack *stack);

// Arithmetic
Status EXC_OP_1ADD(CStack *stack);

#endif