#ifndef __READINGS_H__
#define __READINGS_H__

#include "queries.h"

typedef struct readings_adt *Readings;

/*
 * Crea un conjnto con vacio con los queries especificados. El parametro
 * queries debe ser un vector con los queries que desean ser procesados por
 * cada lectura añadida. El segundo parametro es la cantidad de queries a
 * procesar.
 */
Readings readings_new(unsigned int *queries, unsigned int count);

/*
 * Agrega para cada uno de los queries solicitados al momento de inicializar
 * la variable, entregandole a cada uno de los queries, los datos que les
 * corresponde para procesarlos.
 */
ErrorCodes readings_add(Readings self, unsigned int id, char *name, unsigned int year, char *day, unsigned int nday, unsigned int time, unsigned int count);

/*
 * Devuelve los datos de algun query (siempre y cuando haya sido especificada
 * en su inicializacion) en forma de matriz de string, devolviendo ademas la
 * cantidad total de filas y columnas de la misma para poder trabajar con la
 * misma de la manera que se requirea.
 */
ErrorCodes readings_get_matrix(Readings self, Matrix *mat, unsigned int query, unsigned int *rows, unsigned int *cols);

/*
 * Libera los recursos reservados por cada query especificada.
 */
void readings_free(Readings self);

/*
 * Libera los datos reservados durante la construccion de la matriz.
 */
void readings_free_matrix(Matrix mat, unsigned int rows, unsigned int cols);

#endif // __READINGS_H__
