#ifndef _COMPRESSION_FILE_H_
#define _COMPRESSION_FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/stat.h>

#include "huffman/huffman.h"
#include "struct.h"
#include "file.h"

long int findSize(const char *file_name);

char *compression_function(char *path_input);

char *decompression_function(char *path_input);

#endif /*_COMPRESSION_FILE_H_ */
