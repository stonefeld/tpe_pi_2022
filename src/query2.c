#include "list.h"
#include "logger.h"
#include "parser.h"
#include "query2.h"
#include "readings.h"
#include "sensors.h"
#include "utils.h"
#include "query2.h"

#define QUERY2_COLS 2
#define YEAR_LEN 5

struct query2 {
	unsigned int year;
	unsigned int count;
};

static int _compare(struct query2 *e1, struct query2 *e2);
static int _ifequal(struct query2 *e1, struct query2 *e2);
static char** _tostring(struct query2 *q);

static int
_compare(struct query2 *e1, struct query2 *e2)
{
	return e1->year - e2->year;
}

static int
_ifequal(struct query2 *e1, struct query2 *e2)
{
	e1->count += e2->count;
	free(e2);
	return 0;
}

static char**
_tostring(struct query2 *q)
{
	char **ret = malloc(QUERY2_COLS * sizeof(char*));

	ret[0] = malloc(YEAR_LEN);
	ret[1] = malloc(BLOCK);

	sprintf(ret[0], "%d", q->year);
	sprintf(ret[1], "%d", q->count);

	return ret;
}

Query2
query2_new()
{
	return list_new();
}

int
query2_add(Query2 self, unsigned int year, unsigned int count)
{
	struct query2 *q = malloc(sizeof(struct query2));
	int c = 0;

	if (errno == ENOMEM) {
		log_error("No hay suficiente memoria");
		c = 1;
	} else {
		q->year = year;
		q->count = count;
		c = list_add(self, q, (ListCmp)_compare, (ListIfEqual)_ifequal);
	}
	return c;
}

void
query2_free(Query2 self)
{
	list_free(self);
}

Matrix
query2_to_matrix(Query2 self, unsigned int *rows)
{
	if (self == NULL)
		return NULL;
	return list_tomatrix(self, (ListToString)_tostring, rows);
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
