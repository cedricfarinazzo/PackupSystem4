#ifndef _SRC_FILESYSTEM_SAVE_FILE_CONTENT_H
#define _SRC_FILESYSTEM_SAVE_FILE_CONTENT_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <err.h>
#include "build_metatree.h"
#include "save_metatree.h"

/* FILESYSTEM_save_file_content: saves content of file src in file dst
   src: the file to be saved
   dst: the file in which the content is saved
   return: void
*/

void FILESYSTEM_save_file_content(FILE *src, FILE *dst);

/* FILESYSTEM_load_file_content: allocates memory for the content of one file
                                 file contents are defined by their length
   file: the file in which the content was saved
   offset: the offset at which the content was saved
   return: char*, the content, do not forget to free
*/

char *FILESYSTEM_load_file_content(FILE *file, off_t offset);

#endif //_SRC_FILESYSTEM_SAVE_FILE_CONTENT
