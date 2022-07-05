#include "parser.h"
#include "logger.h"
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
		if (errno == ENOMEM) {
			log_error("No hay suficiente memoria");
		} else {
			ret->id = id;
			ret->tail = self;

			ret->name = malloc(strlen(name) + 1);
			if (errno == ENOMEM)
				log_error("No hay suficiente memoria");
			else
				strcpy(ret->name, name);

			return ret;
		}
	} else {
		self->tail = _sensors_add(self->tail, id, name);
	}
	return self;
}

Sensors
sensors_add(Sensors self, const char *stream)
{
	unsigned int rows;
	char *s = malloc(strlen(stream) + 1);
	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
	} else {
		strcpy(s, stream);
		char **keys = parser_get(s, &rows);

		if (rows != SENSOR_KEYS) {
			log_error("La cantidad de claves leídas en la línea no es correcta.");
		} else {
			unsigned int id = atoi(keys[0]);
			char *name = keys[1];
			char status = keys[2][0];

			if (status == 'A')
				self = _sensors_add(self, id, name);
		}

		free(keys);
	}
	free(s);
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

	if (!exists) {
		name = malloc(1);
		if (errno == ENOMEM) {
			log_error("No hay suficiente memoria");
			return NULL;
		}
		name[0] = '\0';
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
