#ifndef _SRC_COMPRESSION_LISTE_H_
#define _SRC_COMPRESSION_LISTE_H_

#include <err.h>
#include <stdlib.h>
#include "huffman.h"
#include <string.h>
#include "struct.h"

struct liste *new_liste();

void insert(struct liste *liste, char n);

void insertr(struct liste *liste, struct liste *n);

char *liste_to_string(struct liste *liste);

void del_in(struct element *ele);

struct huffele *min_pop(struct freqlist *Freq);

int len_list(struct liste *liste);

void liste_free(struct liste *liste);

#endif /*_SRC_COMPRESSION_LISTE_H_*/
