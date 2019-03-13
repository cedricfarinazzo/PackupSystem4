#ifndef _SRC_COMPRESSION_STRUCT_H_
#define _SRC_COMPRESSION_STRUCT_H_

struct element {
    unsigned char key;
    struct element *next;
    struct element *prec;
} element;

struct liste {
    struct element *first;
    struct element *last;
} liste;

struct huffele {
    unsigned char car;
    unsigned char freq;
} huffele;

struct freqlist {
    struct liste *freq;
    struct liste *car;
} freqlist;

struct bintree {
    unsigned char key;
    struct bintree *left;
    struct bintree *right;
} bintree;

struct binlist {
    struct binele *first;
} binlist;

struct binele {
    struct bintree *key;
    struct binele *next;
} binele;

struct encod_tree {
    char align;
    int prof;
    int len;
    unsigned char *data;
} encod_tree;

struct encod_data {
    char align;
    int len;
    unsigned char *data;
} encod_data;

#endif /*_SRC_COMPRESSION_STRUCT_H_*/
