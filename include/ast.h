#ifndef AST_H
#define AST_H


#include "scope.h"


typedef struct AST {
	unsigned char type;

	struct SCOPE * scope;

	char char_value;
	long long int_value;
	long double float_value;
	char * string_value;

	char * variable_name;

	unsigned char variable_def_type;
	char * variable_def_name;
	struct AST * variable_def_value;

	unsigned char func_def_type;
	char * func_def_name;
	struct AST ** func_def_args;
	unsigned char func_def_args_size;
	struct AST * func_def_body;

	struct AST ** compound;
	unsigned char compound_size;
} Ast;


Ast * ast_init(unsigned char type) {
	Ast * ast = calloc(1, sizeof(Ast));
	ast->type = type;
	ast->scope = (void *)0;
	ast->char_value = 0;
	ast->int_value = 0;
	ast->float_value = 0.0;
	ast->string_value = (void *)0;
	ast->variable_name = (void *)0;
	ast->variable_def_type = 0;
	ast->variable_def_name = (void *)0;
	ast->variable_def_value = (void *)0;
	ast->func_def_type = 0;
	ast->func_def_name = (void*) 0;
	ast->func_def_args = (void*) 0;
	ast->func_def_args_size = 0;
	ast->func_def_body = (void*) 0;
	ast->compound = (void*) 0;
	ast->compound_size = 0;
	return ast;
}


#endif
