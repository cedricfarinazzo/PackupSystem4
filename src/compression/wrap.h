#ifndef _SRC_COMPRESSION_WRAP_H_
#define _SRC_COMPRESSION_WRAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "struct.h"
#include "lz78/lz78.h"
#include "huffman/huffman.h"

void test_simple_huffman_compress(char *input_path, char *output_path);

void test_simple_huffman_decompress(char *input_path, char *output_path);

#endif /*_SRC_COMPRESSION_WRAP_H_*/
