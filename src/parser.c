#include "parser.h"
#include "utils.h"

#define BLOCK 20

char**
parser_get(char *stream, unsigned int *rows)
{
	char **ret = malloc(BLOCK * sizeof(char*));
	char delim[2] = ";";

	char *token;
	token = strtok(stream, delim);
	*rows = 0;

	while (token != NULL) {
		ret[(*rows)++] = token;
		token = strtok(NULL, delim);
	}

	ret = realloc(ret, *rows * sizeof(char*));
	return ret;
}
