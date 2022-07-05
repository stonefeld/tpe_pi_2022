#ifndef __SENSORS_H__
#define __SENSORS_H__

#define SENSOR_KEYS 3

typedef struct sensors_adt *Sensors;

Sensors sensors_add(Sensors self, char *stream);
int sensor_exists(Sensors self, unsigned int id);
char* sensors_get_name(Sensors self, unsigned int id);
void sensors_free(Sensors self);

// TODO(ts): borrar al final del proyecto
void sensors_print(Sensors self);

#endif // __SENSORS_H__
