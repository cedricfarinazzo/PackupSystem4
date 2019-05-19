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
    cr_assert(data1->fs.st_size == data2->fs.st_size);
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

void remove_dir2()
{
    remove("./testfiles/content2/subdir01/subdir03/file06");
    rmdir("./testfiles/content2/subdir01/subdir03");
    remove("./testfiles/content2/subdir01/file04");
    remove("./testfiles/content2/subdir01/file05");
    rmdir("./testfiles/content2/subdir01");
    remove("./testfiles/content2/subdir02/file07");
    rmdir("./testfiles/content2/subdir02");
    remove("./testfiles/content2/file01");
    remove("./testfiles/content2/file02");
    remove("./testfiles/content2/file03");
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
    print_tree(tree->son, 0);
    cr_assert_not_null(tree);
    FILESYSTEM_create_save("./testfiles/content", "./testfiles/saves/save.rdtgs");
    fileexists("./testfiles/saves/save.rdtgs");
    struct stat fs;
    int e __attribute__((unused)) = stat("./testfiles/saves/save.rdtgs", &fs);
    printf("Size of save: %ld Bytes\n", fs.st_size);
    struct meta_tree *tree2 = FILESYSTEM_SAVE_restore_metatree_from_save("./testfiles/saves/save.rdtgs");
    cmp_tree(tree->son, tree2->son);
    print_tree(tree2->son, 0);
    remove_dir();
    FILESYSTEM_free_metatree(tree2);
    FILESYSTEM_restore_original_save("./testfiles/saves/save.rdtgs");
    fileexists("./testfiles/content/file1");
    fileexists("./testfiles/content/file2");
    fileexists("./testfiles/content/file3");
    fileexists("./testfiles/content/sub1/file4");
    fileexists("./testfiles/content/sub1/file5");
    fileexists("./testfiles/content/sub2/file6");
    fileexists("./testfiles/content/sub2/file7");
    struct meta_tree *tree3 = FILESYSTEM_build_metatree("./testfiles/content");
    cmp_tree(tree->son, tree3->son);
    print_tree(tree3->son, 0);
    FILESYSTEM_free_metatree(tree);
    FILESYSTEM_free_metatree(tree3);
}

Test(FILESYSTEM, create_saves)
{
    struct meta_tree *tree = FILESYSTEM_build_metatree("./testfiles/content2");
    print_tree(tree->son, 0);
    cr_assert_not_null(tree);
    FILESYSTEM_free_metatree(tree);
    printf("DEBUG: Tree Built.");
    FILESYSTEM_create_save("./testfiles/content2", "./testfiles/saves2/save01");
    printf("DEBUG: First save created.");
    FILE *file01 = fopen("./testfiles/content2/file01", "a");
    fwrite("I love bananas.\n\n\n",1, 15, file01);
    fclose(file01);
    printf("DEBUG: first file modified.");
    struct meta_tree *tree01 = FILESYSTEM_build_metatree("./testfiles/content2");
    print_tree(tree01->son, 0);
    FILESYSTEM_create_new_save("./testfiles/content2", "./testfiles/saves2/save02", "./testfiles/saves2/save01");
    struct meta_tree *tree01_r = FILESYSTEM_SAVE_restore_metatree_from_save("./testfiles/saves2/save02");
    cmp_tree(tree01->son, tree01_r->son);
    FILESYSTEM_free_metatree(tree01);
    FILESYSTEM_free_metatree(tree01_r);
    printf("DEBUG: second save created.");
    FILE *file03 = fopen("./testfiles/content2/subdir01/file03", "w");
    fwrite("Jussieux.", 1, 9, file03);
    fclose(file03);
    printf("DEBUG: second file modified.");
    struct meta_tree *tree02 = FILESYSTEM_build_metatree("./testfiles/content2");
    print_tree(tree02->son, 0);
    FILESYSTEM_create_new_save("./testfiles/content2", "./testfiles/saves2/save03", "./testfiles/saves2/save02");
    struct meta_tree *tree02_r = FILESYSTEM_SAVE_restore_metatree_from_save("./testfiles/saves2/save03");
    cmp_tree(tree02->son, tree02_r->son);
    FILESYSTEM_free_metatree(tree02_r);
    printf("DEBUG: third save created.");
    remove_dir2();
    FILESYSTEM_restore_save("./testfiles/saves2");
    printf("DEBUG: restoration done");
    struct meta_tree *f_tree = FILESYSTEM_build_metatree("./testfiles/content2");
    cmp_tree(tree02->son, f_tree->son);
    print_tree(f_tree->son, 0);
    FILESYSTEM_free_metatree(tree02);
    FILESYSTEM_free_metatree(f_tree);
}
