#ifndef _SRC_FILESYSTEM_RESTORE_SAVE
#define _SRC_FILESYSTEM_RESTORE_SAVE

#include "build_metatree.h"
#include "save_metatree.h"
#include "save_file_content.h"
#include "build_restore_tree.h"

struct chained
{
    time_t mtime;
    char path[2048];
    struct chained *next;
}chained;

/* //TODO
*/

struct chained *RS_create_save_list(char *save_dir);

void FILESYSTEM_restore_save(char *save_dir);

struct meta_tree *FILESYSTEM_SAVE_restore_metatree_from_save(char *save);

void FILESYSTEM_restore_original_save(char *save);

#endif // _SRC_FILESYSTEM_RESTORE_SAVE
