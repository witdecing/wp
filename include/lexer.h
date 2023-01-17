#ifndef LEXER_H
#define LEXER_H


#include "token.h"
#include "tokens.h"
#include "io.h"
#include <string.h>
#include <ctype.h>
#include <regex.h>


typedef struct LEXER {

	unsigned char current_character;
	unsigned int index;
	unsigned int column;
	unsigned int line;
	unsigned int length;
	char * content;
	char * filepath;
} Lexer;


Token * lexer_proceed_token(Lexer * lexer, Token * token);
Token * lexer_get_identifier_token(Lexer * lexer);
Token * lexer_get_number_token(Lexer * lexer);
Token * lexer_get_string_token(Lexer * lexer);
Token * lexer_get_char_token(Lexer * lexer);
char * lexer_char_to_string(char character);


Lexer * lexer_init(char * filepath) {
	Lexer * lexer = (Lexer *) calloc(1, sizeof(Lexer));
	lexer->content = io_read_source_file(filepath, &lexer->length);
	lexer->current_character = lexer->content[0];
	lexer->index = 0;
	lexer->column = 1;
	lexer->line = 1;
	return lexer;
}


void lexer_proceed(Lexer * lexer) {

	if (lexer->current_character != '\0' && lexer->index < lexer->length) {

		lexer->index += 1;
		lexer->column += 1;
		lexer->current_character = lexer->content[lexer->index];
	}
}


Token * lexer_proceed_token(Lexer * lexer, Token * token) {

	lexer_proceed(lexer);
	return token;
}


Token * lexer_get_newline_token(Lexer * lexer) {
	
	lexer_proceed(lexer);
	lexer->line += 1;
	lexer->column = 1;
	return token_init(WHITESPACE_TAB, "\\n", 0);
}


Token * lexer_get_token(Lexer * lexer) {

	while (lexer->current_character != '\0' && lexer->index < lexer->length) {

		if (lexer->current_character == ' ') return lexer_proceed_token(lexer, token_init(WHITESPACE_SPACE, " ", lexer->line));

		if (lexer->current_character == '\t') return lexer_proceed_token(lexer, token_init(WHITESPACE_TAB, "\\t", lexer->line));

		if (lexer->current_character == '\n') return lexer_get_newline_token(lexer);

		if (isdigit(lexer->current_character)) return lexer_get_number_token(lexer);

		if (isalnum(lexer->current_character) || lexer->current_character == '_') return lexer_get_identifier_token(lexer);

		if (lexer->current_character == '\"') return lexer_get_string_token(lexer);

		if (lexer->current_character == '\'') return lexer_get_char_token(lexer);

		switch (lexer->current_character) {
			case '+': {

				lexer_proceed(lexer);

				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_ADDEQ, "+=", lexer->line));

				return token_init(OPERATION_ADD, "+", lexer->line);
			}
			case '-': {

				lexer_proceed(lexer);

				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_SUBEQ, "-=", lexer->line));
				
				return token_init(OPERATION_SUB, "-", lexer->line);
			}
			case '*': {
				
				lexer_proceed(lexer);
				
				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_MULEQ, "*=", lexer->line));
				
				return token_init(OPERATION_MUL, "*", lexer->line);
			}
			case '/': {
				
				lexer_proceed(lexer);
				
				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_DIVEQ, "/=", lexer->line));
				return token_init(OPERATION_DIV, "/", lexer->line);
			}
			case '%': {
				
				lexer_proceed(lexer);
				
				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_DIVEQ, "%=", lexer->line));
				
				return token_init(OPERATION_MOD, "%", lexer->line);
			}
			case '>': {
				
				lexer_proceed(lexer);
				
				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_GTHANEQ, ">=", lexer->line));
				
				return token_init(OPERATION_GTHAN, ">", lexer->line);
			}
			case '<': {
				
				lexer_proceed(lexer);
				
				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_LTHANEQ, "<=", lexer->line));
				
				return token_init(OPERATION_LTHAN, "<", lexer->line);
			}
			case '!': {
				
				lexer_proceed(lexer);
				
				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_NOTEQ, "!=", lexer->line));
				
				return token_init(OPERATION_NOT, "!", lexer->line);
			}
			case '|': {
				
				lexer_proceed(lexer);
				
				if (lexer->current_character == '|') return lexer_proceed_token(lexer, token_init(OPERATION_OR, "||", lexer->line));
				
				if (lexer->current_character == '=') return lexer_proceed_token(lexer, token_init(OPERATION_OREQ, "|=", lexer->line));
				
				error_init(lexer->line, lexer->column, "SyntaxError", "unexpected character '%c' expected '||' or '|='", lexer->filepath);
			}
			case '=': {
				
				lexer_proceed(lexer);
				
				if (lexer->current_character == '=')return lexer_proceed_token(lexer, token_init(OPERATION_EQEQ, "==", lexer->line));
				
				return token_init(OPERATION_EQ, "=", lexer->line);
			}
			case '.': {
				
				return lexer_proceed_token(lexer, token_init(CONSTANT_DOT, ".", lexer->line));
			}
			case ',': {
				
				return lexer_proceed_token(lexer, token_init(CONSTANT_COMMA, ",", lexer->line));
			}
			case '(': {
				
				return lexer_proceed_token(lexer, token_init(CONSTANT_LPAREN, "(", lexer->line));
			}
			case ')': {
				
				return lexer_proceed_token(lexer, token_init(CONSTANT_RPAREN, ")", lexer->line));
			}
			case '[': {
				
				return lexer_proceed_token(lexer, token_init(CONSTANT_LBRACK, "[", lexer->line));
			}
			case ']': {
				
				return lexer_proceed_token(lexer, token_init(CONSTANT_RBRACK, "]", lexer->line));
			}
			case '{': {
				
				return lexer_proceed_token(lexer, token_init(CONSTANT_LCURL, "{", lexer->line));
			}
			case '}': {
				
				return lexer_proceed_token(lexer, token_init(CONSTANT_RCURL, "}", lexer->line));
			}
		}
	}
	return token_init(SPECIAL_EOF, "\0", lexer->line);
}


Token * lexer_get_identifier_token(Lexer * lexer) {
	
	char * value = (char *) calloc(1, sizeof(char));
	value[0] = '\0';

	while (isalnum(lexer->current_character) || lexer->current_character == '_') {
		
		char * character = lexer_char_to_string(lexer->current_character);
		value = (char *) realloc(value, (strlen(value) + strlen(character) + 1) * sizeof(char));
		strcat(value, character);
		lexer_proceed(lexer);
	}
	

	if (strcmp(value, "char") == 0) return token_init(DATA_TYPE_CHAR, value, lexer->line);
	if (strcmp(value, "schar") == 0) return token_init(DATA_TYPE_SIGNED_CHAR, value, lexer->line);
	if (strcmp(value, "uchar") == 0) return token_init(DATA_TYPE_UNSIGNED_CHAR, value, lexer->line);

	if (strcmp(value, "short") == 0) return token_init(DATA_TYPE_SHORT, value, lexer->line);
	if (strcmp(value, "sshort") == 0) return token_init(DATA_TYPE_SIGNED_SHORT, value, lexer->line);
	if (strcmp(value, "ushort") == 0) return token_init(DATA_TYPE_UNSIGNED_SHORT, value, lexer->line);

	if (strcmp(value, "int") == 0) return token_init(DATA_TYPE_INT, value, lexer->line);
	if (strcmp(value, "sint") == 0) return token_init(DATA_TYPE_SIGNED_INT, value, lexer->line);
	if (strcmp(value, "uint") == 0) return token_init(DATA_TYPE_UNSIGNED_INT, value, lexer->line);

	if (strcmp(value, "long") == 0) return token_init(DATA_TYPE_LONG, value, lexer->line);
	if (strcmp(value, "slong") == 0) return token_init(DATA_TYPE_SIGNED_LONG, value, lexer->line);
	if (strcmp(value, "ulong") == 0) return token_init(DATA_TYPE_UNSIGNED_LONG, value, lexer->line);

	if (strcmp(value, "llong") == 0) return token_init(DATA_TYPE_LONG_LONG, value, lexer->line);
	if (strcmp(value, "sllong") == 0) return token_init(DATA_TYPE_SIGNED_LONG_LONG, value, lexer->line);
	if (strcmp(value, "ullong") == 0) return token_init(DATA_TYPE_UNSIGNED_LONG_LONG, value, lexer->line);

	if (strcmp(value, "float") == 0) return token_init(DATA_TYPE_FLOAT, value, lexer->line);
	if (strcmp(value, "double") == 0) return token_init(DATA_TYPE_DOUBLE, value, lexer->line);
	if (strcmp(value, "ldouble") == 0) return token_init(DATA_TYPE_LONG_DOUBLE, value, lexer->line);

	if (strcmp(value, "string") == 0) return token_init(DATA_TYPE_STRING, value, lexer->line);
	if (strcmp(value, "void") == 0) return token_init(DATA_TYPE_VOID, value, lexer->line);
	if (strcmp(value, "uni") == 0) return token_init(DATA_TYPE_UNI, value, lexer->line);

	if (strcmp(value, "function") == 0) return token_init(DATA_TYPE_FUNCTION, value, lexer->line);
	if (strcmp(value, "class") == 0) return token_init(DATA_TYPE_CLASS, value, lexer->line);
	if (strcmp(value, "struct") == 0) return token_init(DATA_TYPE_STRUCT, value, lexer->line);
	if (strcmp(value, "enum") == 0) return token_init(DATA_TYPE_ENUM, value, lexer->line);
	if (strcmp(value, "list") == 0) return token_init(DATA_TYPE_LIST, value, lexer->line);
	if (strcmp(value, "dict") == 0) return token_init(DATA_TYPE_DICT, value, lexer->line);

	if (strcmp(value, "if") == 0) return token_init(KEYWORD_IF, value, lexer->line);
	if (strcmp(value, "elif") == 0) return token_init(KEYWORD_ELIF, value, lexer->line);
	if (strcmp(value, "else") == 0) return token_init(KEYWORD_ELSE, value, lexer->line);
	if (strcmp(value, "and") == 0) return token_init(KEYWORD_AND, value, lexer->line);
	if (strcmp(value, "or") == 0) return token_init(KEYWORD_OR, value, lexer->line);
	if (strcmp(value, "not") == 0) return token_init(KEYWORD_NOT, value, lexer->line);
	if (strcmp(value, "for") == 0) return token_init(KEYWORD_FOR, value, lexer->line);
	if (strcmp(value, "while") == 0) return token_init(KEYWORD_WHILE, value, lexer->line);
	if (strcmp(value, "repeat") == 0) return token_init(KEYWORD_REPEAT, value, lexer->line);
	if (strcmp(value, "do") == 0) return token_init(KEYWORD_DO, value, lexer->line);
	if (strcmp(value, "break") == 0) return token_init(KEYWORD_BREAK, value, lexer->line);
	if (strcmp(value, "continue") == 0) return token_init(KEYWORD_CONTINUE, value, lexer->line);
	if (strcmp(value, "switch") == 0) return token_init(KEYWORD_SWITCH, value, lexer->line);
	if (strcmp(value, "case") == 0) return token_init(KEYWORD_CASE, value, lexer->line);
	if (strcmp(value, "default") == 0) return token_init(KEYWORD_DEFAULT, value, lexer->line);
	if (strcmp(value, "return") == 0) return token_init(KEYWORD_RETURN, value, lexer->line);
	if (strcmp(value, "import") == 0) return token_init(KEYWORD_IMPORT, value, lexer->line);
	if (strcmp(value, "relative") == 0) return token_init(KEYWORD_RELATIVE, value, lexer->line);
	if (strcmp(value, "local") == 0) return token_init(KEYWORD_LOCAL, value, lexer->line);
	if (strcmp(value, "web") == 0) return token_init(KEYWORD_WEB, value, lexer->line);
	if (strcmp(value, "cache") == 0) return token_init(KEYWORD_CACHE, value, lexer->line);

	if (strcmp(value, "null") == 0) return token_init(VALUE_NULL, value, lexer->line);

	return token_init(DATA_TYPE_ID, value, lexer->line);
}


Token * lexer_get_number_token(Lexer * lexer) {
	char * value = calloc(1, sizeof(char));
	value[0] = '\0';
	int dot = 0;

	while (isdigit(lexer->current_character) || lexer->current_character == '.') {
		
		if (lexer->current_character == '.') {
			
			if (dot) break;
			
			dot = 1;
			char * character = lexer_char_to_string(lexer->current_character);
			value = realloc(value, (strlen(value) + strlen(character) + 1) * sizeof(char));
			strcat(value, character);
			lexer_proceed(lexer);
		}

		else {
			
			char * character = lexer_char_to_string(lexer->current_character);
			value = realloc(value, (strlen(value) + strlen(character) + 1) * sizeof(char));
			strcat(value, character);
			lexer_proceed(lexer);
		}
	}

	if (dot) return token_init(VALUE_FLOAT, value, lexer->line);

	return token_init(VALUE_INT, value, lexer->line);
}


Token * lexer_get_string_token(Lexer * lexer) {

	lexer_proceed(lexer);
	char * value = calloc(1, sizeof(char));
	value[0] = '\0';

	while (lexer->current_character != '\"') {

		char * character = lexer_char_to_string(lexer->current_character);
		value = realloc(value, (strlen(value) + strlen(character) + 1) * sizeof(char));
		strcat(value, character);
		lexer_proceed(lexer);
	}

	lexer_proceed(lexer);
	return token_init(VALUE_STRING, value, lexer->line);
}


Token * lexer_get_char_token(Lexer * lexer) {

	lexer_proceed(lexer);
	char * value = calloc(1, sizeof(char));
	value[0] = '\0';

	while (lexer->current_character != '\'') {

		char * character = lexer_char_to_string(lexer->current_character);
		value = realloc(value, (strlen(value) + strlen(character) + 1) * sizeof(char));
		strcat(value, character);
		lexer_proceed(lexer);
	}
	
	lexer_proceed(lexer);
	return token_init(VALUE_CHAR, value, lexer->line);
}


char * lexer_char_to_string(char character) {
	
	char * value = calloc(2, sizeof(char));
  	value[0] = character;
  	value[1] = '\0';
	return value;
}


#endif