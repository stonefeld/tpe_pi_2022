#ifndef __LIST_H__
#define __LIST_H__

#include "logger.h"

typedef int (*ListCmp)(void *e1, void *e2);
typedef void (*ListIfEqual)(void *e1, void *e2);
typedef ErrorCodes (*ListToString)(char ***ret, void *e);
typedef int (*ListOrderBy)(void *e1, void *e2);
typedef struct list_adt *List;
typedef char ***Matrix;

List list_new(void);
ErrorCodes list_add(List self, void *elem, ListCmp compare, ListIfEqual ifequal);
void list_begin(List self);
int list_hasnext(List self);
void* list_next(List self);
void list_order(List self, ListOrderBy orderby);
ErrorCodes list_tomatrix(List self, Matrix *mat, ListToString tostring, unsigned int *rows);
void list_free(List self);
void list_free_matrix(Matrix mat, unsigned int rows, unsigned int cols);

#endif // __LIST_H__
