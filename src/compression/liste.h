#ifndef _SRC_COMPRESSION_LISTE_H_
#define _SRC_COMPRESSION_LISTE_H_

#include <err.h>
#include <stdlib.h>
#include "huffman.h"
#include <string.h>

struct element {
    char key;
    struct element *next;
    struct element *prec;
} element;

struct liste {
    struct element *first;
    struct element *last;
} liste;

struct huffele {
    char car;
    char freq;
} huffele;

struct liste *new_liste();

void insert(struct liste *liste, char n);

void insertr(struct liste *liste, char* n);

char *list_to_string(struct liste *liste);

void del_in(struct element *ele);

struct huffele *min_pop(struct freqlist *Freq);

int len_list(struct liste *liste);

void liste_free(struct liste *liste);

#endif /*_SRC_COMPRESSION_LISTE_H_*/
