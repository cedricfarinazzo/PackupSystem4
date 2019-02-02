#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <err.h>
#include "build_metatree.h"

struct meta_tree *sub_build_mti(char *path)
{
    path = path;
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));
    struct stat fs;
    struct meta_data *data = malloc(sizof(struct meta_data));
    size_t len;
    for (char *temp = path; *temp; temp++)
    {
        len++;
    }
    data->path = malloc((len + 1) * sizeof(char));
    for (size_t p = 0; *(path + p); p++)
    {
        *(data->path + p) = *(path + p);
    }
    *(data->path + p) = 0;
    int e = stat(path, &fs);
    if (e == -1)
        err(22, "FILESYSTEM: sub build file stat failure.");
    data->fs = &fs;
    tree->data = data;
    return tree;
}

struct meta_tree *sub_build_mtd(char *path)
{
    path = path;
    //TODO
    return NULL;
}

struct meta_tree *FILESYSTEM_build_metatree(char *path)
{
    struct stat data;
    int er = stat(path, &data);
    if (er == -1)
        err(21, "FILESYSTEM: stat Failure.");
    struct meta_tree *tree = calloc(1 , sizeof(struct meta_tree));
    if (S_ISREG(data.st_mode))
    {
        tree->son = sub_build_mti(path);
    }
    else if (S_ISDIR(data.st_mode))
    {
        DIR *directory = opendir(path);
        struct dirent *next = readdir(directory);
        char newpath[1024];
        strcpy(newpath, path);
        char *start;
        for (start = newpath; *start; start++);
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
    tree = tree;
    //TODO
}

