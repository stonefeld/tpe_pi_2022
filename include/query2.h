#ifndef __QUERY2_H__
#define __QUERY2_H__

#include "list.h"
#include "logger.h"

typedef List Query2;

/*
 * Crea una lista encadenada de los datos de la query2 vacia.
 * @return: la lista de la query vacia
 */
Query2 query2_new(void);

/*
 * Llama a una funcion la cual agrega un elemento a la lista de la query2.
 * @param: self: el puntero al TAD a modificar
 * @param: year: el anio en que fue leido el dato
 * @param: count: la cantidad de registros leidos por ese sensor en ese anio
 * particular
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes query2_add(Query2 self, unsigned int year, unsigned int count);

/*
 * Llama a la funcion que genera la matriz. En caso de error retorna un
 * ErrorCode y en el caso de no encontrar error retorna NOE.
 * @param: self: el puntero al TAD a modificar
 * @param: mat: el puntero a la matriz donde quedaran almacenados los datos en
 * forma de matriz de strings
 * @param: rows: donde se almacenara la cantidad final de filas
 * @param: cols: donde se almacenara la cantidad final de columnas
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes query2_tomatrix(Query2 self, Matrix *mat, unsigned int *rows, unsigned int *cols);

/*
 * Libera los recursos usados por la query2.
 * @param: self: el puntero al TAD a ser liberado
 */
void query2_free(Query2 self);

#endif // __QUERY2_H__
