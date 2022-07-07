#ifndef __QUERY1_H__
#define __QUERY1_H__

#include "list.h"
#include "logger.h"

typedef List Query1;

Query1 query1_new(void);
ErrorCodes query1_add(Query1 self, unsigned int id, char *name, unsigned int count);
ErrorCodes query1_tomatrix(Query1 self, Matrix *mat, unsigned int *rows, unsigned int *cols);
void query1_free(Query1 self);

void query1_print(Query1 self);

#endif // __QUERY1_H__
