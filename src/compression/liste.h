#ifndef _SRC_COMPRESSION_LISTE_H_
#define _SRC_COMPRESSION_LISTE_H_

#include <err.h>
#include <stdlib.h>

struct element {
    char key;
    struct element *next;
    struct element *prec;
} element;

struct liste {
    struct element *first;
    struct element *last;
} liste;

struct liste *new_liste();

void insert(struct liste *liste, char n);

void free(struct liste *liste);

#endif /*_SRC_COMPRESSION_LISTE_H_*/
