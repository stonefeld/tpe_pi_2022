#include "parser.h"
#include "sensors.h"
#include "utils.h"

struct sensors_adt {
	unsigned int id;
	char *name;
	struct sensors_adt *tail;
};

static Sensors
_sensors_add(Sensors self, unsigned int id, char *name)
{
	if (self == NULL || self->id > id) {
		Sensors ret = malloc(sizeof(struct sensors_adt));
		ret->id = id;
		ret->name = malloc(strlen(name) + 1);
		strcpy(ret->name, name);
		ret->tail = self;
		return ret;
	} else {
		self->tail = _sensors_add(self->tail, id, name);
	}
	return self;
}

Sensors
sensors_add(Sensors self, char *stream)
{
	unsigned int rows;
	char **keys = parser_get(stream, &rows);

	if (rows != SENSOR_KEYS)
		printf("Warning: la cantidad de claves leídas en la línea no es correcta.\n");

	unsigned int id = atoi(keys[0]);
	char *name = keys[1];
	char status = keys[2][0];

	if (status == 'A')
		self = _sensors_add(self, id, name);

	free(keys);
	return self;
}

int
sensor_exists(Sensors self, unsigned int id)
{
	int exists = 0;

	for (Sensors aux = self; aux != NULL && !exists; aux = aux->tail)
		if (aux->id == id)
			exists = 1;

	return exists;
}

char*
sensors_get_name(Sensors self, unsigned int id)
{
	char *name;
	int exists = 0;

	// TODO(ts): pensar si conviene pasar el puntero al nombre o hacer una
	// copia del mismo como retorno
	for (Sensors aux = self; aux != NULL && !exists; aux = aux->tail) {
		if (aux->id == id) {
			name = aux->name;
			exists = 1;
		}
	}

	return name;
}

void
sensors_free(Sensors self)
{
	if (self == NULL)
		return;
	sensors_free(self->tail);
	free(self->name);
	free(self);
}

// TODO(ts): borrar al final del proyecto
void
sensors_print(Sensors self)
{
#if DEBUG
	for (Sensors aux = self; aux != NULL; aux = aux->tail)
		printf("%d - %s\n", aux->id, aux->name);
#endif
}
