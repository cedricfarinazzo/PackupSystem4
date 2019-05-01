#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dico.h"
#include "../struct.h"
#include "../liste/liste.h"

struct dico *new_dico(ssize_t len)
{
    struct dico *Dico = malloc(sizeof(struct dico));
    Dico->letter = malloc(sizeof(unsigned char * len));
    Dico->vector = malloc(sizeof(int * len));
    Dico->len = len;
    Dico->taux = 0;
    return Dico;
}

void transfert(void *src, void *dst, ssize_t len)
{
    for (ssize_t i = 0; i < len; ++i)
    {
        dst[i] = src[i];
    }
}

void extension_dico(struct dico *Dico)
{
    ssize_t *tmp = Dico->vector;
    unsigned char *let = Dico->letter;
    Dico->vector = malloc(sizeof(int * (len * 2)));
    Dico->letter = malloc(sizeof(unsigned char * (len * 2)));
    Dico->len *= 2;
    transfert(tmp, Dico->vector, Dico->taux);
    transfert(let, Dico->letter, Dico->taux);
    free(tmp);
    free(let);
}

void free_dico(struct dico *Dico)
{
    free(Dico->vector);
    free(Dico->letter);
    free(Dico);
}
