#ifndef _SRC_COMPRESSION_LZ78_LZ78_H_
#define _SRC_COMPRESSOPM_LZ78_LZ78_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include "../struct.h"
#include "../file/file.h"
#include "../liste/pylist.h"
#include "dico.h"

void list_to_string(struct pylist *py, unsigned char *output);

size_t pylist_to_string(struct pylist *py, unsigned char *output);

size_t string_to_pylist(struct pylist *py, unsigned char *input, size_t len);

char *len_to_file(size_t len);

size_t file_to_len(char *f_len);

void compress_lz78(char *data_path, char *dico_path, char *tmp_path);

void decompress_lz78(char *data_path, char *dico_path, char *out_path);

#endif /* _SRC_COMPRESSION_LZ78_LZ78_H_*/
