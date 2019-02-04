#ifndef _SRC_COMPRESSION_BINTREE_H_
#define _SRC_COMPRESSION_BINTREE_H_

#include <err.h>
#include <stdlib.h>

struct bintree {
    unsigned char key;
    struct bintree *left;
    struct bintree *right;
} bintree;

bintree new_tree(char n);

bintree insert_right(bintree b, char n);

bintree insert_left(bintree b, char n);

void bin_free(bintree b);

#endif /*_SRC_COMPRESSION_BINTREE_H_*/
