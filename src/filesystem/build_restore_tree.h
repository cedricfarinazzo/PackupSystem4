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
    char file[2048];
    char src[2048];
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

/*RS_update_restore_tree_from_mt: creates a restore tree\
                                  or update it from a meta_tree\
                                  given in argument, this metatree\
                                  comes from savepath\
  restore_tree *rt: the restore tree to update or init
  meta_tree *mt: the meta_tree from which it is updated or initialized
  char *savepath: the saved from which mt was restored
*/

void RS_update_restore_tree_from_mt(struct restore_tree *rt, struct meta_tree *mt, char *savepath);

/*RS_free_restore_tree: free the restore tree rt
  restore_tree *rt: the tree needing to be freed
*/

void RS_free_restore_tree(struct restore_tree *rt);

#endif //_SRC_FILESYSTEM_BUILD_RESTORE_TREE_H
