#include "list.h"
#include "logger.h"
#include "utils.h"

struct node {
	void *elem;
	struct node *tail;
};

struct list_adt {
	struct node *first;
	struct node *iter;
};

static struct node* _list_create_node(void *elem);

static struct node*
_list_create_node(void *elem)
{
	struct node *ret = malloc(sizeof(struct node));
	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		free(ret);
		return NULL;
	}
	ret->elem = elem;
	ret->tail = NULL;
	return ret;
}

List
list_new()
{
	List ret = calloc(1, sizeof(struct list_adt));
	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		return NULL;
	}
	return ret;
}

int
list_add(List self, void *elem, ListCmp compare, ListIfEqual ifequal)
{
	char status = 0;

	struct node *ret = _list_create_node(elem);
	if (self->first == NULL || compare(self->first->elem, elem) < 0) {
		ret->tail = self->first;
		self->first = ret;
		status = 1;
	} else {
		struct node *aux = self->first;
		while (aux != NULL && !status) {
			if (compare(aux->elem, elem) == 0) {
				ifequal(aux->elem, elem);
				status = 2;
			} else if (aux->tail == NULL || compare(aux->tail->elem, elem) < 0) {
				ret->tail = aux->tail;
				aux->tail = ret;
				status = 1;
			}
			aux = aux->tail;
		}
	}

	if (status != 1)
		free(ret);

	return status;
}

void
list_begin(List self)
{
	self->iter = self->first;
}

int
list_hasnext(List self)
{
	return self->iter != NULL;
}

void*
list_next(List self)
{
	void *e = self->iter->elem;
	self->iter = self->iter->tail;
	return e;
}

void
list_order(List self, ListOrderBy orderby)
{
}

Matrix
list_tomatrix(List self, ListToString tostring, unsigned int *rows)
{
	Matrix mat = NULL;
	char error = 0;
	*rows = 0;

	for (struct node *aux = self->first; aux != NULL && !error; aux = aux->tail) {
		if (*rows % BLOCK == 0) {
			mat = realloc(mat, (*rows + BLOCK) * sizeof(char**));
			if (errno == ENOMEM) {
				log_error("No hay memoria suficiente");
				error = 0;
			}
		}
		if (!error)
			mat[(*rows)++] = tostring(aux->elem);
	}
	mat = realloc(mat, *rows * sizeof(char**));

	return mat;
}

void
list_free(List self)
{
	struct node *aux;
	while (self->first != NULL) {
		aux = self->first;
		self->first = self->first->tail;
		free(aux->elem);
		free(aux);
	}
	free(self);
}

void
list_free_matrix(Matrix mat, unsigned int rows, unsigned int cols)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			free(mat[i][j]);
		free(mat[i]);
	}
	free(mat);
}
