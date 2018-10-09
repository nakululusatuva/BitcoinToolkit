#ifndef _INTERPRETER_
#define _INTERPRETER_

#include "../common.h"
#include "script.h"
#include "../container/CStack.h"

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
Status delete_Interpreter(Interpreter *this);
Status Interpreter_dump_data_stack(Interpreter *this);
Status Interpreter_dump_alt_stack(Interpreter *this);
Status Interpreter_launch(Interpreter *this, uint64_t pos);
Status Interpreter_load_script(Interpreter *this, Script *feed);
Script * Interpreter_unload_script(Interpreter *this);

#endif