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

static struct node* _list_add_rec(struct node *self, void *elem, ListCmp compare, ListIfEqual ifequal, char *added);
static void _list_free_rec(struct node *self);

static struct node*
_list_add_rec(struct node *self, void *elem, ListCmp compare, ListIfEqual ifequal, char *added)
{
	int n;
	if (self == NULL || (n = compare(self->elem, elem)) < 0) {
		struct node *ret = malloc(sizeof(struct node));
		if (errno == ENOMEM) {
			log_error("No hay suficiente memoria");
		} else {
			ret->elem = elem;
			ret->tail = self;
			*added = 1;
			return ret;
		}
	} else if (n == 0) {
		ifequal(self->elem, elem);
	} else {
		self->tail = _list_add_rec(self->tail, elem, compare, ifequal, added);
	}
	return self;
}

static void
_list_free_rec(struct node *self)
{
	if (self == NULL)
		return;
	_list_free_rec(self->tail);
	free(self->elem);
	free(self);
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
	char added = 0;
	self->first = _list_add_rec(self->first, elem, compare, ifequal, &added);
	return !added;
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
	_list_free_rec(self->first);
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
