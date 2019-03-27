#ifndef _SRC_FILESYSTEM_RESTORE_SAVE
#define _SRC_FILESYSTEM_RESTORE_SAVE

#include "build_metatree.h"
#include "save_metatree.h"
#include "save_file_content.h"
#include "build_restore_tree.h"

/* //TODO
*/

void FILESYSTEM_restore_save(char *save_dir);

struct meta_tree *FILESYSTEM_SAVE_restore_meta_tree(FILE *file);

#endif // _SRC_FILESYSTEM_RESTORE_SAVE
