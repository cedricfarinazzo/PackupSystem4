#include <err.h>
#include <stdlib.h>

#include "liste.h"

struct liste *new_liste() {
    struct liste *l_new = malloc(sizeof(struct liste));
    if (l_new != NULL)
    {
        l_new->first = NULL;
        l_new->last = NULL;
    }
    else
    {
        errx(1, "No free memory !");
    }
    return l_new;
}

int len_list(struct liste *liste)
{
    if (liste->first == NULL)
    {
        return 0;
    }
    int len = 1;
    struct element *actuel = liste->first;
    while (actuel->next != NULL)
    {
        len++;
        actuel = actuel->next;
    }
    free(actuel);
    return len;
}

void insert(struct liste *liste, char n)
{
    struct element *n_ele = malloc(sizeof(struct element));
    n_ele->key = n;

    if (liste->first == NULL)
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

void element_free(struct element *ele)
{
    if (ele->next != NULL)
    {
        element_free(ele->next);
    }
    free(ele);
}

void liste_free(struct liste *liste)
{
    if (liste->first != NULL)
    {
        element_free(liste->first);
    }
    free(liste);
}
