#include "create_save.h"

void FS_save_path(char *path, FILE *save)
{
    long length = 1;
    length += strlen(path);
    int e = fwrite(&length, 1, sizeof(long), save);
    if (e < (long)sizeof(long))
        err(33, "FS_save_path failure. fwrite failure");
    fwrite(path, sizeof(char), length, save);
    if (e < length)
        err(33, "FS_save_path failure. fwrite failure");
}

void FS_save_stats(struct stat fs, FILE *save)
{
    int e = fwrite(&fs, 1, sizeof(struct stat), save);
    if (e < (long)sizeof(struct stat))
        err(33, "FS_save_path failure. fwrite failure");
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
        err(33, "FS_save_inheritance failure. fwrite failed.");
}

void FS_save_data(struct meta_data *data, FILE *save)
{
    FS_save_path(data->path, save);
    FS_save_stats(data->fs, save);
}

void FS_save_file(char *path, char *save)
{
    FILE *original = fopen(path, "w");
    FILESYSTEM_save_file_content(original, save);
    fclose(original);
}

void FS_save_tree(FILE *save, struct meta_tree *tree)
{
    FS_save_data(tree->data, save);
    FS_save_inheritance(tree, save);
    FS_save_file(tree->data->path, save);
    struct meta_tree *temp = tree->son;
    while (temp)
    {
        FS_save_tree(save, temp);
        temp = temp->sibling;
    }
}

void FILESYSTEM_create_save(char *path, char *savepath)
{
    FILE *save = fopen(savepath, "w");
    struct meta_tree *tree = FILESYSTEM_build_metatree(path);
    FS_save_tree(save, tree->son);
    FILESYSTEM_free_metatree(tree);
    fclose(save);
}

void FILESYSTEM_create_new_save(char *path, char*savepath, char *oldsave)
{
    //TODO
}
