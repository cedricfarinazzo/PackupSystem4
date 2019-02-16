#include "save_metatree.h"

void FS_save_path(char *path, FILE *save)
{
    path = path;
    save = save;
}

void FS_save_stats(struct stat fs, FILE *save)
{
    fs = fs;
    save = save;
}

void FS_save_inheritance(struct meta_tree *tree, FILE *save)
{
    tree = tree;
    save = save;
}

void FS_save_data(struct meta_data *data, FILE *save)
{
    FS_save_path(data->path, save);
    FS_save_stats(data->fs, save);
}

void FS_save_tree(FILE *save, struct meta_tree *tree)
{
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
    FS_save_tree(file, tree);
    fclose(file);
}

struct meta_tree *FS_restore_tree(FILE *file)
{
    file = file;
    return NULL;
}

struct meta_tree *FILESYSTEM_restore_metatree(char *path)
{
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));
    FILE *file = fopen(path, "r");
    tree->son = FS_restore_tree(file);
    fclose(file);
    return tree;
}
