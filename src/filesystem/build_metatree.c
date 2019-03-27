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

/*
builds node from regular file
*/

struct meta_tree *sub_build_mti(char *path)
{
    /*
    allocates memory for tree and data and declares stat struct
    */
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));
    struct stat fs;
    struct meta_data *data = malloc(sizeof(struct meta_data));
    
    /*
    determines len of path and allocate memory to save it
    */
    size_t len = strlen(path);
    data->path = calloc((len + 1), sizeof(char));
    
    /*
    saves path in data
    */
    strncpy(data->path, path, len);

    /*
    gets stats of file and checks for errors
    */
    int e = stat(path, &fs);
    if (e == -1)
        err(22, "FILESYSTEM: sub build file stat failure.");
    
    /*
    saves stats in data and initialize offset of file content
    to 0 as this tree was not loaded from a save file
    */
    data->fs = fs;
    data->file_content = 0;

    /*
    links meta_data to meta_tree
    */
    tree->data = data;
        
    return tree;
}

/*
builds node from directory
*/

struct meta_tree *sub_build_mtd(char *path)
{
    /*
    allocates memory for tree and data and declares stat struct
    */
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));
    struct meta_data *data = malloc(sizeof(struct meta_data));
    struct stat fs;

    /*
    gets stats and checks for errors
    */
    int e = stat(chemin, &fs);
    if (e == -1)
        err(23, "FILESYSTEM: sub build tree directory stat failure.");
    
    /*
    gets len of path and allocates memory for it in data
    */
    size_t len = strlen(path);
    data->path = calloc((len + 1), sizeof(char));
    
    /*
    copies path to data
    */
    strncpy(data->path, path, len);
    
    /*
    saves stats in data and initialize offset of file content to 0
    as this tree is not loaded from a save
    */
    data->fs = fs;
    data->file_content = 0;

    /*
    links data to tree node
    */
    tree->data = data;

    /*
    opens directory and gets the first file in it
    */
    DIR *directory = opendir(path);
    struct dirent *next = readdir(directory);

    /*
    avoid directories '.' and '..' as they cause issues and are useless in our tree
    */
    while (next && (strcmp(next->d_name, ".") == 0 || strcmp(next->d_name, "..") == 0))
        next = readdir(directory);

    /*
    checks if there are no more files in the directory. if that is the case, 
    son is non-existant and we close the directory before returning the built meta_tree
    */
    if (next == NULL)
    {
        tree->son = NULL;
        closedir(directory);
        return tree;
    }

    /*
    copying path in a buffer to be able to add the new paths afterwards
    and to add a / char
    */
    char newpath[2048];
    strcpy(newpath, path);
    char *start;
    for (start = newpath; *start; start++);
    *start = '/';
    start++;
    
    /*
    copying the name of the first file found after the given path
    */
    char *p = start;
    char *q;
    for (q = next->d_name; *q; q++)
    {
        *p = *q;
        p++;
    }
    *p = 0;

    /*
    depending on the type of this first file, recursively creating its node
    */
    switch (next->d_type)
    {
        case DT_DIR:
            tree->son = sub_build_mtd(newpath);
            break;
        default:
            tree->son = sub_build_mti(newpath);
            break;
    }

    /*
    creating all of the nodes for the files in the directory
    and linking them to the current node
    */
    struct meta_tree *temp = tree->son;
    while ((next = readdir(directory)))
    {
        p = start;
        if (strcmp(next->d_name, ".") == 0 || strcmp(next->d_name, "..") == 0)
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
            default:
                temp->sibling = sub_build_mti(newpath);
                break;
        }
        temp = temp->sibling;
    }

    /*
    closing the directory
    */
    closedir(directory);
    
    return tree;
}

struct meta_tree *FILESYSTEM_build_metatree(char *path)
{
    /*
    declaring stats struct
    */
    struct stat data;

    /*
    getting stats of file and checking for errors
    */
    int er = stat(path, &data);
    if (er == -1)
        err(21, "FILESYSTEM: stat Failure.");

    /*
    allocating memory for sentinel
    */
    struct meta_tree *tree = calloc(1 , sizeof(struct meta_tree));
    
    /*
    depending on the type of the file, creating its node and linking it to the sentinel
    */
    if (S_ISREG(data.st_mode))
    {
        tree->son = sub_build_mti(path);
    }
    else if (S_ISDIR(data.st_mode))
    {
        tree->son = sub_build_mtd(path);
    }

    return tree;
}

void FILESYSTEM_free_metatree(struct meta_tree *tree)
{
    /*
    saving the son of the tree in a variable
    */
    struct meta_tree *temp1 = tree->son;
    struct meta_tree *temp2;

    /*
    while there is a son, free it, then go to its sibling
    */
    while (temp1)
    {
        temp2 = temp1->sibling;
        FILESYSTEM_free_metatree(temp1);
        temp1 = temp2;
    }

    /*
    if this is not the sentinel, data must be freed
    */
    if (tree->data)
    {
        /*
        freeing path memory and data memory
        */
        free(tree->data->path);
        free(tree->data);
    }

    /*
    freeing memory allocated for the node
    */
    free(tree);
}

