#include "logger.h"
#include "parser.h"
#include "readings.h"
#include "utils.h"
#include "query2.h"

struct readings_adt {
	// Query1 query1;
	Query2 query2;
	unsigned int *queries;
	unsigned int count;
};

Readings
readings_new(unsigned int *queries, unsigned int count)
{
	Readings self = calloc(1, sizeof(struct readings_adt));
	self->queries = queries;
	self->count = count;

	for (int i = 0; i < count; i++) {
		switch (queries[i]) {
			// case 1: self->query1 = query1_new(); break;
			case 2: self->query2 = query2_new(); break;
			default: log_warn("Query no definido"); break;
		}
	}

	return self;
}

int
readings_add(Readings self, Sensors sensors, const char *stream)
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

		if (rows != READING_KEYS) {
			log_error("La cantidad de claves leidas en la linea no es correcta");
			c = 1;
		} else {
			unsigned int year = atoi(keys[0]);
			char *wday = keys[3];
			unsigned int id = atoi(keys[4]);
			unsigned int time = atoi(keys[5]);
			unsigned int count = atoi(keys[6]);
			char *name;

			if (sensors_get_name(sensors, id, name)) {
				for (int i = 0; i < self->count; i++) {
					switch (self->queries[i]) {
						// case 1: query1_add(self->query1, id, name, count); break;
						case 2: query2_add(self->query2, year, count); break;
						default: log_warn("La query solicitada no existe. Se saltea"); break;
					}
				}
			}
		}
		free(keys);
	}
	free(s);
	return c;
}

int
readings_free(Readings self)
{
	for (int i = 0; i < self->count; i++) {
		switch (self->queries[i]) {
			case 2: query2_free(self->query2); break;
		}
	}
	free(self);
	return 0;
}

void
readings_print(Readings self)
{
	for (int i = 0; i < self->count; i++) {
		switch (self->queries[i]) {
			case 2: query2_print(self->query2); break;
		}
	}
}
