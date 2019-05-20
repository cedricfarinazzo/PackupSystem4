#ifndef _SRC_COMPRESSION_STRUCT_CONVERTER_H_
#define _SRC_COMPRESSION_STRUCT_CONVERTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include "../struct.h"
#include "../lz78/dico.h"

size_t file_to_len(char *f_len);

char *len_to_file(size_t len);

void list_to_string(struct pylist *py, unsigned char *output);

size_t pylist_to_string(struct pylist *py, unsigned char *output);

size_t string_to_pylist(struct pylist *py, unsigned char *input, size_t len);

#endif /*_SRC_COMPRESSION_STRUCT_CONVERTER_H_*/
