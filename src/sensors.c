#include "parser.h"
#include "logger.h"
#include "sensors.h"
#include "utils.h"

struct sensor {
	unsigned int id;
	char *name;
	struct sensor *tail;
};

struct sensors_adt {
	struct sensor *first;
};

static struct sensor* _sensors_add_rec(struct sensor *self, unsigned int id, char *name, char *added);
static void _sensors_free_rec(struct sensor *self);

static struct sensor*
_sensors_add_rec(struct sensor *self, unsigned int id, char *name, char *added)
{
	if (self == NULL || self->id > id) {
		struct sensor *ret = malloc(sizeof(struct sensor));
		if (errno == ENOMEM) {
			log_error("No hay suficiente memoria");
		} else {
			ret->id = id;
			ret->tail = self;

			ret->name = malloc(strlen(name) + 1);
			if (errno == ENOMEM)
				log_warn("No hay suficiente memoria para guardar el nombre");
			else
				strcpy(ret->name, name);

			*added = 1;

			return ret;
		}
	} else {
		self->tail = _sensors_add_rec(self->tail, id, name, added);
	}
	return self;
}

static void
_sensors_free_rec(struct sensor *self)
{
	if (self == NULL)
		return;
	_sensors_free_rec(self->tail);
	free(self->name);
	free(self);
}

Sensors
sensors_new()
{
	return calloc(1, sizeof(struct sensors_adt));
}

int
sensors_add(Sensors self, const char *stream)
{
	char *s = malloc(strlen(stream) + 1);
	int c = 0;
	unsigned int rows;

	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		c = 1;
	} else {
		strcpy(s, stream);
		char **keys = parser_get(s, &rows);

		if (rows != SENSOR_KEYS) {
			log_error("La cantidad de claves leídas en la línea no es correcta");
			c = 1;
		} else {
			unsigned int id = atoi(keys[0]);
			char *name = keys[1];
			char status = keys[2][0];

			if (status == 'A') {
				char added = 0;
				self->first = _sensors_add_rec(self->first, id, name, &added);
				if (!added)
					c = 1;
			}
		}
		free(keys);
	}
	free(s);
	return c;
}

int
sensor_exists(Sensors self, unsigned int id)
{
	int exists = 0;

	for (struct sensor *aux = self->first; aux != NULL && !exists; aux = aux->tail)
		if (aux->id == id)
			exists = 1;

	return exists;
}

int
sensors_get_name(Sensors self, unsigned int id, char *name)
{
	int exists = 0;

	// TODO(ts): pensar si conviene pasar el puntero al nombre o hacer una
	// copia del mismo como retorno
	for (struct sensor *aux = self->first; aux != NULL && !exists; aux = aux->tail) {
		if (aux->id == id) {
			name = aux->name;
			exists = 1;
		}
	}

	return 0;
}

int
sensors_free(Sensors self)
{
	_sensors_free_rec(self->first);
	free(self);
	return 0;
}

// TODO(ts): borrar al final del proyecto
void
sensors_print(Sensors self)
{
#if DEBUG
	for (struct sensor *aux = self->first; aux != NULL; aux = aux->tail)
		printf("%d - %s\n", aux->id, aux->name);
#endif
}
