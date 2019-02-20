#ifndef _SRC_FILESYSTEM_SAVE_METATREE_H
#define _SRC_FILESYSTEM_SAVE_METATREE_H

#define _GNU_SOURCE
#define FILESYSTEM_TREE_HAS_SON 0x10
#define FILESYSTEM_TREE_HAS_SIBLING 0x01

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <err.h>
#include "build_metatree.h"

/* FILESYSTEM_save_metatree: saves metatree in a file
   path: path to the file in which the tree is to
         saved
   tree: the tree to be saved in the file
*/

void FILESYSTEM_save_metatree(struct meta_tree *tree, char *path);

/* FILESYSTEM_restore_metatree: restores metatree
                                from a saved file
   path: the path in which the tree is saved
*/

struct meta_tree *FILESYSTEM_restore_metatree(char *path);

#endif //_SRC_FILESYSTEM_SAVE_METATREE_H
