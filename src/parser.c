#include "parser.h"
#include "logger.h"
#include "utils.h"

#define BLOCK 20

char**
parser_get(char *stream, unsigned int *rows)
{
	char **ret = malloc(BLOCK * sizeof(char*));
	*rows = 0;
	if (errno == ENOMEM) {
		ret = NULL;
		log_error("No hay suficiente memoria");
	} else {
		char delim[2] = ";";

		char *token;
		token = strtok(stream, delim);

		while (token != NULL) {
			if (*rows % BLOCK == 0) {
				ret = realloc(ret, (*rows + BLOCK) * sizeof(char*));
				if (errno == ENOMEM) {
					log_error("No hay suficiente memoria");
					return ret;
				}
			}
			ret[(*rows)++] = token;
			token = strtok(NULL, delim);
		}

		ret = realloc(ret, *rows * sizeof(char*));
		if (errno == ENOMEM)
			log_error("No hay suficiente memoria");
	}
	return ret;
}
