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

void liste_to_string(struct liste *liste, unsigned char *output);

void del_in(struct element *ele);

void min_pop(struct freqlist *Freq, struct huffele *output);

void liste_free(struct liste *liste);

struct list_int *new_list(void);

void insertint(struct list_int *Liste, int n);

void free_list_int(struct list_int *liste);
#endif /*_SRC_COMPRESSION_LISTE_H_*/
