#ifndef _SRC_COMPRESSION_FILE_FILE_H_
#define _SRC_COMPRESSION_FILE_FILE_H_

#include <stdlib.>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

#include "../struct.h"
#include "../lz78/dico.h"
#include "../lz78/lz78.h"

void load_data_file(char *file_path, unsigned char *data_file,
        ssize_t len_data);

void load_dico_file(struct dico *table, char *path_dico);

void write_data_file(char *data, ssize_t len_data, char *path_file);

void write_dico_file(struct dico *table, char *path_dico);

#endif /*_SRC_COMPRESSION_FILE_FILE_H_*/
