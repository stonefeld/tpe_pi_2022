#ifndef __QUERY1_H__
#define __QUERY1_H__

#include "list.h"
#include "logger.h"

typedef List Query1;

/*
 * Crea una lista encadenada de los datos de la query1 vacia.
 * @return: la lista de la query vacia
 */
Query1 query1_new(void);

/*
 * Llama a una funcion la cual agrega un elemento a la lista de la query1.
 * @param: self: el puntero al TAD a modificar
 * @param: id: el id del sensor con el que esta linkeado el registro
 * @param: name: el nombre del sensor con el que esta linkeado el registro
 * @param: count: la cantidad de registros leidos por ese sensor
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes query1_add(Query1 self, unsigned int id, char *name, unsigned int count);

/*
 * Antes de realizar la matriz, esta funcion llama al funcion que ordena los
 * elementos de la query1. Se ordena utilizando count (de mayor a menor) y en
 * caso de igualdad se desempata por nombre. Luego llama a la funcion que genera
 * la matriz.
 * @param: self: el puntero al TAD a modificar
 * @param: mat: el puntero a la matriz donde quedaran almacenados los datos en
 * forma de matriz de strings
 * @param: rows: donde se almacenara la cantidad final de filas
 * @param: cols: donde se almacenara la cantidad final de columnas
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes query1_tomatrix(Query1 self, Matrix *mat, unsigned int *rows, unsigned int *cols);

/*
 * Libera los recursos usados por la query1.
 * @param: self: el puntero al TAD a ser liberado
 */
void query1_free(Query1 self);

#endif // __QUERY1_H__
