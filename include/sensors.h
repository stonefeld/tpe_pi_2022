#ifndef __SENSORS_H__
#define __SENSORS_H__

#define SENSOR_KEYS 3

typedef struct sensors_adt *Sensors;

Sensors sensors_new(void);
Sensors sensors_add(Sensors self, char *stream);
void sensors_free(Sensors self);

void sensors_print(Sensors self);

#endif // __SENSORS_H__
