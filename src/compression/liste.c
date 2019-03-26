#include <err.h>
#include <stdlib.h>
#include "huffman.h"
#include "liste.h"
#include <string.h>
#include "struct.h"
#include <stdio.h>


struct liste *new_liste()
{
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
    return len;
}

void insert(struct liste *liste, char n)
{
    //struct element *n_ele = malloc(sizeof(struct element));
    //n_ele->key = n;

    if (liste->first == NULL)
    {
        //n_ele->prec = NULL;
        //n_ele->next = NULL;
        liste->first = malloc(sizeof(struct element));
        liste->first->key = n;
        liste->first->prec = NULL;
        liste->first->next = NULL;
        //liste->first = n_ele;
        liste->last = liste->first;
    }
    else
    {
        liste->last->next = malloc(sizeof(struct element));
        liste->last->next->key = n;
        liste->last->next->next = NULL;
        liste->last->next->prec = liste->last;
        liste->last = liste->last->next;
    }
}

void insertr(struct liste *Liste, struct liste *n)
{
    struct element *actuel = n->first;
    while(actuel != NULL)
    {
        insert(Liste, actuel->key);
        actuel = actuel->next;
    }
}

void inserts(struct liste *Liste, struct liste *n, int c)
{
    int i = 1;
    struct element *actual = n->first;
    while (i != c && actual != NULL)
    {
        i++;
        actual = actual->next;
    }
    while (actual != NULL)
    {
        insert(Liste, actual->key);
        actual = actual->next;
    }
}

void del_last(struct liste *Liste)
{
    if (Liste->last == NULL)
    {
        errx(4, "del_first : List empty");
    }
    if (Liste->last != Liste->first)
    {
        struct element *tmp = Liste->last;
        Liste->last = Liste->last->prec;
    //if (Liste->last->prec != NULL)
    //{
        Liste->last->next = NULL;
    //}
        free(tmp);
    }
}

void liste_to_string(struct liste *liste, unsigned char *output)
{
    struct element *actual = liste->first;
    int i = 0;
    while (actual != NULL)
    {
        output[i] = actual->key;
        actual = actual->next;
        ++i;
    }
}

void del_in(struct element *ele)
{
    if (ele == NULL)
    {
        errx(2, "Can not free Null element");
    }
    if (ele->next == NULL)
    {
        if (ele->prec != NULL)            
        {
            ele->prec->next = NULL;
        }
    }
    else 
    {
        if (ele->prec == NULL)
        {
            ele->next->prec = NULL;
        }
        else
        {
            ele->prec->next = ele->next;
            ele->next->prec = ele->prec;
        }
    }
    free(ele);
}

void del_inT(struct ele_int *ele)
{
    if (ele == NULL)
        errx(2, "Can not free Null element");
    if (ele->next == NULL)
    {
        if (ele->prec != NULL)
            ele->prec->next = NULL;
    }
    else
    {
        if (ele->prec == NULL)
        {
            ele->next->prec = NULL;
        }
        else
        {
            ele->prec->next = ele->next;
            ele->next->prec = ele->prec;
        }
    }
    free(ele);
}

void min_pop(struct freqlist *Freq, struct huffele *output)
{
    struct ele_int *f_fr = Freq->freq->first;
    struct element *f_car = Freq->car->first;
    struct ele_int *m_fr = Freq->freq->first;
    struct element *m_car = Freq->car->first;
    while (f_fr != NULL && f_car != NULL)
    {
        if (f_fr->key < m_fr->key)
        {
            m_fr = f_fr->prec->next;
            m_car = f_car->prec->next;
        }
        f_fr = f_fr->next;
        f_car = f_car->next;
    }
    //struct huffele *output = malloc(sizeof(struct huffele));
    output->car = m_car->key;
    output->freq = m_fr->key;
    if (m_fr->next == NULL)
    {
        Freq->freq->last = Freq->freq->last->prec;
        Freq->car->last = Freq->car->last->prec;
    }
    if (m_fr->prec == NULL)
    {
        Freq->freq->first = Freq->freq->first->next;
        Freq->car->first = Freq->car->first->next;
    }
    del_inT(m_fr);
    del_in(m_car);
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

//List_int

struct list_int *new_list()
{
    struct list_int *newlist = malloc(sizeof(struct list_int));
    if (newlist == NULL)
        errx(EXIT_FAILURE, "No free memory");
    newlist->first = NULL;
    newlist->last = NULL;
    return newlist;
}

void insertint (struct list_int *Liste, int n)
{
    if (Liste->first == NULL)
    {
        Liste->first = malloc(sizeof(struct ele_int));
        Liste->first->key = n;
        Liste->first->prec = NULL;
        Liste->first->next = NULL;
        Liste->last = Liste->first;
    }
    else
    {
        Liste->last->next = malloc(sizeof(struct ele_int));
        Liste->last->next->key = n;
        Liste->last->next->next = NULL;
        Liste->last->next->prec = Liste->last;
        Liste->last = Liste->last->next;
    }
}

void free_ele_int(struct ele_int *ele)
{
    if (ele->next != NULL)
        free_ele_int(ele->next);
    free(ele);
}

void free_list_int(struct list_int *liste)
{
    if (liste->first != NULL)
        free_ele_int(liste->first);
    free(liste);
}
