#ifndef __LIST_H__
#define __LIST_H__

#define BLOCK 20

typedef int (*ListCmp)(void *e1, void *e2);
typedef int (*ListIfEqual)(void *e1, void *e2);
typedef char** (*ListToString)(void *e);
typedef int (*ListOrderBy)(void *e1, void *e2);
typedef struct list_adt *List;
typedef char*** Matrix;

List list_new(void);
int list_add(List self, void *elem, ListCmp compare, ListIfEqual ifequal);
void list_begin(List self);
int list_hasnext(List self);
void* list_next(List self);
void list_order(List self, ListOrderBy orderby);
Matrix list_tomatrix(List self, ListToString tostring, unsigned int *rows);
void list_free(List self);
void list_free_matrix(Matrix mat, unsigned int rows, unsigned int cols);

#endif // __LIST_H__
