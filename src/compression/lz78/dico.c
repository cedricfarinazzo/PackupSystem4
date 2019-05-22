#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../struct.h"
#include "../liste/liste.h"
#include "../liste/pylist.h"
#include "dico.h"

struct dico *new_dico(size_t len, size_t taux)
{
    struct dico *Dico = malloc(sizeof(struct dico));
    Dico->letter = malloc(sizeof(unsigned char)* len);
    Dico->vector = malloc(sizeof(ssize_t) * len);
    Dico->len = len;
    Dico->taux = taux;
    return Dico;
}

void extension_dico(struct dico *Dico)
{
    Dico->vector = realloc(Dico->vector, sizeof(ssize_t) * (Dico->len * 2));
    Dico->letter = realloc(Dico->letter, sizeof(unsigned char) *
            (Dico->len * 2));
    Dico->len *= 2;
}

void free_dico(struct dico *Dico)
{
    free(Dico->vector);
    free(Dico->letter);
    free(Dico);
}
