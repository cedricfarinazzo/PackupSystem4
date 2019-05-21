#ifndef _SRC_FILESYSTEM_RESTORE_SAVE
#define _SRC_FILESYSTEM_RESTORE_SAVE

#include "build_metatree.h"
#include "save_metatree.h"
#include "save_file_content.h"
#include "build_restore_tree.h"

struct chained
{
    time_t mtime;
    char path[4096];
    struct chained *next;
}chained;

/*FILESYSTEM_restore_save: restore a dir from the saves in folder save_dir
  save_dir: the dir with the saves inside
*/

void FILESYSTEM_restore_save(char *save_dir);

/*FILESYSTEM_SAVE_restore_metatree_from_save: restores a metatree from a save
  save: the save in which the tree was saved
*/

struct meta_tree *FILESYSTEM_SAVE_restore_metatree_from_save(char *save);

/*FILESYSTEM_restore_original_save: restores a dir from an original save
  save: the original save
*/

void FILESYSTEM_restore_original_save(char *save);

#endif // _SRC_FILESYSTEM_RESTORE_SAVE
