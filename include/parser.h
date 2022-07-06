#ifndef __PARSER_H__
#define __PARSER_H__

char** parser_get(char *stream, unsigned int *rows);
unsigned int parser_get_int(char *stream);
char parser_get_char(char *stream);

#endif // __PARSER_H__
