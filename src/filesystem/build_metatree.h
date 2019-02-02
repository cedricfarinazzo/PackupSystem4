#ifndef _SRC_FILESYSTEM_BUILD_METATREE_H
#define _SRC_FILESYSTEM_BUILD_METATREE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <err.h>

struct meta_data
{
    char *path;
    struct stat fs;
}

struct meta_tree
{
    neta_data data;
    meta_tree *son;
    meta_tree *sibling;
}

/*build_metatree: creates a tree with all metadata\
                  It takes the form of a tree as bintree\
                  each node represents a file, each left son\
                  is a son of the directory and each\
                  right son is a sibling of the file or\
                  directory.\
                  the tree has a sentinel that is the root.
  char *path: path to the directory that has to be represented\
              as a tree.
  return: a pointer to the allocated memory for the tree.
*/

struct meta_tree *FILESYSTEM_build_metatree(char *path);

/*free_metatree: free allocated memory for meta_tree struct and all it's\
                  sons and sibings.\
  struct meta_tre *tree: the tree that has to be freed;
  return: void
*/

void FILESYSTEM_free_metatree(struct meta_tree *tree);

#endif //_SRC_FILESYSTEM_BUILD_METATREE_H
