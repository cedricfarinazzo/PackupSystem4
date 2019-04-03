#ifndef _SRC_COMPRESSION_HUFFMAN_SIMPLE_DECOMPRESSION_H_
#define _SRC_COMPRESSION_HUFFMAN_SIMPLE_DECOMPRESSION_H_

#include <err.h>
#include <stdlib.h>
#include "../../struct.h"
#include "../huffman.h"

struct huff_out *one_decompression(unsigned char *bindata, int len,
        unsigned char *chareq, char align);

struct huff_out *dual_decompression(unsigned char *bindata, int len,
        unsigned char *chareq, char align);

struct huff_out *simple_decompression(unsigned char *dataIN, int len_IN);

#endif /*_SRC_COMPRESSION_HUFFMAN_SIMPLE_DECOMPRESSION_H_ */
