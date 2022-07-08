#ifndef __READINGS_H__
#define __READINGS_H__

#include "queries.h"

typedef struct readings_adt *Readings;

/*
 * Crea un conjnto con vacio con los queries especificados.
 * @param: queries: vector indicando los queries que desean ser procesados
 * @param: count: la cantidad de queries a ser procesados
 * @return: un puntero al TAD con los queries a ser procesados
 */
Readings readings_new(unsigned int *queries, unsigned int count);

/*
 * Agrega para cada uno de los queries solicitados al momento de inicializar
 * la variable, entregandole a cada uno de los queries, los datos que les
 * corresponde para procesarlos.
 * @param: self: el puntero al TAD a modificar
 * @param: id: el id del sensor del registro
 * @param: name: el nombre del sensor del registro
 * @param: year: el anio en que fue leido
 * @param: day: el nombre del dia de la semana
 * @param: nday: el numero del dia de la semana
 * @param: time: la hora del dia
 * @param: count: la cantidad de peatores registrados
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes readings_add(Readings self, unsigned int id, char *name, unsigned int year, char *day, unsigned int nday, unsigned int time, unsigned int count);

/*
 * Devuelve los datos de algun query (siempre y cuando haya sido especificada
 * en su inicializacion) en forma de matriz de string, devolviendo ademas la
 * cantidad total de filas y columnas de la misma para poder trabajar con la
 * misma de la manera que se requirea. Para su utilizacion es requisito que
 * `Matrix mat` este inicializada en `NULL`.
 * @param: self: el puntero del TAD que almacena los queries
 * @param: mat: el puntero a la matriz donde quedaran almacenados los datos en
 * forma de matriz de strings
 * @param: query: el numero de query que se quiere recibir como matriz
 * @param: rows: donde se almacenara la cantidad final de filas
 * @param: cols: donde se almacenara la cantidad final de columnas
 * @return: un codigo de error el cual al no haber error es NOE

 */
ErrorCodes readings_get_matrix(Readings self, Matrix *mat, unsigned int query, unsigned int *rows, unsigned int *cols);

/*
 * Libera los recursos reservados por cada query especificada.
 * @param: self: el puntero al TAD a ser liberado
 */
void readings_free(Readings self);

/*
 * Libera los datos reservados durante la construccion de la matriz.
 * @param: mat: la matriz a ser liberada
 * @param: rows: la cantidad de filas que tiene la matriz
 * @param: cols: la cantidad de columnas que tiene la matriz
 */
void readings_free_matrix(Matrix mat, unsigned int rows, unsigned int cols);

#endif // __READINGS_H__
