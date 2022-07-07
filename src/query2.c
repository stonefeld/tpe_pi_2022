#include "config.h"
#include "logger.h"
#include "query2.h"
#include "utils.h"

struct query2 {
	unsigned int year;
	unsigned int count;
};

static int _compare(struct query2 *e1, struct query2 *e2);
static void _ifequal(struct query2 *e1, struct query2 *e2);
static ErrorCodes _tostring(char ***ret, struct query2 *q);

static int
_compare(struct query2 *e1, struct query2 *e2)
{
	return e1->year - e2->year;
}

static void
_ifequal(struct query2 *e1, struct query2 *e2)
{
	e1->count += e2->count;
}

static ErrorCodes
_tostring(char ***ret, struct query2 *q)
{
	*ret = malloc(QUERY2_COLS * sizeof(char*));

	if (errno == ENOMEM)
		return E_NOMEM;

	(*ret)[0] = malloc(YEAR_LEN);
	(*ret)[1] = malloc(BLOCK);

	sprintf((*ret)[0], "%d", q->year);
	sprintf((*ret)[1], "%d", q->count);

	return NOE;
}

Query2
query2_new()
{
	return list_new();
}

ErrorCodes
query2_add(Query2 self, unsigned int year, unsigned int count)
{
	ErrorCodes code;
	struct query2 *q = malloc(sizeof(struct query2));

	if (errno == ENOMEM)
		return E_NOMEM;

	q->year = year;
	q->count = count;
	code = list_add(self, q, (ListCmp)_compare, (ListIfEqual)_ifequal);

	if (code == I_UPDATED)
		free(q);

	return code;
}

ErrorCodes
query2_tomatrix(Query2 self, Matrix *mat, unsigned int *rows, unsigned int *cols)
{
	if (self == NULL)
		return E_EMPLIST;

	*cols = QUERY2_COLS;
	return list_tomatrix(self, mat, (ListToString)_tostring, rows);
}

void
query2_free(Query2 self)
{
	list_free(self);
}

void
query2_print(Query2 self)
{
	struct query2 *n;
	list_begin(self);
	while (list_hasnext(self)) {
		n = list_next(self);
		printf("%d\t%d\n", n->year, n->count);
	}
}
