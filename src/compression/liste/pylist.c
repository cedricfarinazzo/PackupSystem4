#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "../struct.h"

struct pylist *new_py(void)
{
    struct pylist *newpylist = malloc(sizeof(struct pylist));
    if (newpylist == NULL)
        errx(EXIT_FAILURE, "No free memory");
    newpylist->begin = NULL;
    newpylist->end = NULL;
    newpylist->len = 0;
    return newpylist;
}

void addpy(struct pylist *py, size_t element)
{
    if (py->end == NULL)
    {
        py->end = malloc(sizeof(struct pyele));
        py->end->next = NULL;
        py->end->key = element;
        py->end->prec = NULL;
        py->begin = py->end;
    }
    py->end->next = malloc(sizeof(struct pyele));
    py->end->next->next = NULL;
    py->end->next->key = element;
    py->end->next->prec = py->end;
    py->end = py->end->next;
    py->len += 1;
}

void freeele(struct pyele *pye)
{
    if (pye->next != NULL)
        freeele(pye->next);
    free(pye);
}

void freepy(struct pylist *py)
{
    if (py->begin != NULL)
        freeele(py->begin);
    free(py);
}

