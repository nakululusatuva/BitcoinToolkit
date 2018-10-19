/** AUTOHEADER TAG: DELETE BEGIN **/
#ifndef _INTERPRETER_
#define _INTERPRETER_

#include "internal/common.h"
#include "internal/machine/script.h"
#include "internal/container/CStack.h"
/** AUTOHEADER TAG: DELETE END **/

/* 0x1040 ~ 0x1050 : Interpreter */
// Opcode execution status.
#define OPERATION_EXECUTED     (void *)0x1040 // No error and executed.
#define OPERATION_NOT_EXECUTED (void *)0x1041 // No error but not executed.
// Interpretation result.
#define INTERPRETER_TRUE  (void *)0x1042
#define INTERPRETER_FALSE (void *)0x1043
// Interpret-time errors.
#define INTERPRETER_ERROR                  (void *)0x1044
#define INTERPRETER_OP_ELSE_WITHOUT_PREFIX (void *)0x1045
#define INTERPRETER_IF_WITHOUT_ENDIF       (void *)0x1046
#define INTERPRETER_ENDIF_WITHOUT_IF       (void *)0x1047
#define INTERPRETER_OP_RETURN_OVERSIZE     (void *)0x1048
#define INTERPRETER_OPCODE_DISABLED        (void *)0x1049
// Functional errors.
#define INTERPRETER_ALREADY_LOADED   (void *)0x104A
#define INTERPRETER_NO_SCRIPT_LOADED (void *)0x104B

typedef struct Interpreter Interpreter;
struct Interpreter
{
	Script *script;
	CStack *data_stack;
	CStack *alt_stack;

	Status (*dump_data_stack)(Interpreter *);
	Status (*dump_alt_stack)(Interpreter *);
	Status (*launch)(Interpreter *, uint64_t);
	Status (*load_script)(Interpreter *, Script *);
	Script * (*unload_script)(Interpreter *);
};

Interpreter * new_Interpreter();
Status delete_Interpreter(Interpreter *self);

/** AUTOHEADER TAG: DELETE BEGIN **/
Status Interpreter_dump_data_stack(Interpreter *self);
Status Interpreter_dump_alt_stack(Interpreter *self);
Status Interpreter_launch(Interpreter *self, uint64_t pos);
Status Interpreter_load_script(Interpreter *self, Script *feed);
Script * Interpreter_unload_script(Interpreter *self);

#endif
/** AUTOHEADER TAG: DELETE END **/