#include <stdlib.h>
#include <stdio.h>

#include <criterion/criterion.h>

#include "../src/filesystem/build_metatree.h"
#include "../src/filesystem/save_metatree.h"
#include "../src/filesystem/restore_save.h"
#include "../src/filesystem/create_save.h"

void teardown(void) {
    remove("test_tree_savetree.txt");
    remove("test_tree_restoretree.txt");
}

void fileexists(const char * filename)
{
    FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fclose(file);
        return;
    } 
    cr_assert_fail("File not found %s", filename);
}

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

void cmp_data(struct meta_data *data1, struct meta_data *data2)
{
    cr_assert_str_eq(data1->path, data2->path);
    cr_assert(data1->fs.st_atime == data2->fs.st_atime);
}

void cmp_tree(struct meta_tree *tree1, struct meta_tree *tree2)
{
    if (tree1->data != NULL || tree2->data != NULL)
    {
        cr_assert_not_null(tree1->data, "tree1->data is null");
        cr_assert_not_null(tree2->data, "tree2->data is null");
        cmp_data(tree1->data, tree2->data);
    
    }
    if (tree1->son != NULL || tree2->son != NULL)
    {
        cr_assert_not_null(tree1->son, "tree1->son is null");
        cr_assert_not_null(tree2->son, "tree2->son is null");
        cmp_tree(tree1->son, tree2->son);
    
    }
    if (tree1->sibling != NULL || tree2->sibling != NULL)
    {
        cr_assert_not_null(tree1->sibling, "tree1->sibling is null");
        cr_assert_not_null(tree2->sibling, "tree2->sibling is null");
        cmp_tree(tree1->sibling, tree2->sibling);
    }
}

void remove_dir()
{
    remove("./testfiles/content/sub1/file4");
    remove("./testfiles/content/sub1/file5");
    rmdir("./testfiles/content/sub1");
    remove("./testfiles/content/sub2/file6");
    remove("./testfiles/content/sub2/file7");
    rmdir("./testfiles/content/sub2");
    remove("./testfiles/content/file1");
    remove("./testfiles/content/file2");
    remove("./testfiles/content/file3");
    rmdir("./testfiles/content");
}

Test(FILESYSTEM, Build_Tree)
{
    struct meta_tree *tree = FILESYSTEM_build_metatree(".");
    cr_assert_not_null(tree);
    print_tree(tree->son, 0);
    FILESYSTEM_free_metatree(tree);
}

Test(FILESYSTEM, save_tree)
{
    struct meta_tree *tree = FILESYSTEM_build_metatree(".");
    cr_assert_not_null(tree);
    FILESYSTEM_save_metatree(tree, "test_tree_savetree.txt");
    fileexists("test_tree_savetree.txt");
    FILESYSTEM_free_metatree(tree);
}

Test(FILESYSTEM, restore_tree)
{
    struct meta_tree *tree = FILESYSTEM_build_metatree(".");
    cr_assert_not_null(tree);
    FILESYSTEM_save_metatree(tree, "test_tree_restoretree.txt");
    fileexists("test_tree_restoretree.txt");
    struct meta_tree *restored = FILESYSTEM_restore_metatree("test_tree_restoretree.txt");
    cr_assert_not_null(restored);
    cmp_tree(tree->son, restored->son);
    FILESYSTEM_free_metatree(restored);
    FILESYSTEM_free_metatree(tree);
}

Test(FILESYSTEM, create_save)
{
    struct meta_tree *tree = FILESYSTEM_build_metatree("./testfiles/content");
    cr_assert_not_null(tree);
    /*FILESYSTEM_create_save("./testfiles/content", "./testfiles/saves/save.rdtgs");
    fileexists("./testfiles/saves/save.rdtgs");
    FILE *save = fopen("./testfiles/saves/save.rdtgs", "r");
    struct meta_tree *tree2 = FILESYSTEM_SAVE_restore_metatree_from_save(save);
    fclose(save);
    cmp_tree(tree, tree2);
    remove_dir();
    FILESYSTEM_free_metatree(tree);
    FILESYSTEM_free_metatree(tree2);
    FILESYSTEM_restore_original_save("./testfiles/saves/save.rdtgs");
    fileexists("./testfiles/content/file1");
    fileexists("./testfiles/content/file2");
    fileexists("./testfiles/content/file3");
    fileexists("./testfiles/content/sub1/file4");
    fileexists("./testfiles/content/sub1/file5");
    fileexists("./testfiles/content/sub2/file6");
    fileexists("./testfiles/content/sub2/file7");*/
}
