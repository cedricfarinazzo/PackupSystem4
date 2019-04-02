#ifndef _SRC_COMPRESSION_BINTREE_H_
#define _SRC_COMPRESSION_BINTREE_H_

#include <err.h>
#include <stdlib.h>
#include "../struct.h"

struct bintree *new_tree(char n);

struct binlist *new_binlist();

void insert_right(struct bintree *b, char n);

void insert_left(struct bintree *b, char n);

void insert_binlist(struct binlist *l, struct bintree *b);

void bin_free(struct bintree *b);

void binlist_free(struct binlist *b);

#endif /*_SRC_COMPRESSION_BINTREE_H_*/
