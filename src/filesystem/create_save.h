#ifndef _SRC_FILESYSTEM_CREATE_SAVE
#define _SRC_FILESYSTEM_CREATE_SAVE

#include "build_metatree.h"
#include "save_file_content.h"

#define FILESYSTEM_TREE_HAS_SON 0x10
#define FILESYSTEM_TREE_HAS_SIBLING 0x01

/*FILESYSTEM_create_save: creates initial save of path
  path: the path of the dir to save
  savepath: the path of the file in which to save it
*/

void FILESYSTEM_create_save(char *path, char *savepath);

/*FILESYSTEM_create_new_save: creates a new save based on an old one
  path: the path of the dir to be saved
  savepath: the path in which to save it
  oldsave: the path of the previous save
*/

void FILESYSTEM_create_new_save(char *path, char *savepath, char *oldsave);

#endif // _SRC_FLESYSTEM_CREATE_SAVE
