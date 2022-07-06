#include "logger.h"
#include "parser.h"
#include "sensors.h"
#include "readings.h"
#include "utils.h"
#include "query1.h"

#define QUERY1_COLS 2

struct query1{
	char* name;
	unsigned int id;
	unsigned int count;	
};

Query1 query1_new( void ){
	return list_new();
}

//1 si no se pudo agregar, 0 si se pudo agregar
int query1_add ( Query1 self, char* name, unsigned int count, unsigned int id ){
	
	struct query1 *q = malloc(sizeof(struct query1));
	int c = 0;
	
	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		c = 1;
	} else {
		q->name = name;
		q->count = count;
		q->id = id;
		
		c = list_add( self, q, query1_compare, query1_ifequal );
	}
	
	return c;
}

int query1_compare( struct query1 *e1, struct query1 *e2 ){
	return e1->id - e2->id;
}

int query1_ifequal( struct query1 *e1, struct query1 *e2 ){
	
	e1->count += e2->count;
	free(e2);
	
	return 0;
}

int query1_free( Query1 self ){
	return Listfree( Query1 self );
}

Matrix query1_to_matrix( Query1 self ){
	if ( self == NULL ){
		return NULL;
	}
	list_order ( self, query_orderby );
	return list_to_matrix( self, query1_to_string);
}

int query1_orderby( struct query1 *e1, struct query1 *e2 ){
	int n = e1->count - e2->count;
	if( n == 0 ){
		return strcmp(e2->name, e1->name);
	}
	return n;
}

char** query1_to_string( struct query1 *q ){
	char** string_vector = malloc(QUERY1_COLS*sizeof(char*));
	
	array_query[0] = malloc(strlen(q->name));
	strcpy(array_query[0], q->name);
	
	array_query[1] = malloc(BLOCK);
	sprintf(array_query[1], "%d", q->count);
	
	return string_vector;
}