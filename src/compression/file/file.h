#ifndef _SRC_COMPRESSION_FILE_FILE_H_
#define _SRC_COMPRESSION_FILE_FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>

#include "../struct.h"
#include "../lz78/dico.h"
#include "../lz78/lz78.h"
#include "../struct/converter.h"

void load_data_file(char *file_path, unsigned char **data_file,
        size_t *len_data);

void load_dico_file(struct dico **table, char *path_dico);

void write_data_file(char *data, ssize_t len_data, char *path_file);

void write_dico_file(struct dico *table, char *path_dico);

#endif /*_SRC_COMPRESSION_FILE_FILE_H_*/
