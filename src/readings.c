#include "config.h"
#include "logger.h"
#include "parser.h"
#include "readings.h"
#include "utils.h"

struct readings_adt {
	Query1 query1;
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
			case 1: self->query1 = query1_new(); break;
			case 2: self->query2 = query2_new(); break;
			default: log_warn("Query no definido"); break;
		}
	}

	return self;
}

int
readings_add(Readings self, Sensors sensors, const char *stream)
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

		if (rows != READING_KEYS) {
			log_error("La cantidad de claves leidas en la linea no es correcta");
			c = 1;
		} else {
			unsigned int id = atoi(keys[4]);
			char *name;

			if (sensors_get_name(sensors, id, &name)) {
				unsigned int year = atoi(keys[0]);
				char *wday = keys[3];
				unsigned int time = atoi(keys[5]);
				unsigned int count = atoi(keys[6]);

				for (int i = 0; i < self->count; i++) {
					switch (self->queries[i]) {
						case 1: query1_add(self->query1, id, name, count); break;
						case 2: query2_add(self->query2, year, count); break;
						default: log_warn("La query solicitada no existe. Se saltea"); break;
					}
				}
			}
		}
		free(keys);
	}
	return c;
}

void
readings_free(Readings self)
{
	for (int i = 0; i < self->count; i++) {
		switch (self->queries[i]) {
			case 1: query1_free(self->query1); break;
			case 2: query2_free(self->query2); break;
		}
	}
	free(self);
}

void
readings_free_matrix(Matrix mat, unsigned int rows, unsigned int cols)
{
	list_free_matrix(mat, rows, cols);
}

Matrix
readings_get_matrix(Readings self, unsigned int query, unsigned int *rows, unsigned int *cols)
{
	Matrix ret;
	switch (query) {
		case 1: ret = query1_tomatrix(self->query1, rows, cols); break;
		case 2: ret = query2_tomatrix(self->query2, rows, cols); break;
		default: log_warn("El query solicitado para la matriz no existe. Se saltea"); break;
	}
	return ret;
}

void
readings_print(Readings self)
{
	for (int i = 0; i < self->count; i++) {
		switch (self->queries[i]) {
			case 1: query1_print(self->query1); break;
			case 2: query2_print(self->query2); break;
		}
	}
}
