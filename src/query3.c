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
	return e1->nday - e2->nday;
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
	*ret = malloc(QUERY3_COLS*sizeof(char*));
	
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
	ErrorCodes code;
	struct query3 *q = malloc(sizeof(struct query3));
	
	if (errno == ENOMEM)
		return E_NOMEM;
	
	strcpy(q->day, day);
	q->count = count;
	q->nday = nday;
	
	if (time >= 6 && time <= 18){
		q->day_count = day_count;
	}else{
		q->night_count = night_count;
	}
	
	code = list_add(self, q, (ListCmp)_compare, (ListIfEqual)_ifequal);
	
	if(code == I_UPDATED)
		free(q);
	
	return q;
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

void
query3_print(Query3 self)
{
	struct query3 *n;
	list_begin(self);
	while (list_hasnext(self)) {
		n = list_next(self);
		printf("%.10s\t%d\t%d\t%d\n", n->day, n->day_count, n->night_count, n->count);
	}
}
