#include "config.h"
#include "logger.h"
#include "parser.h"
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

static int _sensors_add(Sensors self, unsigned int id, char *name);
static struct sensor* _sensors_create_node(unsigned int id, char *name);

static int
_sensors_add(Sensors self, unsigned int id, char *name)
{
	char added = 0;

	struct sensor *ret = _sensors_create_node(id, name);
	if (self->first == NULL || self->first->id < id) {
		ret->tail = self->first;
		self->first = ret;
		added = 1;
	} else {
		struct sensor *aux = self->first;
		while (aux != NULL && !added) {
			if (aux->id > 0) {
				ret->tail = aux->tail;
				aux->tail = ret;
				added = 1;
			}
			aux = aux->tail;
		}
	}

	if (!added)
		free(ret);

	return added;
}

static struct sensor*
_sensors_create_node(unsigned int id, char *name)
{
	struct sensor *ret = malloc(sizeof(struct sensor));
	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		free(ret);
		return NULL;
	}
	ret->id = id;
	ret->name = malloc(strlen(name) + 1);
	strcpy(ret->name, name);
	ret->tail = NULL;
	return ret;
}

Sensors
sensors_new()
{
	return calloc(1, sizeof(struct sensors_adt));
}

int
sensors_add(Sensors self, const char *stream)
{
	char s[BUF_SIZE];
	int c = 0;
	unsigned int rows;

	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		c = 1;
	} else {
		strcpy(s, stream);
		char **keys = parser_get(s, &rows);

		if (rows != SENSOR_KEYS) {
			log_error("La cantidad de claves leidas en la linea no es correcta");
			c = 1;
		} else {
			unsigned int id = atoi(keys[0]);
			char *name = keys[1];
			char status = keys[2][0];

			if (status == 'A')
				_sensors_add(self, id, name);
		}
		free(keys);
	}
	return c;
}

int
sensors_exists(Sensors self, unsigned int id)
{
	int exists = 0;

	for (struct sensor *aux = self->first; aux != NULL && !exists; aux = aux->tail)
		if (aux->id == id)
			exists = 1;

	return exists;
}

int
sensors_get_name(Sensors self, unsigned int id, char **name)
{
	int exists = 0;

	// TODO(ts): pensar si conviene pasar el puntero al nombre o hacer una
	// copia del mismo como retorno
	for (struct sensor *aux = self->first; aux != NULL && !exists; aux = aux->tail) {
		if (aux->id == id) {
			*name = aux->name;
			exists = 1;
		}
	}

	return exists;
}

void
sensors_free(Sensors self)
{
	struct sensor *aux;
	while (self->first != NULL) {
		aux = self->first;
		self->first = self->first->tail;
		free(aux->name);
		free(aux);
	}
	free(self);
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
