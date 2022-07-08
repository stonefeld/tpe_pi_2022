#include "config.h"
#include "logger.h"
#include "query1.h"
#include "utils.h"

struct query1 {
	char* name;
	unsigned int id;
	unsigned int count;
};

static int _compare(struct query1 *e1, struct query1 *e2);
static void _ifequal(struct query1 *e1, struct query1 *e2);
static int _orderby(struct query1 *e1, struct query1 *e2);
static ErrorCodes _tostring(char ***ret, struct query1 *q);

static int
_compare(struct query1 *e1, struct query1 *e2)
{
	return e1->id - e2->id;
}

static void
_ifequal(struct query1 *e1, struct query1 *e2)
{
	e1->count += e2->count;
}

static int
_orderby(struct query1 *e1, struct query1 *e2)
{
	int n = e1->count - e2->count;
	if (n == 0)
		return strcmp(e2->name, e1->name);
	return n;
}

static ErrorCodes
_tostring(char ***ret, struct query1 *q)
{
	*ret = malloc(QUERY1_COLS * sizeof(char*));

	if (errno == ENOMEM)
		return E_NOMEM;

	(*ret)[0] = malloc(strlen(q->name) + 1);
	(*ret)[1] = malloc(BLOCK);

	strcpy((*ret)[0], q->name);
	sprintf((*ret)[1], "%d", q->count);

	return NOE;
}

Query1
query1_new()
{
	return list_new();
}

ErrorCodes
query1_add(Query1 self, unsigned int id, char *name, unsigned int count)
{
	ErrorCodes code;
	struct query1 *q = malloc(sizeof(struct query1));

	if (errno == ENOMEM)
		return E_NOMEM;

	q->name = name;
	q->count = count;
	q->id = id;
	code = list_add( self, q, (ListCmp)_compare, (ListIfEqual)_ifequal);

	if (code == I_UPDATED)
		free(q);

	return code;
}

ErrorCodes
query1_tomatrix(Query1 self, Matrix *mat, unsigned int *rows, unsigned int *cols)
{
	if (self == NULL)
		return E_EMPLIST;

	*cols = QUERY1_COLS;
	list_order(self, (ListOrderBy)_orderby);
	return list_tomatrix(self, mat, (ListToString)_tostring, rows);
}

void
query1_free(Query1 self)
{
	list_free(self);
}
