#ifndef _SRC_COMPRESSION_STRUCT_H_
#define _SRC_COMPRESSION_STRUCT_H_
#include <unistd.h>
#include <stdlib.h>

struct element {
    unsigned char key;
    struct element *next;
    struct element *prec;
} element;

struct ele_int {
    int key;
    struct ele_int *next;
    struct ele_int *prec;
} ele_int;

struct liste {
    struct element *first;
    struct element *last;
} liste;

struct list_int {
    struct ele_int *first;
    struct ele_int *last;
} list_int;

struct huffele {
    unsigned char car;
    unsigned char freq;
} huffele;

struct freqlist {
    struct list_int *freq;
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

struct huff_out {
    unsigned char *dataOUT;
    int len;
} huff_out;

struct pyele {
    ssize_t key;
    struct pyele *prec;
    struct pyele *next;
} pyele;

struct pylist {
    size_t len;
    struct pyele *begin;
    struct pyele *end;
} pylist;

struct dico {
    ssize_t *vector;
    unsigned char *letter;
    size_t len;
    size_t taux;
} dico;
#endif /*_SRC_COMPRESSION_STRUCT_H_*/
