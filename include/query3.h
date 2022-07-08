#ifndef __QUERY3_H__
#define __QUERY3_H__

#include "list.h"

typedef List Query3;

/*
 * Crea una lista encadenada de los datos de la query3 vacia.
 * @return: la lista de la query vacia
 */
Query3 query3_new(void);

/*
 * Llama a una funcion la cual agrega un elemento a la lista de la query3.
 * @param: self: el puntero al TAD a modificar
 * @param: day: el nombre del dia en que se leyo tal registro
 * @param: nday: el numero de orden del dia en la semana. En situaciones normales
 * iria del 0 al 6 siendo 0 lunes y 6 domingo
 * @param: count: la cantidad de registros leidos por ese sensor en ese dia
 * especifico
 * @param: time: la hora del dia en que el registro fue tomado
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes query3_add(Query3 self, char *day, unsigned short nday, unsigned int count, unsigned short time);

/*
 * Llama a la funcion que genera la matriz.
 * @param: self: el puntero al TAD a modificar
 * @param: mat: el puntero a la matriz donde quedaran almacenados los datos en
 * forma de matriz de strings
 * @param: rows: donde se almacenara la cantidad final de filas
 * @param: cols: donde se almacenara la cantidad final de columnas
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes query3_tomatrix(Query3 self, Matrix *mat, unsigned int *rows, unsigned int *cols);

/*
 * Libera los recursos usados por la query3.
 * @param: self: el puntero al TAD a ser liberado
 */
void query3_free(Query3 self);

#endif // __QUERY3_H__
