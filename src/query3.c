#include "config.h"
#include "logger.h"
#include "query3.h"
#include "utils.h"

struct query3 {
	char day[DAYS_LEN];
	unsigned short nday;
	unsigned int day_count;
	unsigned int night_count;
	unsigned int count;
};

static int _compare(struct query3 *e1, struct query3 *e2);
static void _ifequal(struct query3 *e1, struct query3 *e2);
static ErrorCodes _tostring(char ***ret, struct query3 *q);

static int
_compare(struct query3 *e1, struct query3 *e2)
{
	return e2->nday - e1->nday;
}

static void
_ifequal(struct query3 *e1, struct query3 *e2)
{
	e1->day_count += e2->day_count;
	e1->night_count += e2->night_count;
	e1->count += e2->count;
}

static ErrorCodes
_tostring(char ***ret, struct query3 *q)
{
	*ret = malloc(QUERY3_COLS * sizeof(char*));

	if (errno == ENOMEM)
		return E_NOMEM;

	(*ret)[0] = malloc(DAYS_LEN);
	(*ret)[1] = malloc(BLOCK);
	(*ret)[2] = malloc(BLOCK);
	(*ret)[3] = malloc(BLOCK);

	strcpy((*ret)[0], q->day);
	sprintf((*ret)[1], "%d", q->day_count);
	sprintf((*ret)[2], "%d", q->night_count);
	sprintf((*ret)[3], "%d", q->count);

	return NOE;
}

Query3
query3_new()
{
	return list_new();
}

ErrorCodes
query3_add(Query3 self, char* day, unsigned short nday, unsigned int count, unsigned short time)
{
	if (self == NULL)
		return E_EMPLIST;

	ErrorCodes code;
	struct query3 *q = malloc(sizeof(struct query3));

	if (errno == ENOMEM)
		return E_NOMEM;

	strcpy(q->day, day);
	q->nday = nday;
	q->count = count;

	if (time >= 6 && time < 18) {
		q->day_count = count;
		q->night_count = 0;
	} else {
		q->day_count = 0;
		q->night_count = count;
	}

	code = list_add(self, q, (ListCmp)_compare, (ListIfEqual)_ifequal);

	if (code == I_UPDATED)
		free(q);

	return code;
}

ErrorCodes
query3_tomatrix(Query3 self, Matrix *mat, unsigned int *rows, unsigned int *cols)
{
	if (self == NULL)
		return E_EMPLIST;

	*cols = QUERY3_COLS;
	return list_tomatrix(self, mat, (ListToString)_tostring, rows);
}

void
query3_free(Query3 self)
{
	list_free(self);
}
