#ifndef TOKEN_H
#define TOKEN_H


#include <stdlib.h>


typedef struct TOKEN {

	unsigned char type;
	char * value;
	unsigned int line;
} Token;


Token * token_init(unsigned char type, char * value, unsigned int line) {
	
	Token * token = (Token *) calloc(1, sizeof(Token));
	token->type = type;
	token->value = value;
	token->line = line;
	return token;
}


#endif
