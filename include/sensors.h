#ifndef __SENSORS_H__
#define __SENSORS_H__

#include "logger.h"

typedef struct sensors_adt *Sensors;

/*
 * Crea una lista encadenada de los sensores vacia.
 */
Sensors sensors_new(void);

/*
 * Agrega un elemento a la lista ordenada de sensores, ordenada por id A este
 * elemento se le asigna el id y el nombre pasados como argumentos. En caso de
 * error retorna un ErrorCode y en el caso de no encontrar error retorna NOE.
 */
ErrorCodes sensors_add(Sensors self, unsigned int id, char *name);

/*
 * Retorna 1 si el sensor existe y 0 si no existe, recibiendo el id del sensor
 * a encontrar.
 */
int sensors_exists(Sensors self, unsigned int id);

/*
 * Retorna 1 si el sensor existe y 0 si no existe, recibiendo el id del sensor
 * a encontrar. En caso de que exista devuelve el nombre del sensor en la
 * puntero pasado como argumento (No devuelve una copia del mismo).
 */
int sensors_get_name(Sensors self, unsigned int id, char **name);

/*
 * Libera los recursos usados por la lista ordenada de sensores.
 */
void sensors_free(Sensors self);

#endif // __SENSORS_H__
