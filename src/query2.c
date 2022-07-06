#include "logger.h"
#include "parser.h"
#include "sensors.h"
#include "readings.h"
#include "utils.h"
#include "query2.h"

#define QUERY2_COLS 2
#define YEAR_LEN 5

struct query2{
	unsigned int year;
	unsigned int count;	
};

Query2 query2_new( void ){
	return list_new();
}

//1 si no se pudo agregar, 0 si se pudo agregar
int query2_add ( Query2 self, unsigned int year, unsigned int count ){
	
	struct query2 *q = malloc(sizeof(struct query2));
	int c = 0;
	
	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		c = 1;
	} else {
		q->year = year;
		q->count = count;
		
		c = list_add( self, q, query2_compare, query2_ifequal );
	}
	
	return c;
}

int query2_compare( struct query2 *e1, struct query2 *e2 ){
	return e1->year1 - e2->year2;
}

int query2_ifequal( struct query2 *e1, struct query2 *e2 ){
	
	e1->count += e2->count;
	free(e2);
	
	return 0;
}

int query2_free( Query2 self ){
	return Listfree( Query2 self );
}

Matrix query2_to_matrix( Query2 self ){
	if ( self == NULL ){
		return NULL;
	}
	
	return list_to_matrix( self, query2_to_string);
	
}

char** query2_to_string( struct query2 *q ){
	char** string_vector = malloc(QUERY2_COLS*sizeof(char*));
	
	array_query[0] = malloc(YEAR_LEN);
	array_query[1] = malloc(BLOCK);
	
	sprintf(array_query[0], "%d", q->year);
	sprintf(array_query[1], "%d", q->count);
	
	return string_vector;
}