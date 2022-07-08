#ifndef __LIST_H__
#define __LIST_H__

#include "logger.h"

typedef int (*ListCmp)(void *e1, void *e2);
typedef void (*ListIfEqual)(void *e1, void *e2);
typedef ErrorCodes (*ListToString)(char ***ret, void *e);
typedef int (*ListOrderBy)(void *e1, void *e2);
typedef struct list_adt *List;
typedef char ***Matrix;

/*
 * Devuelve una lista generica vacia.
 * @return: la lista vacia
 */
List list_new(void);

/*
 * Agrega un elemento a la lista. El elemento es de tipo `void*`, pues la misma
 * es generica y para determinar el orden en que se posiciona el nodo se
 * suministra a la funcion con un puntero a funcion llamada `compare` que
 * devuelve menor a 0 si e1 es menor a e2, igual a 0 si son iguales o mayor a 0
 * si e1 es mayor a e2. En caso de ser iguales, se suministra una funcion
 * `ifequal` que determina que accion realizar con los elementos iguales.
 * @param: self: el puntero a la lista a modificar
 * @param: elem: el puntero al elemento a agregar en un nodo
 * @param: compare: el puntero a la funcion que determina el orden en que
 * agregar los nodos
 * @param: ifequal: el puntero a la funcion que determina que accion tomar
 * cuando el elemento a agregar ya existe en la lista
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes list_add(List self, void *elem, ListCmp compare, ListIfEqual ifequal);

/*
 * Ordena los elementos de la lista bajo un cierto criterio determinado por la
 * funcion pasada como parametro llamada `orderby`.
 * @param: self: el puntero a la lista a ordenar
 * @param: orderby: el puntero a la funcion que determina bajo que orden se
 * reordena la lista
 */
void list_order(List self, ListOrderBy orderby);

/*
 * Transforma las lista encadenada en una matriz de strings.
 * @param: self: el puntero a la lista a transformar en matriz
 * @param: mat: el puntero a la matriz donde quedaran almacenados los datos en
 * forma de matriz de strings
 * @param: tostring: el puntero a la funcion que transforma los datos de cada
 * elemento en un vector de strings
 * @param: rows: donde se almacenara la cantidad final de filas
 * @param: cols: donde se almacenara la cantidad final de columnas
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes list_tomatrix(List self, Matrix *mat, ListToString tostring, unsigned int *rows);

/*
 * Libera los recursos reservados por la lista.
 * @param: self: el puntero a la lista a liberar
 */
void list_free(List self);

/*
 * Libera los recursos reservados para la matriz.
 * @param: mat: la matriz a liberar
 * @param: rows: la cantidad de filas que tiene la matriz
 * @param: cols: la cantidad de columnas que tiene la matriz
 */
void list_free_matrix(Matrix mat, unsigned int rows, unsigned int cols);

#endif // __LIST_H__
