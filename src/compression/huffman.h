#ifndef _SRC_COMPRESSION_HUFFMAN_H_
#define _SRC_COMPRESSION_HUFFMAN_H_

#include <err.h>
#include <stdlib.h>
#include "bintree.h"
#include "liste.h"
#include "struct.h"

void free_freqlist(struct freqlist *Freqlist);

struct freqlist* buildFrequenceList(char dataIN[]);

struct bintree *buildHuffmantree(struct freqlist *Freq);

void table_codage(struct bintree *huffman, struct liste *table);

struct liste *encode_data(char dataIN[], char *table, int len);

void codage_tree_(struct liste *output, struct bintree *huffman);

int principale(char dataIN[]);

#endif /*_SRC_COMPRESSION_HUFFMAN_H_*/
