#include "config.h"
#include "logger.h"
#include "parser.h"
#include "readings.h"
#include "utils.h"

struct readings_adt {
	Query1 query1;
	Query2 query2;
	Query3 query3;
	unsigned int *queries;
	unsigned int count;
};

Readings
readings_new(unsigned int *queries, unsigned int count)
{
	Readings self = calloc(1, sizeof(struct readings_adt));
	self->queries = queries;
	self->count = count;

	// TODO(ts): cambiar para el default a un codigo de error.
	for (int i = 0; i < count; i++) {
		switch (queries[i]) {
			case 1: self->query1 = query1_new(); break;
			case 2: self->query2 = query2_new(); break;
			case 3: self->query3 = query3_new(); break;
			default: log_warn("Query no definido"); break;
		}
	}

	return self;
}

ErrorCodes
readings_add(Readings self, unsigned int id, char *name, unsigned int year, char *day, unsigned int nday, unsigned int time, unsigned int count)
{
	int status = 0;
	for (int i = 0; i < self->count; i++) {
		switch (self->queries[i]) {
			case 1: status = query1_add(self->query1, id, name, count); break;
			case 2: status = query2_add(self->query2, year, count); break;
			case 3: status = query3_add(self->query3, day, nday, count, time); break;
			default: log_warn("La query solicitada no existe. Se saltea"); break;
		}
	}
	return status;
}

ErrorCodes
readings_get_matrix(Readings self, Matrix *mat, unsigned int query, unsigned int *rows, unsigned int *cols)
{
	ErrorCodes code;
	char exists = 0;

	for (int i = 0; i < self->count && !exists; i++)
		if (self->queries[i] == query)
			exists = 1;

	if (!exists)
		return 1;

	switch (query) {
		case 1: code = query1_tomatrix(self->query1, mat, rows, cols); break;
		case 2: code = query2_tomatrix(self->query2, mat, rows, cols); break;
		case 3: code = query3_tomatrix(self->query3, mat, rows, cols); break;
		default: log_warn("El query solicitado para la matriz no existe. Se saltea"); break;
	}
	return code;
}

void
readings_free(Readings self)
{
	for (int i = 0; i < self->count; i++) {
		switch (self->queries[i]) {
			case 1: query1_free(self->query1); break;
			case 2: query2_free(self->query2); break;
			case 3: query3_free(self->query3); break;
		}
	}
	free(self);
}

void
readings_free_matrix(Matrix mat, unsigned int rows, unsigned int cols)
{
	if (mat == NULL)
		return;
	list_free_matrix(mat, rows, cols);
}

void
readings_print(Readings self)
{
	for (int i = 0; i < self->count; i++) {
		switch (self->queries[i]) {
			case 1: query1_print(self->query1); break;
			case 2: query2_print(self->query2); break;
			case 3: query3_print(self->query3); break;
		}
	}
}
