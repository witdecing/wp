#ifndef SCOPE_H
#define SCOPE_H


#include "ast.h"
#include <stdlib.h>


typedef struct SCOPE {
	Ast ** var_defs;
	unsigned int var_defs_size;
	Ast ** func_defs;
	unsigned int func_defs_size;
} Scope;


Scope * scope_init(){
	Scope * scope = calloc(1, sizeof(Scope));
	scope->var_defs = (void*) 0;
	scope->var_defs_size = 0;
	scope->func_defs = (void*) 0;
	scope->func_defs_size = 0;
	return scope;
}


Ast * scope_add_variable_definition(Scope * scope, Ast * vdef){
	if (scope->var_defs == (void*) 0) {
		scope->var_defs = calloc(1, sizeof(Ast *));
		scope->var_defs[0] = vdef;
		scope->var_defs_size += 1;
	}

	else {
		scope->var_defs_size += 1;
		scope->var_defs = realloc(scope->var_defs, scope->var_defs_size * sizeof(Ast *));
		scope->var_defs[scope->var_defs_size - 1] = vdef;
	}
	return vdef;
}


Ast * scope_get_variable_definition(Scope * scope, const char * name){
	for (int i = 0; i < scope->var_defs_size; i++) {
		Ast * vdef = scope->var_defs[i];

		if (strcmp(vdef->variable_def_name, name) == 0) {
			return vdef;
		}
	}
	return (void*)0;
}


Ast * scope_add_function_definition(Scope * scope, Ast * fdef){

}


Ast * scope_get_function_definition(Scope * scope, const char * fname){

}


#endif
