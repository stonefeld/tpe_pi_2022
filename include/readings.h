#ifndef __READINGS_H__
#define __READINGS_H__

#include "queries.h"
#include "sensors.h"

#define READING_KEYS 7

typedef struct readings_adt *Readings;

Readings readings_new(unsigned int *queries, unsigned int count);
ErrorCodes readings_add(Readings self, unsigned int id, char *name, unsigned int year, char *day, unsigned int nday, unsigned int time, unsigned int count);
ErrorCodes readings_get_matrix(Readings self, Matrix *mat, unsigned int query, unsigned int *rows, unsigned int *cols);
void readings_free(Readings self);
void readings_free_matrix(Matrix mat, unsigned int rows, unsigned int cols);

void readings_print(Readings self);

#endif // __READINGS_H__
