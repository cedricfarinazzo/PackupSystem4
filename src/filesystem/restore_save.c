#include "restore_save.h"
#include "create_save.h"

void FILESYSTEM_restore_save(char *save_dir)
{
    save_dir = save_dir;
    //TODO
}

char *RS_restore_path(FILE *file)
{
    /*
    reads the len of the saved path, reads said path and return it
    */
    //TODO
}

struct stat RS_restore_stats(FILE *file)
{
    /*
    reads in the file the saved stat struct and returns it
    */
    //TODO
}

off_t RS_skip_file_content(FILE *file)
{
    /*
    gets current offset to return it
    reads length of saved content
    if 0: returns 0, nothing to be skipped
    else: skips file content and returns saved offset
    */
    //TODO
}

struct meta_data *RS_restore_data(FILE *file)
{
    /*
    first: allocates memory for meta_data struct
    second: restores path
    third: restores stats
    fourth: gets offset for file_content and skips file_content
    fifth: returns pointer to allocated memory
    */
    //TODO
}

char RS_restore_inheritance(FILE *file)
{
    /*
    reads the inheritance byte and returns it
    */
    //TODO
}

struct meta_tree *RS_restore_tree(FILE *file)
{
    /*
    first: restore data
    second: get inheritance byte
    third: get offset for file content
    fourth: recursive calls
    */
    //TODO
}

struct meta_tree *FILESYSTEM_SAVE_restore_metatree_from_save(FILE *file)
{
    file = file;
    //TODO
    return NULL;
}
