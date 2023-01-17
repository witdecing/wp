#ifndef VISITOR_H
#define VISITOR_H


#include "scope.h"
#include <string.h>
#include <stdio.h>


typedef struct VISITOR {

} Visitor;


Ast * visitor_process_variable(Visitor * visitor, Ast * node);
Ast * visitor_process_variable_def(Visitor * visitor, Ast * node);


Visitor * visitor_init(){
	Visitor * visitor = calloc(1, sizeof(Visitor));
	return visitor;
}


Ast * visitor_visit(Visitor * visitor, Ast * node) {
	switch (node->type) {
		case ID: {
			return visitor_process_variable(visitor, node);
			break;
		}
		case INT_VAR_DEF: {
			return visitor_process_variable_def(visitor, node);
			break;
		}
	}
}


Ast * visitor_process_variable(Visitor * visitor, Ast * node){
	Ast * vdef = scope_get_variable_definition(node->scope, node->variable_name);
	if (vdef != (void*) 0) {
		return visitor_visit(visitor, vdef->variable_def_value);
	}
    printf("Undefined variable `%s`\n", node->variable_name);
    exit(1);
}


Ast * visitor_process_variable_def(Visitor * visitor, Ast * node){
	scope_add_variable_definition(node->scope, node);
	return node;
}


Ast * visitor_process_compound(Visitor * visitor, Ast * node) {
	for (int i = 0; i < node->compound_size; i++) {
		visitor_visit(visitor, node->compound[i]);
	}
	return ast_init(0);
}


#endif
