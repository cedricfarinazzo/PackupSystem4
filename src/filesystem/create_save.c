#include "create_save.h"
#include "restore_save.h"

void CS_save_path(char *path, FILE *save)
{
    long length = 1;
    length += strlen(path);
    int e = fwrite(&length, 1, sizeof(long), save);
    if (e < (long)sizeof(long))
        err(33, "CS_save_path failure. fwrite failure(1)");
    e = fwrite(path, sizeof(char), length, save);
    if (e < length)
        err(33, "CS_save_path failure. fwrite failure(1)");
}

void CS_save_stats(struct stat fs, FILE *save)
{
    int e = fwrite(&fs, 1, sizeof(struct stat), save);
    if (e < (long)sizeof(struct stat))
        err(33, "CS_save_path failure. fwrite failure");
}

void CS_save_inheritance(struct meta_tree *tree, FILE *save)
{
    char inheritance = 0;
    if (tree->son)
        inheritance |= FILESYSTEM_TREE_HAS_SON;
    if (tree->sibling)
        inheritance |= FILESYSTEM_TREE_HAS_SIBLING;
    int e = fwrite(&inheritance, sizeof(char), 1, save);
    if (e < 1)
        err(33, "CS_save_inheritance failure. fwrite failed.");
}

void CS_save_data(struct meta_data *data, FILE *save)
{
    CS_save_path(data->path, save);
    CS_save_stats(data->fs, save);
}

void CS_save_file(char *path, FILE *save)
{
    FILE *original = fopen(path, "r");
    if (original == NULL)
    {
        printf("Failed to open file: %s\n", path);
        err(35, "CS_save_file: Failed to open source file.");
    }
    FILESYSTEM_save_file_content(original, save);
    fclose(original);
}

void CS_save_tree(FILE *save, struct meta_tree *tree)
{
    if (tree == NULL)
        err(34, "CS_save_tree: Null tree error");
    CS_save_data(tree->data, save);
    CS_save_inheritance(tree, save);
    CS_save_file(tree->data->path, save);
    struct meta_tree *temp = tree->son;
    while (temp)
    {
        CS_save_tree(save, temp);
        temp = temp->sibling;
    }
}

void FILESYSTEM_create_save(char *path, char *savepath)
{
    FILE *save = fopen(savepath, "w");
    if (save == NULL)
        err(35, "FILESYSTEM_create_save: failed to open file.");
    struct meta_tree *tree = FILESYSTEM_build_metatree(path);
    if (tree)
    {
        CS_save_tree(save, tree->son);
        FILESYSTEM_free_metatree(tree);
    }
    else
    {
        fclose(save);
        err(36, "FILESYSTEM_create_save: failed to build tree.");
    }
    fclose(save);
}

void FILESYSTEM_create_new_save(char *path, char*savepath, char *oldsave)
{
    path = path;
    savepath=savepath;
    oldsave=oldsave;
    //TODO
}