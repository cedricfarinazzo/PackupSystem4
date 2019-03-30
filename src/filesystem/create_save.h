#ifndef _SRC_FILESYSTEM_CREATE_SAVE
#define _SRC_FILESYSTEM_CREATE_SAVE

#include "build_metatree.h"
#include "save_file_content.h"

#define FILESYSTEM_TREE_HAS_SON 0x10
#define FILESYSTEM_TREE_HAS_SIBLING 0x01

/* //TODO
*/

void FILESYSTEM_create_save(char *path, char *savepath);

void FILESYSTEM_create_new_save(char *path, char *savepath, char *oldsave);

#endif // _SRC_FLESYSTEM_CREATE_SAVE
