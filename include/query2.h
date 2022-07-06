#ifndef __QUERY2_H__
#define __QUERY2_H__

#include "list.h"

typedef List Query2;

Query2 query2_new( void );
int query2_add ( Query2 self, unsigned int year, unsigned int count );
int query2_compare( struct query2 *e1, struct query2 *e2 )
int query2_ifequal( struct query2 *e1, struct query2 *e2 );
int query2_free( Query2 self );
Matrix query2_to_matrix( Query2 self );
char** query2_to_string( struct query2 *q );

#endif // __QUERY2_H__