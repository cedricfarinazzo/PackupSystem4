#ifndef _SRC_COMPRESSION_BINTREE_H_
#define _SRC_COMPRESSION_BINTREE_H_

#include <err.h>
#include <stdlib.h>

struct bintree {
    unsigned char key;
    struct bintree *left;
    struct bintree *right;
} bintree;

struct bintree new_tree(char n);

struct bintree insert_right(struct bintree b, char n);

struct bintree insert_left(struct bintree b, char n);

void bin_free(struct bintree b);

#endif /*_SRC_COMPRESSION_BINTREE_H_*/
