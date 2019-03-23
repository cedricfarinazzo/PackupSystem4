#ifndef _SRC_FILESYSTEM_CREATE_SAVE
#define _SRC_FILESYSTEM_CREATE_SAVE

#include "build_metatree.h"
#include "save_metatree.h"
#include "save_file_content.h"

/* //TODO
*/

void FILESYSTEM_create_save(char *path, char *savepath);

void FILESYSTEM_create_new_save(char *path, char *savepath, char *oldsave);

#endif // _SRC_FLESYSTEM_CREATE_SAVE
