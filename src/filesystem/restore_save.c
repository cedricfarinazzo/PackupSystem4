#include "restore_save.h"
#include "create_save.h"

char *RS_restore_path(FILE *file)
{
    /*
    reads the len of the saved path, reads said path and return it
    */
    long length;
    int e = fread(&length, sizeof(long), 1, file);
    if (e < (long) sizeof(long))
        err(33, "RS_restore_path, failed to read length");

    char *path = malloc(length * sizeof(char));
    fread(path, sizeof(char), length, file);

    return path;
}

struct stat RS_restore_stats(FILE *file)
{
    /*
    reads in the file the saved stat struct and returns it
    */
    struct stat fs;
    fread(&fs, sizeof(struct stat), 1, file);

    return fs;
}

off_t RS_skip_file_content(FILE *file)
{
    /*
    gets current offset to return it
    reads length of saved content
    if 0: returns 0, nothing to be skipped
    else: skips file content and returns saved offset
    */
    size_t length;
    fread(&length, sizeof(size_t), 1, file);

    if (length == 0)
        return 0;

    fseek(file, length, SEEK_CUR);

    return length;
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
    char *path = RS_restore_path(file);

    struct stat fs = RS_restore_stats(file);

    struct meta_data *data = malloc(sizeof(struct meta_data));

    data->path = path;
    data->fs = fs;

    data->file_content = RS_skip_file_content(file);

    return data;
}

char RS_restore_inheritance(FILE *file)
{
    /*
    reads the inheritance byte and returns it
    */
    char inheritance;
    fread(&inheritance, sizeof(char), 1, file);

    return inheritance;
}

struct meta_tree *RS_restore_tree(FILE *file)
{
    /*
    first: restore data
    second: get inheritance byte
    third: get offset for file content
    fourth: recursive calls
    */
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));

    tree->data = RS_restore_data(file);

    char inheritance = RS_restore_inheritance(file);

    if (inheritance & FILESYSTEM_TREE_HAS_SON)
        tree->son = RS_restore_tree(file);
    
    if (inheritance & FILESYSTEM_TREE_HAS_SIBLING)
        tree->sibling = RS_restore_tree(file);

    return tree;
}

struct meta_tree *FILESYSTEM_SAVE_restore_metatree_from_save(FILE *file)
{
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));

    tree->son = RS_restore_tree(file);

    return tree;
}

void FILESYSTEM_restore_save(char *save_dir)
{
    save_dir = save_dir;
    //TODO
}

void RS_restore_content(FILE *src, off_t offset, FILE *dst)
{
    fseek(src, offset, SEEK_SET);

    size_t length;
    fread(&length, sizeof(size_t), 1, src);

    char buf[2048];
    int r;
    int w;
    size_t total = 0;
    
    while (total < length)
    {
        if (total + 2048 > length)
        {
            r = fread(buf, sizeof(char), length - total, src);
            w = fwrite(buf, sizeof(char), r, dst);
            if (w < r)
                err(33, "RS_restore_content: failed to write all bytes");
            break;
        }
        r = fread(buf, sizeof(char), 2048, src);
        w = fwrite(buf, sizeof(char), r, dst);
        if (w < r)
            err(33, "RS_restore_content: failed to write all bytes");
        total += r;
    }
}

void RS_restore_from_meta_tree(struct meta_tree *tree, FILE *src)
{
    if (tree->son == NULL)
    {
        FILE *dst = fopen(tree->data->path, "w");
        RS_restore_content(src, tree->data->file_content, dst);
        fclose(dst);
    }
    else
    {
        mkdir(tree->data->path, 0666);
        struct meta_tree *temp = tree->son;
        while (temp)
        {
            RS_restore_from_meta_tree(temp, src);
            temp = temp->sibling;
        }
    }
}

void FILESYSTEM_restore_original_save(char *save)
{
    FILE *file = fopen(save, "r");
    struct meta_tree *tree = FILESYSTEM_SAVE_restore_meta_tree_from_save(file);

    RS_restore_from_meta_tree(tree->son, file);

    fclose(file);
    FILESYSTEM_free_metatree(tree);
}
