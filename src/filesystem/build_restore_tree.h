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
    char *src;
    off_t offset;
    mode_t mode;
    time_t mtime;
};

struct restore_tree
{
    struct restore_data *data;
    struct restore_tree *son;
    struct restore_tree *sibling;
};

void RS_update_restore_tree_from_mt(struct restore_tree *rt, struct meta_tree *mt, char *savepath);

void RS_free_restore_tree(struct restore_tree *rt);

#endif //_SRC_FILESYSTEM_BUILD_RESTORE_TREE_H
