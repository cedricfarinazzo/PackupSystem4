#ifndef _SRC_COMPRESSION_SIMPLE_FOUR_COMPRESSION_H_
#define _SRC_COMPRESSION_SIMPLE_FOUR_COMPRESSION_H_

#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../liste/liste.h"
#include "../huffman.h"
#include "../../struct.h"

struct huff_out *one_compression(struct freqlist *freqList, int len_IN);

struct huff_out *two_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN);

struct huff_out *triple_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN, int longueur);

struct huff_out *simple_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN);

#endif /*_SRC_COMPRESSION_SIMPLE_FOUR_COMPRESSION_H_*/
