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
 */
List list_new(void);

/*
 * Agrega un elemento a la lista. El elemento es de tipo `void*`, pues la misma
 * es generica y para determinar el orden en que se posiciona el nodo se
 * suministra a la funcion con un puntero a funcion llamada `compare` que
 * devuelve menor a 0 si e1 es menor a e2, igual a 0 si son iguales o mayor a 0
 * si e1 es mayor a e2. En caso de ser iguales, se suministra una funcion
 * `ifequal` que determina que accion realizar con los elementos iguales.
 */
ErrorCodes list_add(List self, void *elem, ListCmp compare, ListIfEqual ifequal);

/*
 * Ordena los elementos de la lista bajo un cierto criterio determinado por la
 * funcion pasada como parametro llamada `orderby`.
 */
void list_order(List self, ListOrderBy orderby);

/*
 * Transforma las lista encadenada en una matriz de strings. Por cada nodo de
 * la lista, se utiliza la funcion `tostring` provista por quien implementa la
 * lista para que devuelve los contenidos de los elementos de cada nodo en un
 * vector de string que representara cada fila de la matriz.
 */
ErrorCodes list_tomatrix(List self, Matrix *mat, ListToString tostring, unsigned int *rows);

/*
 * Libera los recursos reservados por la lista.
 */
void list_free(List self);

/*
 * Libera los recursos reservados para la matriz.
 */
void list_free_matrix(Matrix mat, unsigned int rows, unsigned int cols);

#endif // __LIST_H__
