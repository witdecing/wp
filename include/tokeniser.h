#ifndef TOKENISER_H
#define TOKENISER_H


#include "lexer.h"


typedef struct TOKENISER{
	Lexer * lexer;
	Token ** tokens;
	unsigned int tokens_size;
} Tokeniser;


Tokeniser * tokeniser_init(Lexer * lexer) {
	Tokeniser * tokeniser = calloc(1, sizeof(Tokeniser));
	tokeniser->lexer = lexer;
	tokeniser->tokens = (void *)0;
	tokeniser->tokens_size = 0;
	return tokeniser;
}


void tokeniser_create_tokens(Tokeniser * tokeniser) {
	tokeniser->tokens = calloc(1, sizeof(Tokeniser *));
	Token * token = lexer_get_token(tokeniser->lexer);
	tokeniser->tokens[tokeniser->tokens_size] = token;
	tokeniser->tokens_size += 1;

	while (tokeniser->lexer->current_character != '\0' && tokeniser->lexer->index < tokeniser->lexer->length) {
		token = lexer_get_token(tokeniser->lexer);
		tokeniser->tokens_size += 1;
		tokeniser->tokens = realloc(tokeniser->tokens, tokeniser->tokens_size * sizeof(Tokeniser *));
		tokeniser->tokens[tokeniser->tokens_size - 1] = token;
	}
}


void tokeniser_pop_token(Tokeniser * tokeniser) {

}


#endif
