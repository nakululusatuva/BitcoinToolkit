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

	void * (*dump_data_stack)(Interpreter *);
	void * (*dump_alt_stack)(Interpreter *);
	void * (*launch)(Interpreter *, uint64_t);
	void * (*load_script)(Interpreter *, Script *);
	Script * (*unload_script)(Interpreter *);
};

Interpreter * new_Interpreter();
void * delete_Interpreter(Interpreter *this);
void * Interpreter_dump_data_stack(Interpreter *this);
void * Interpreter_dump_alt_stack(Interpreter *this);
void * Interpreter_launch(Interpreter *this, uint64_t pos);
void * Interpreter_load_script(Interpreter *this, Script *feed);
Script * Interpreter_unload_script(Interpreter *this);

#endif