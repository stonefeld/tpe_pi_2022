#include "config.h"
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

static ErrorCodes _sensors_add(Sensors self, unsigned int id, char *name);
static ErrorCodes _sensors_create_node(struct sensor **ret, unsigned int id, char *name);

static ErrorCodes
_sensors_add(Sensors self, unsigned int id, char *name)
{
	ErrorCodes code;
	struct sensor *ret;

	code = _sensors_create_node(&ret, id, name);

	if (code == NOE) {
		char ready = 0;

		if (self->first == NULL || self->first->id > id) {
			ret->tail = self->first;
			self->first = ret;
			code = I_ADDED;
			ready = 1;
		} else {
			struct sensor *aux = self->first;

			while (aux != NULL && !ready) {
				if (aux->id == id) {
					ready = 1;
				} else if (aux->tail == NULL || aux->tail->id > id) {
					ret->tail = aux->tail;
					aux->tail = ret;
					code = I_ADDED;
					ready = 1;
				}
				aux = aux->tail;
			}
		}

		if (code != I_ADDED)
			free(ret);
	}

	return code;
}

static ErrorCodes
_sensors_create_node(struct sensor **ret, unsigned int id, char *name)
{
	*ret = malloc(sizeof(struct sensor));
	if (errno == ENOMEM)
		return E_NOMEM;

	(*ret)->id = id;
	(*ret)->name = malloc(strlen(name) + 1);
	strcpy((*ret)->name, name);
	(*ret)->tail = NULL;
	return NOE;
}

Sensors
sensors_new()
{
	return calloc(1, sizeof(struct sensors_adt));
}

int
sensors_add(Sensors self, unsigned int id, char *name)
{
	if (_sensors_add(self, id, name))
		return NOE;
	return W_NOTADD;
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
