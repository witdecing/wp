#ifndef IO_H
#define IO_H


#include "error.h"
#include <stdio.h>
#include <stdlib.h>


char * io_read_source_file(char * filepath, unsigned int * length) {
	char * content;
	unsigned int read_length;
	FILE * file = fopen(filepath, "rb");

	if (file) {

		fseek(file, 0, SEEK_END);
        *length = ftell(file);
        fseek(file, 0, SEEK_SET);

		content = (char *) malloc(*length + 1);

		if (*length) {

			read_length = fread(content, 1, *length, file);

			if (*length != read_length) {

				free(content);
				error_init(-1, -1, "FileError", "file read error", filepath);
			}
		}

		fclose(file);
		content[*length] = '\0';
		return content;
	}

	error_init(-1, -1, "FileError", "file was not found", filepath);
}


#endif