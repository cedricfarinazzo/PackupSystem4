#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../struct.h"
#include "../liste/liste.h"
#include "../liste/pylist.h"

struct dico *new_dico(size_t len, size_t taux)
{
    struct dico *Dico = malloc(sizeof(struct dico));
    Dico->letter = malloc(sizeof(unsigned char)* len);
    Dico->vector = malloc(sizeof(int) * len);
    Dico->len = len;
    Dico->taux = taux;
    return Dico;
}

void transfert_letter(unsigned char *src, unsigned char *dst, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        dst[i] = src[i];
    }
}

void transfert_vector(size_t *src, size_t *dst, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        dst[i] = src[i];
    }
}

void extension_dico(struct dico *Dico)
{
    size_t *tmp = Dico->vector;
    unsigned char *let = Dico->letter;
    Dico->vector = malloc(sizeof(size_t) * (Dico->len * 2));
    Dico->letter = malloc(sizeof(unsigned char) * (Dico->len * 2));
    Dico->len *= 2;
    transfert_vector(tmp, Dico->vector, Dico->taux);
    transfert_letter(let, Dico->letter, Dico->taux);
    free(tmp);
    free(let);
}

void free_dico(struct dico *Dico)
{
    free(Dico->vector);
    free(Dico->letter);
    free(Dico);
}
