#include <stdlib.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "../src/filesystem/build_metatree.h"

void print_tree(struct meta_tree *tree, int indent)
{
    char indents[indent + 1];
    for (int i = 0; i < indent; ++i)
        indents[i] = ' ';
    indents[indent] = 0;

    if (tree != NULL)
    {
        printf("%spath:%s | ", indents, tree->data->path);
        printf("size:%ld o | ",  tree->data->fs.st_size);
        printf("mode: %d | ", tree->data->fs.st_mode);
        printf("\n");

        struct meta_tree *c = tree->son;
        while (c)
        {
            print_tree(c, indent + 6);
            c = c->sibling;
        }
    }
}

int cmp_data(struct meta_data *data1, struct meta_data *data2)
{
    int cmp = strcmp(data1->path, data2->path);
    cmp = cmp && (data1->fs.st_atime == data2->fs.st_atime);
    return cmp;
}

int cmp_tree(struct meta_tree *tree1, struct meta_tree *tree2)
{
    int cmp = cmp_data(tree1->data, tree2->data);
    cmp = cmp && (cmp_tree(tree1->son, tree2->son));
    cmp = cmp && (cmp_tree(tree1->sibling, tree2->sibling));
    return cmp;
}

Test(FILESYSTEM, Build_Tree)
{
    struct meta_tree *tree = FILESYSTEM_build_metatree(".");
    print_tree(tree->son, 0);
    printf("First tests done.\n");
    FILESYSTEM_save_metatree(tree, "savedtree");
    printf("Tree saved.\n");
    struct meta_tree *restored = FILESYSTEM_restore_metatree("savedtree");
    printf("Tree restored.\n");
    int result = cmp_tree(tree, restored);
    if (result)
        printf("Tree successfully saved and restored.\n");
    else
        printf("Tree failed to be saved and restored.\n");
    FILESYSTEM_free_metatree(tree);
    FILESYSTEM_free_metatree(restored);
}
