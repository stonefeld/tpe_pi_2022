#ifndef __QUERY3_H__
#define __QUERY3_H__

#include "list.h"

typedef List Query3;

Query3 query3_new(void);
ErrorCodes query3_add(Query3 self, char* day, unsigned int day_count, unsigned int night_count, unsigned int count);
ErrorCodes query3_tomatrix(Query3 self, Matrix *mat, unsigned int *rows, unsigned int *cols);
void query3_free(Query3 self);

void query3_print(Query3 self);

#endif // __QUERY3_H__
