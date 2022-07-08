#ifndef __QUERY2_H__
#define __QUERY2_H__

#include "list.h"
#include "logger.h"

typedef List Query2;

/*
 * Crea una lista encadenada de los datos de la query2 vacia.
 */
Query2 query2_new(void);

/*
 * Llama a una funcion la cual agrega ordenadamente un elemento a la lista de
 * la query2, pasando como argumentos funcion de comparacion y funcion de
 * igualdad A este elemento se le asigna el año y el count pasados como
 * argumentos. En caso de error retorna un ErrorCode y en el caso de no
 * encontrar error retorna NOE.
 */
ErrorCodes query2_add(Query2 self, unsigned int year, unsigned int count);

/*
 * Llama a la funcion que genera la matriz. En caso de error retorna un
 * ErrorCode y en el caso de no encontrar error retorna NOE.
 */
ErrorCodes query2_tomatrix(Query2 self, Matrix *mat, unsigned int *rows, unsigned int *cols);

/*
 * Libera los recursos usados por la query2.
 */
void query2_free(Query2 self);

#endif // __QUERY2_H__
