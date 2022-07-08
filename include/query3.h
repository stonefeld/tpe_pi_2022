#ifndef __QUERY3_H__
#define __QUERY3_H__

#include "list.h"

typedef List Query3;

/*
 * Crea una lista encadenada de los datos de la query3 vacia.
 */
Query3 query3_new(void);

/*
 * Llama a una funcion la cual agrega ordenadamente un elemento a la lista de
 * la query3, pasando como argumentos funcion de comparacion y funcion de
 * igualdad A este elemento se le asigna el nombre, el numero de dia, el count
 * y la hora pasados como argumentos. El numero de dia es 0 si es lunes, 1 si es
 * martes, 2 si es miercoles, etc. Dependiendo de la hora del dia el count se
 * agrega a el contador de dia o el contador de noche En caso de error retorna
 * un ErrorCode y en el caso de no encontrar error retorna NOE.
 */
ErrorCodes query3_add(Query3 self, char *day, unsigned short nday, unsigned int count, unsigned short time);

/*
 * Llama a la funcion que genera la matriz. En caso de error retorna un
 * ErrorCode y en el caso de no encontrar error retorna NOE.
 */
ErrorCodes query3_tomatrix(Query3 self, Matrix *mat, unsigned int *rows, unsigned int *cols);

/*
 * Libera los recursos usados por la query3.
 */
void query3_free(Query3 self);

#endif // __QUERY3_H__
