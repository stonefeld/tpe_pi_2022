#ifndef __QUERY1_H__
#define __QUERY1_H__

#include "list.h"

typedef List Query1;

Query1 query1_new( void );
int query1_add ( Query1 self, char* name, unsigned int count );
int query1_compare( struct query1 *e1, struct query1 *e2 );
int query1_ifequal( struct query1 *e1, struct query1 *e2 );
int query1_free( Query1 self );
Matrix query1_to_matrix( Query1 self );
int query1_orderby( struct query1 *e1, struct query1 *e2 );
char** query1_to_string( struct query1 *q );

#endif // __QUERY1_H__