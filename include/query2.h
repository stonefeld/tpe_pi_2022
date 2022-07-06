#ifndef __QUERY2_H__
#define __QUERY2_H__

#include "list.h"

typedef List Query2;

Query2 query2_new(void);
int query2_add (Query2 self, unsigned int year, unsigned int count);
Matrix query2_to_matrix(Query2 self, unsigned int *rows, unsigned int *cols);
void query2_free(Query2 self);

void query2_print(Query2 self);

#endif // __QUERY2_H__
