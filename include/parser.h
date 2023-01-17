#ifndef PARSER_H
#define PARSER_H


#include "tokeniser.h"
#include "visitor.h"
#include <stdio.h>


typedef struct PARSER {
	Tokeniser * tokeniser;
	Token * current_token;
	Token * previous_token;
	unsigned int current_token_position;
} Parser;


Ast * parser_process_statements(Parser * parser);
Ast * parser_process_variable_def(Parser * parser);
Ast * parser_process_func_def(Parser * parser);
Ast * parser_process_id(Parser * parser);
Ast * parser_process_expression(Parser * parser);


Parser * parser_init(Tokeniser * tokeniser) {
	Parser * parser = calloc(1, sizeof(Parser));
	parser->tokeniser = tokeniser;
	tokeniser_create_tokens(parser->tokeniser);
	parser->current_token = parser->tokeniser->tokens[0];
	parser->previous_token = parser->tokeniser->tokens[0];
	parser->current_token_position = 0;
	return parser;
}


void parser_process(Parser * parser, unsigned char type) {
	if (parser->current_token->type == type) {
		parser->previous_token = parser->current_token;
		parser->current_token_position += 1;
		parser->current_token = parser->tokeniser->tokens[parser->current_token_position];
	}
	else {
		printf("SyntaxError: unexpected token `%s` ln %d", parser->current_token->value, parser->current_token->line);
		exit(1);
	}
}


Ast * parser_parse(Parser * parser) {
	return parser_process_statements(parser);
}


Ast * parser_process_statement(Parser * parser) {
	switch (parser->current_token->type) {
		case IDCHAR: {
			return parser_process_variable_def(parser);
		}
		case IDINT: {
			return parser_process_variable_def(parser);
		}
		case IDFLOAT: {
			return parser_process_variable_def(parser);
		}
		case IDSTRING: {
			return parser_process_variable_def(parser);
		}
		case FUNC: {
			return parser_process_func_def(parser);
		}
		case ID: {
			return parser_process_id(parser);
		}
	}
}


Ast * parser_process_statements(Parser * parser) {
	Ast * ast = ast_init(COMPOUND);
	ast->compound = calloc(1, sizeof(Ast *));
	Ast * statement = parser_process_statement(parser);
	ast->compound[0] = statement;
	ast->compound_size += 1;

	while (parser->current_token->type == NEWLINE) {
		parser_process(parser, NEWLINE);
		statement = parser_process_statement(parser);

		if (statement) {
			ast->compound_size += 1;
			ast->compound = realloc(ast->compound, ast->compound_size * sizeof(Ast *));
			ast->compound[ast->compound_size - 1] = statement;
		}
	}
	return ast;
}


Ast * parser_process_variable_def(Parser * parser) {
	parser_process(parser, parser->current_token->type);
	unsigned char var_type = parser->previous_token->type;
	char * var_name = parser->current_token->value;
	parser_process(parser, ID);
	parser_process(parser, EQ);
	Ast * var_value = parser_process_expression(parser);
	unsigned char var_def_type;
	switch (var_type) {
		case IDCHAR: {
			var_def_type = CHAR_VAR_DEF;
			Ast * ast = ast_init(var_def_type);
			ast->variable_def_type = var_type;
			ast->variable_def_name = var_name;
			ast->variable_def_value = var_value;
			return ast;
		}
		case IDINT: {
			var_def_type = INT_VAR_DEF;
			Ast * ast = ast_init(var_def_type);
			ast->variable_def_type = var_type;
			ast->variable_def_name = var_name;
			ast->variable_def_value = var_value;
			return ast;
		}
		case IDFLOAT: {
			var_def_type = FLOAT_VAR_DEF;
			Ast * ast = ast_init(var_def_type);
			ast->variable_def_type = var_type;
			ast->variable_def_name = var_name;
			ast->variable_def_value = var_value;
			return ast;
		}
		case IDSTRING: {
			var_def_type = STRING_VAR_DEF;
			Ast * ast = ast_init(var_def_type);
			ast->variable_def_type = var_type;
			ast->variable_def_name = var_name;
			ast->variable_def_value = var_value;
			return ast;
		}
	}
}


Ast * parser_process_func_def(Parser * parser) {

}


Ast * parser_process_id(Parser * parser) {

}


Ast * parser_process_expression(Parser * parser) {

}



#endif
