#include "config.h"
#include "logger.h"
#include "query1.h"
#include "utils.h"

struct query1 {
	char* name;
	unsigned int id;
	unsigned int count;
};

Query1
query1_new()
{
	return list_new();
}

static int _compare(struct query1 *e1, struct query1 *e2);
static int _ifequal(struct query1 *e1, struct query1 *e2);
static int _orderby(struct query1 *e1, struct query1 *e2);
static char** _tostring(struct query1 *q);

static int
_compare(struct query1 *e1, struct query1 *e2)
{
	return e1->id - e2->id;
}

static int
_ifequal(struct query1 *e1, struct query1 *e2)
{
	e1->count += e2->count;
	return 0;
}

static int
_orderby(struct query1 *e1, struct query1 *e2)
{
	int n = e1->count - e2->count;
	if (n == 0)
		return strcmp(e2->name, e1->name);
	return n;
}

static char**
_tostring(struct query1 *q)
{
	char **ret = malloc(QUERY1_COLS * sizeof(char*));

	ret[0] = malloc(strlen(q->name) + 1);
	ret[1] = malloc(BLOCK);

	strcpy(ret[0], q->name);
	sprintf(ret[1], "%d", q->count);

	return ret;
}

int
query1_add(Query1 self, unsigned int id, char *name, unsigned int count)
{
	struct query1 *q = malloc(sizeof(struct query1));
	int c = 0;

	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		c = 1;
	} else {
		q->name = name;
		q->count = count;
		q->id = id;
		c = list_add( self, q, (ListCmp)_compare, (ListIfEqual)_ifequal);
		if (c == 2) {
			free(q);
			c = 0;
		}
	}

	return c;
}

Matrix
query1_tomatrix(Query1 self, unsigned int *rows, unsigned int *cols)
{
	if (self == NULL)
		return NULL;
	*cols = QUERY1_COLS;
	list_order(self, (ListOrderBy)_orderby);
	return list_tomatrix(self, (ListToString)_tostring, rows);
}

void
query1_free(Query1 self)
{
	list_free(self);
}

void
query1_print(Query1 self)
{
	struct query1 *n;
	list_begin(self);
	while (list_hasnext(self)) {
		n = list_next(self);
		printf("%d\t%.20s\t%d\n", n->id, n->name, n->count);
	}
}
