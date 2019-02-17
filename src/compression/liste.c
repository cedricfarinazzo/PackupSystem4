#include <err.h>
#include <stdlib.h>
#include "huffman.h"
#include "liste.h"
#include <string.h>

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

void insertr(struct liste *liste, char *n)
{
    int len = strlen(n);
    for (int i = 0; i < len; i++)
    {
        insert(liste, n[i]);
    }
}

char *liste_to_string(struct liste *liste)
{
    int len = len_list(liste);
    char *output = malloc(sizeof((char) * len));
    struct element *actual = liste->first;
    int i = 0;
    while (actual != NULL)
    {
        output[i] = actual->key;
        i++;
        actual = actual->next;
    }
    free(actual);
    return output;
}

void del_in(struct element *ele)
{
    if (ele == NULL)
    {
        errx(2, "Can not free Null element");
    }
    if (ele->next == NULL)
    {
        ele->prec->next = ele->next;
    }
    if (ele->prec == NULL)
    {
        ele->next->prec = ele->prec;
    }
    free(ele);
}

struct huffele *min_pop(struct freqlist *Freq)
{
    struct element *f_fr = Freq->freq->first;
    struct element *f_car = Freq->car->first;
    struct element *m_fr = Freq->freq->first;
    struct element *m_car = Freq->car->first;
    while (f_fr->next != NULL && f_car->next != NULL)
    {
        if (f_fr->key < m_fr->key)
        {
            m_fr = f_fr->prec->next;
            m_car = f_car->prec->next;
        }
        f_fr = f_fr->next;
        f_car = f_car->next;
    }
    struct huffele *output = malloc(sizeof(struct huffele));
    output->car = m_car->key;
    output->freq = m_fr->key;
    del_in(m_fr);
    del_in(m_car);
    free(f_fr);
    free(f_car);
    return output;
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
