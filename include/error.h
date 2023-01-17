#ifndef ERROR_H
#define ERROR_H


#define ERROR_RESET			"\x1B[0m"
#define ERROR_COLOUR_RED	"\x1B[1;31m"
#define ERROR_COLOUR_GREEN	"\x1B[1;32m"
#define ERROR_COLOUR_YELLOW	"\x1B[1;33m"
#define ERROR_COLOUR_BLUE	"\x1B[1;34m"
#define ERROR_COLOUR_CYAN	"\x1B[1;36m"


#include <stdio.h>


void error_init(long line, long col, char * name, char * details, char * filepath) {

/*
File {filepath} ln {line} col {col}
{errorname}: {details}
*/

	printf("\n%sFile%s [%s%s%s] ln %s%li%s col %s%li%s\n%s%s%s: %s\n\n",
			ERROR_COLOUR_BLUE,
			ERROR_RESET,
			ERROR_COLOUR_GREEN,
			filepath,
			ERROR_RESET,
			ERROR_COLOUR_CYAN,
			line,
			ERROR_RESET,
			ERROR_COLOUR_CYAN,
			col,
			ERROR_RESET,
			ERROR_COLOUR_RED,
			name,
			ERROR_RESET,
			details);
	exit(1);
}


#endif