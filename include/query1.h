#ifndef __QUERY1_H__
#define __QUERY1_H__

#include "list.h"

typedef List Query1;

Query1 query1_new(void);
int query1_add(Query1 self, unsigned int id, char *name, unsigned int count);
void query1_free(Query1 self);
Matrix query1_to_matrix(Query1 self, unsigned int *rows);

#endif // __QUERY1_H__
