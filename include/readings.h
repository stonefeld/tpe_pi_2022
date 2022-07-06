#ifndef __READINGS_H__
#define __READINGS_H__

#include "sensors.h"

#define READING_KEYS 7

typedef struct readings_adt *Readings;

Readings readings_new(unsigned int *queries, unsigned int count);
int readings_add(Readings self, Sensors sensors, const char *stream);
int readings_free(Readings self);
void readings_print(Readings self);

#endif // __READINGS_H__
