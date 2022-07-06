#ifndef __SENSORS_H__
#define __SENSORS_H__

#define SENSOR_KEYS 3

typedef struct sensors_adt *Sensors;

Sensors sensors_new(void);
int sensors_add(Sensors self, const char *stream);
int sensors_exists(Sensors self, unsigned int id);
int sensors_get_name(Sensors self, unsigned int id, char *name);
void sensors_free(Sensors self);

// TODO(ts): borrar al final del proyecto
void sensors_print(Sensors self);

#endif // __SENSORS_H__
