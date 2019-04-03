#ifndef _SRC_COMPRESSION_HUFFMAN_H_
#define _SRC_COMPRESSION_HUFFMAN_H_

#include <err.h>
#include <stdlib.h>
#include "../tree/bintree.h"
#include "../liste/liste.h"
#include "../struct.h"
#include "simple/four_compression.h"
#include "simple/four_decompression.h"

void free_out(struct huff_out *data);

void free_freqlist(struct freqlist *Freqlist);

void print_chare(unsigned char *output, int len);

void strcpyh(unsigned char *dest, const unsigned char *src, int n);

struct freqlist* buildFrequenceList(unsigned char dataIN[], int len);

struct bintree *buildHuffmantree(struct freqlist *Freq);

void table_codage(struct bintree *huffman, struct liste *table);

struct liste *encode_data(unsigned char dataIN[], unsigned char *table,
        int len);

void codage_tree_(struct liste *output, struct bintree *huffman);

int max_prof(struct bintree *H);

void output_tree(struct liste *table, struct encod_tree *output,
        struct bintree *H);

void output_data(struct liste *datai, struct encod_data *output);

struct huff_out *compression(unsigned char *dataIN, int len);

int rebuild_tree(unsigned char *data, int actual, unsigned char key,
        struct bintree *huffman, int prof);

unsigned char power(unsigned char n, unsigned char p);

struct bintree *decode_tree(unsigned char *data, int len, int prof, char align,
        int nb_char);

int decompressing_data(struct bintree *huffman, unsigned char *data,
        int len, struct liste *decompressed, char align);

struct huff_out *decompression(unsigned char *data, int len_data);

#endif /*_SRC_COMPRESSION_HUFFMAN_H_*/
