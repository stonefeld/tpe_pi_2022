#ifndef __SENSORS_H__
#define __SENSORS_H__

#include "logger.h"

typedef struct sensors_adt *Sensors;

/*
 * Crea una lista encadenada de los sensores vacia.
 * @return: la lista de sensores
 */
Sensors sensors_new(void);

/*
 * Agrega un elemento a la lista ordenada de sensores, ordenada por id. A este
 * elemento se le asigna el id y el nombre pasados como argumentos.
 * @param: self: el puntero al TAD a ser modificado
 * @param: id: el id del sensor
 * @param: name: el nombre del sensor
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes sensors_add(Sensors self, unsigned int id, char *name);

/*
 * Genera dentro del TAD un vector donde cada indice representa el vector. Los
 * vectores existentes ocuparan un espacio mientras que los que no quedaran en
 * NULL. Es posible no utilizar la funcion, y las funciones de busqueda
 * seguiran funcionando de la manera tradicional de busqueda de listas.
 * @param: self: el puntero al TAD a ser modificado
 * @return: un codigo de error el cual al no haber error es NOE
 */
ErrorCodes sensors_order(Sensors self);

/*
 * Retorna 1 si el sensor existe y 0 si no existe, recibiendo el id del sensor
 * a encontrar. En caso de que exista devuelve el nombre del sensor en la
 * puntero pasado como argumento (No devuelve una copia del mismo).
 * @param: self: el puntero al TAD donde buscar el nombre
 * @param: id: el id del sensor buscado
 * @param: name: el puntero al puntero donde se guarda el nombre
 * @return: 1 si encuentra el sensor, 0 sino
 */
int sensors_get_name(Sensors self, unsigned int id, char **name);

/*
 * Libera los recursos usados por la lista ordenada de sensores.
 * @param: self: el puntero al TAD a liberar
 */
void sensors_free(Sensors self);

#endif // __SENSORS_H__
