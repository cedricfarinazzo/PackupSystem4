#include "save_metatree.h"

void FS_save_path(char *path, FILE *save)
{
    long length = 1;
    length += strlen(path);
    int e = fwrite(&length, sizeof(char), sizeof(long), save);
    if (e < (long) sizeof(long))
        err(33, "FS_save_path failure. fwrite failure (1).");
    e = fwrite(path, sizeof(char), length, save);
    if (e < length)
        err(33, "FS_save_path failure. fwrite failure (2).");
}

void FS_save_stats(struct stat fs, FILE *save)
{
    int e = fwrite(&fs, 1, sizeof(struct stat), save);
    if (e < (long) sizeof(struct stat))
        err(33, "FS_save_stats failure. fwrite failure.");
}

void FS_save_inheritance(struct meta_tree *tree, FILE *save)
{
    char inheritance = 0;
    if (tree->son)
        inheritance |= FILESYSTEM_TREE_HAS_SON;
    if (tree->sibling)
        inheritance |= FILESYSTEM_TREE_HAS_SIBLING;
    int e = fwrite(&inheritance, sizeof(char), 1, save);
    if (e < 1)
        err(33, "FS_save_inheritance failure. fwrite failure.");
}

void FS_save_data(struct meta_data *data, FILE *save)
{
    if (data->path)
        FS_save_path(data->path, save);
    FS_save_stats(data->fs, save);
}

void FS_save_tree(FILE *save, struct meta_tree *tree)
{
    if (tree->data)
        FS_save_data(tree->data, save);
    FS_save_inheritance(tree, save);
    struct meta_tree *temp = tree->son;
    while (temp)
    {
        FS_save_tree(save, temp);
        temp = temp->sibling;
    }
}

void FILESYSTEM_save_metatree(struct meta_tree *tree, char *path)
{
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

struct meta_tree *FS_restore_tree(FILE *file)
{
    struct meta_tree *tree = malloc(sizeof(struct meta_tree));
    tree->data = FS_restore_data(file);
    char inheritance = FS_restore_inheritance(file);
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
