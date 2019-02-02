#include "build_metatree.h"

struct meta_tree *sub_build_mti(char *path)
{
    //TODO
}

struct meta_tree *sub_build_mtd(char *path)
{
    //TODO
}

struct meta_tree *FILESYSTEM_build_metatree(char *path)
{
    struct stat data;
    int err = stat(path, data);
    if (err == -1)
        err(21, "FILESYSTEM: stat Failure.");
    struct meta_tree *tree = calloc(tree, sizeof(struct meta_tree));
    if (S_ISREG(data.st_mode))
    {
        tree->son = sub_build_mtf(path);
    }
    else if (S_ISDIR(data.st_mode))
    {
        DIR *directory = opendir(path);
        struct dirent *next = readdir(directory);
        char newpath[1024];
        strcpy(newpath, path);
        for (char *start = newpath; p; p++);
        *start = '/';
        start++;
        char *p = start;
        char *q;
        for (q = next->d_name; *q; q++)
        {
            *p = *q;
            p++;
        }
        switch (next->d_type)
        {
            case DT_DIR:
                tree->son = sub_build_mtd(newpath);
                break;
            case DT_REG:
                tree->son = sub_build_mti(newpath);
                break;
            break;
        }
        struct meta_tree *temp = tree->son;
        while ((next = readdir(directory)))
        {
            p = start;
            for (q = next->d_name; *q; q++)
            {
                *p = *q;
                p++;
            }
            switch (next->d_type)
            {
                case DT_DIR:
                    temp->sibling = sub_build_mtd(newpath);
                    break;
                case DT_REG:
                    temp->sibling = sub_build_mti(newpath);
                    break;
                break;
            }
            temp = temp->sibling;
        }
    }
    return tree;
}

void FILESYSTEM_free_metatree(struct meta_tree *tree)
{
    //TODO
}

