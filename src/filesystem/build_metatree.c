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
    //printf("entered mti with path: %s\n", path);
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));
    struct stat fs;
    struct meta_data *data = malloc(sizeof(struct meta_data));
    size_t len = 0;
    for (char *temp = path; *temp; temp++)
    {
        len++;
    }
    data->path = calloc((len + 1), sizeof(char));
    size_t p;
    for (p = 0; *(path + p); p++)
    {
        *(data->path + p) = *(path + p);
    }
    int e = stat(path, &fs);
    if (e == -1)
        err(22, "FILESYSTEM: sub build file stat failure.");
    data->fs = fs;
    tree->data = data;
    //printf("finished mti\n");
    return tree;
}

struct meta_tree *sub_build_mtd(char *chemin)
{
    //printf("entered mtd with path: %s\n", chemin);
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));
    struct meta_data *data = malloc(sizeof(struct meta_data));
    struct stat fs;
    int e = stat(chemin, &fs);
    if (e == -1)
        err(23, "FILESYSTEM: sub build tree directory stat failure.");
    size_t len = 0;
    for (char *temp = chemin; *temp; temp++)
    {
        len++;
    }
    data->path = malloc((len + 1) * sizeof(char));
    size_t k;
    for (k = 0; *(chemin + k); k++)
    {
        *(data->path + k) = *(chemin + k);
    }
    *(data->path + k) = 0;
    data->fs = fs;
    tree->data = data;
    DIR *directory = opendir(chemin);
    struct dirent *next = readdir(directory);
    while (next && *(next->d_name) == '.')
        next = readdir(directory);
    if (next == NULL)
    {
        //printf("null detected\n");
        tree->son = NULL;
        return tree;
    }
    //printf("copy started\n");
    char newpath[1024];
    strcpy(newpath, chemin);
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
    *p = 0;
    //printf("copy finished \n");
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
        if (*(p) == '.')
            continue;
        for (q = next->d_name; *q; q++)
        {
            *p = *q;
            p++;
        }
        *p = 0;
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
    //printf("finished mtd\n");
    return tree;
}

struct meta_tree *FILESYSTEM_build_metatree(char *path)
{
    //printf("start building tree\n");
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
        tree->son = sub_build_mtd(path);
    }
    //printf("returning tree\n");
    return tree;
}

void FILESYSTEM_free_metatree(struct meta_tree *tree)
{
    struct meta_tree *temp1 = tree->son;
    struct meta_tree *temp2;
    while (temp1)
    {
        temp2 = temp1->sibling;
        FILESYSTEM_free_metatree(temp1);
        temp1 = temp2;
    }
    if (tree->data)
    {
        free(tree->data->path);
        free(tree->data);
    }
    free(tree);
}

