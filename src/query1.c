#include "logger.h"
#include "parser.h"
#include "sensors.h"
#include "readings.h"
#include "utils.h"
#include "query1.h"

#define QUERY1_COLS 2

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
	free(e2);
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

	ret[0] = malloc(strlen(q->name));
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
	}

	return c;
}

void
query1_free(Query1 self)
{
	list_free(self);
}

Matrix
query1_to_matrix(Query1 self, unsigned int *rows)
{
	if ( self == NULL )
		return NULL;
	list_order(self, (ListOrderBy)_orderby);
	return list_tomatrix(self, (ListToString)_tostring, rows);
}
