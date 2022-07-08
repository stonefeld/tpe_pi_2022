#ifndef __QUERY1_H__
#define __QUERY1_H__

#include "list.h"
#include "logger.h"

typedef List Query1;

/*
*	Crea una lista encadenada de los datos de la query 1 vacia
*/
Query1 query1_new(void);

/*
*	Llama a una funcion la cual agrega ordenadamente un elemento a la lista de la query 1, pasando como argumentos funcion de comparacion y funcion de igualdad 
*	A este elemento se le asigna el nombre, el id y el count pasados como argumentos
*	En caso de error retorna un ErrorCode y en el caso de no encontrar error retorna NOE
*/
ErrorCodes query1_add(Query1 self, unsigned int id, char *name, unsigned int count);

/*
*	Antes de realizar la matriz, esta funcion llama al funcion que ordena los elementos de la query 1
*	Se ordena utilizando count (de mayor a menor) y en caso de igualdad se desempata por nombre
*	Luego llama a la funcion que genera la matriz
*	En caso de error retorna un ErrorCode y en el caso de no encontrar error retorna NOE
*/
ErrorCodes query1_tomatrix(Query1 self, Matrix *mat, unsigned int *rows, unsigned int *cols);

/*
*	Libera los recursos usados por la query 1
*/
void query1_free(Query1 self);

void query1_print(Query1 self);

#endif // __QUERY1_H__
