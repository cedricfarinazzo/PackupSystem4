#ifndef _SRC_FILESYSTEM_BUILD_RESTORE_TREE_H
#define _SRC_FILESYSTEM_BUILD_RESTORE_TREE_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "build_metatree.h"
#include "save_metatree.h"

struct restore_data
{
    char *file;
    off_t offset;
};

struct restore_tree
{
    struct restore_data *data;
    struct restore_tree *son;
    struct restore_tree *sibling;
};

struct restore_tree *FILESYSTEM_build_restore_tree(DIR *save_dir);

#endif //_SRC_FILESYSTEM_BUILD_RESTORE_TREE_H
