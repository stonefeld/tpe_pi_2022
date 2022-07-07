#include "config.h"
#include "list.h"
#include "utils.h"

struct node {
	void *elem;
	struct node *tail;
};

struct list_adt {
	struct node *first;
	struct node *iter;
};

static ErrorCodes _list_create_node(struct node **ret, void *elem);

static ErrorCodes
_list_create_node(struct node **ret, void *elem)
{
	*ret = malloc(sizeof(struct node));
	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		free(ret);
		return E_NOMEM;
	}
	(*ret)->elem = elem;
	(*ret)->tail = NULL;
	return NOE;
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

ErrorCodes
list_add(List self, void *elem, ListCmp compare, ListIfEqual ifequal)
{
	ErrorCodes code;
	struct node *ret;

	code = _list_create_node(&ret, elem);

	if (code == NOE) {
		char ready = 0;

		if (self->first == NULL || compare(self->first->elem, elem) < 0) {
			ret->tail = self->first;
			self->first = ret;
			code = I_ADDED;
		} else {
			struct node *aux = self->first;

			while (aux != NULL && !ready) {
				if (compare(aux->elem, elem) == 0) {
					ifequal(aux->elem, elem);
					code = I_UPDATED;
					ready = 1;
				} else if (aux->tail == NULL || compare(aux->tail->elem, elem) < 0) {
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

ErrorCodes
list_tomatrix(List self, Matrix *mat, ListToString tostring, unsigned int *rows)
{
	ErrorCodes code = NOE;
	char **row;
	*rows = 0;

	for (struct node *aux = self->first; aux != NULL && code == NOE; aux = aux->tail) {
		if (*rows % BLOCK == 0) {
			*mat = realloc(*mat, (*rows + BLOCK) * sizeof(char**));
			if (errno == ENOMEM) {
				log_error("No hay memoria suficiente");
				code = E_NOMEM;
			}
		}
		if (code != E_NOMEM) {
			code = tostring(&row, aux->elem);
			(*mat)[(*rows)++] = row;
		}
	}
	*mat = realloc(*mat, *rows * sizeof(char**));

	if (errno == ENOMEM)
		return E_NOMEM;
	return code;
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
