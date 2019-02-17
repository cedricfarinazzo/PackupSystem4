#ifndef _SRC_COMPRESSION_STRUCT_H_
#define _SRC_COMPRESSION_STRUCT_H_

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

struct freqlist {
    struct liste *freq;
    struct liste *car;
} freqlist;

struct bintree {
    char key;
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

#endif /*_SRC_COMPRESSION_STRUCT_H_*/
