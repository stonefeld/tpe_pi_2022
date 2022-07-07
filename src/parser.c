#include "config.h"
#include "parser.h"
#include "logger.h"
#include "utils.h"

void
parser_get(char *stream, unsigned int *keys, char **tokens, unsigned int dim)
{
	char delim[2] = ";";
	char *token;

	*keys = 0;
	token = strtok(stream, delim);

	while (dim > 0 && token != NULL) {
		tokens[(*keys)++] = token;
		token = strtok(NULL, delim);
		dim--;
	}
}
