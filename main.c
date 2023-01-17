#include "include/lexer.h"



int main() {

	Lexer * lexer = lexer_init("example.wp");

	while (lexer->current_character != '\0' && lexer->index < lexer->length) {
		
		Token * token = lexer_get_token(lexer);
		printf("'%s'	%d]\n", token->value, token->type);
	}
	

	return 0;
}
