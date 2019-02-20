#ifndef _SRC_COMPRESSION_LISTE_H_
#define _SRC_COMPRESSION_LISTE_H_

#include <err.h>
#include <stdlib.h>
#include "huffman.h"
#include <string.h>
#include "struct.h"

struct liste *new_liste();

int len_list(struct liste *liste);

void insert(struct liste *liste, char n);

void insertr(struct liste *liste, struct liste *n);

void inserts(struct liste *Liste, struct liste *n, int c);

void del_last(struct liste *Liste);

char *liste_to_string(struct liste *liste);

void del_in(struct element *ele);

struct huffele *min_pop(struct freqlist *Freq);

void liste_free(struct liste *liste);

#endif /*_SRC_COMPRESSION_LISTE_H_*/
