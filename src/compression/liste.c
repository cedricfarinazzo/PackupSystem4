#include <err.h>
#include <stdlib.h>

#include "liste.h"

struct liste *new_liste() {
    struct liste *l_new = malloc(sizeof(struct liste));
    if (l_new != NULL)
    {
        l_new->premier = NULL;
        l_new->dernier = NULL;
    }
    else
    {
        errx(1, "No free memory !");
    }
    return l_new;
}

void insert(struct liste *liste, char n)
{
    struct element n_ele = malloc(sizeof(struct element));
    n_ele->key = n;

    if (liste->next == NULL)
    {
        n_ele->prec = NULL;
        n_ele->next = NULL;
        liste->first = n_ele;
        liste->last = n_ele;
    }
    else
    {
        n_ele->prec = liste->last;
        n_ele->next = NULL;
        liste->last->next = n_ele;
        liste->last = n_ele;
    }
}

void liste_free(struct liste *liste)
{
    if (liste->premier != NULL)
    {
        liste_free(liste->premier);
    }
    free(liste);
}
