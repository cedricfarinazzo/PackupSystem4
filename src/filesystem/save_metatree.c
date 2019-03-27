#include "save_metatree.h"

/*
saves given path and its len in given file
*/

void FS_save_path(char *path, FILE *save)
{
    /*
    calculating length counting terminating null byte
    */
    long length = 1;
    length += strlen(path);

    /*
    writing said length
    */
    int e = fwrite(&length, sizeof(char), sizeof(long), save);
    if (e < (long) sizeof(long))
        err(33, "FS_save_path failure. fwrite failure (1).");
    
    /*
    writing path
    */
    e = fwrite(path, sizeof(char), length, save);
    if (e < length)
        err(33, "FS_save_path failure. fwrite failure (2).");
}

/*
saves given stat struct by writing it in given file
*/

void FS_save_stats(struct stat fs, FILE *save)
{
    /*
    writing stats struct
    */
    int e = fwrite(&fs, 1, sizeof(struct stat), save);
    if (e < (long) sizeof(struct stat))
        err(33, "FS_save_stats failure. fwrite failure.");
}

/*
saves inheritance of tree by translating it in a byte and writing it in a file
*/

void FS_save_inheritance(struct meta_tree *tree, FILE *save)
{
    /*
    initializing said byte to 0
    */
    char inheritance = 0;

    /*
    if given tree has a son, indicate it in a byte
    */
    if (tree->son)
        inheritance |= FILESYSTEM_TREE_HAS_SON;

    /*
    if given tree has a sibling, indicate it in a byte
    */
    if (tree->sibling)
        inheritance |= FILESYSTEM_TREE_HAS_SIBLING;

    /*
    writing said byte in given file
    */
    int e = fwrite(&inheritance, sizeof(char), 1, save);
    if (e < 1)
        err(33, "FS_save_inheritance failure. fwrite failure.");
}

/*
saves meta_data struct in given file
*/

void FS_save_data(struct meta_data *data, FILE *save)
{
    /*
    saves path first by calling save path function
    */
    if (data->path)
        FS_save_path(data->path, save);

    /*
    saves stats then by calling save stat function
    */
    FS_save_stats(data->fs, save);
}

/*
save_tree function that saves tree by calling other functions
*/

void FS_save_tree(FILE *save, struct meta_tree *tree)
{
    /*
    first save data
    */
    if (tree->data)
        FS_save_data(tree->data, save);

    /*
    then save inheritance
    */
    FS_save_inheritance(tree, save);

    /*
    then recursively save sons
    */
    struct meta_tree *temp = tree->son;
    while (temp)
    {
        FS_save_tree(save, temp);
        temp = temp->sibling;
    }
}

void FILESYSTEM_save_metatree(struct meta_tree *tree, char *path)
{
    /*
    opens file, call function to save tree then close file
    */
    FILE *file = fopen(path, "w");
    FS_save_tree(file, tree->son);
    fclose(file);
}

char *FS_restore_path(FILE *file)
{
    long len;
    fread(&len, sizeof(long), 1, file);
    char *path = malloc(len * sizeof(char));
    fread(path, sizeof(char), len, file);
    return path;
}

struct stat FS_restore_stats(FILE *file)
{
    struct stat fs;
    fread(&fs, sizeof(struct stat), 1, file);
    return fs;
}

struct meta_data *FS_restore_data(FILE *file)
{
    char *path = FS_restore_path(file);
    struct stat fs = FS_restore_stats(file);
    struct meta_data *data = malloc(sizeof(struct meta_data));
    data->path = path;
    data->fs = fs;
    return data;
}

char FS_restore_inheritance(FILE *file)
{
    char inheritance;
    fread(&inheritance, sizeof(char), 1, file);
    return inheritance;
}

void FS_skip_file_content(FILE *file)
{
    long len;
    fread(&len, 1, sizeof(len), file);
    fseek(file, len, SEEK_CUR);
}

struct meta_tree *FS_restore_tree(FILE *file)
{
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));
    tree->data = FS_restore_data(file);
    char inheritance = FS_restore_inheritance(file);
    FS_skip_file_content(file);
    if (inheritance & FILESYSTEM_TREE_HAS_SON)
        tree->son = FS_restore_tree(file);
    if (inheritance & FILESYSTEM_TREE_HAS_SIBLING)
        tree->sibling = FS_restore_tree(file);
    return tree;
}

struct meta_tree *FILESYSTEM_restore_metatree(char *path)
{
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));
    FILE *file = fopen(path, "r");
    tree->son = FS_restore_tree(file);
    fclose(file);
    return tree;
}
