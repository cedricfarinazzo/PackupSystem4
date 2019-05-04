#include "restore_save.h"
#include "create_save.h"
#include <utime.h>

char *RS_restore_path(FILE *file)
{
    /*
    reads the len of the saved path, reads said path and return it
    */
    long length;
    int e = fread(&length, sizeof(long), 1, file);
    if (e < 1)
        err(33, "RS_restore_path, failed to read length");

    char *path = malloc(length * sizeof(char));
    fread(path, sizeof(char), length, file);

    return path;
}

struct stat RS_restore_stats(FILE *file)
{
    /*
    reads in the file the saved stat struct and returns it
    */
    struct stat fs;
    int e = fread(&fs, sizeof(struct stat), 1, file);
    if (e < 1)
        err(34, "RS_restore_stats: failed to read stats");
    return fs;
}

off_t RS_skip_file_content(FILE *file)
{
    /*
    gets current offset to return it
    reads length of saved content
    if 0: returns 0, nothing to be skipped
    else: skips file content and returns saved offset
    */
    off_t offset = ftell(file);
    size_t length;
    fread(&length, sizeof(size_t), 1, file);

    fseek(file, length, SEEK_CUR);
    return offset;
}

struct meta_data *RS_restore_data(FILE *file)
{
    /*
    first: allocates memory for meta_data struct
    second: restores path
    third: restores stats
    fourth: gets offset for file_content and skips file_content
    fifth: returns pointer to allocated memory
    */
    char *path = RS_restore_path(file);

    struct stat fs = RS_restore_stats(file);

    struct meta_data *data = malloc(sizeof(struct meta_data));

    data->path = path;
    data->fs = fs;

    return data;
}

char RS_restore_inheritance(FILE *file)
{
    /*
    reads the inheritance byte and returns it
    */
    char inheritance;
    fread(&inheritance, sizeof(char), 1, file);

    return inheritance;
}

struct meta_tree *RS_restore_tree(FILE *file)
{
    /*
    first: restore data
    second: get inheritance byte
    third: get offset for file content
    fourth: recursive calls
    */
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));

    tree->data = RS_restore_data(file);

    char inheritance = RS_restore_inheritance(file);
    
    tree->data->file_content = RS_skip_file_content(file);

    if (inheritance & FILESYSTEM_TREE_HAS_SON)
        tree->son = RS_restore_tree(file);
    
    if (inheritance & FILESYSTEM_TREE_HAS_SIBLING)
        tree->sibling = RS_restore_tree(file);

    return tree;
}

struct meta_tree *FILESYSTEM_SAVE_restore_metatree_from_save(char *save)
{
    struct meta_tree *tree = calloc(1, sizeof(struct meta_tree));

    FILE *file = fopen(save, "r");
    tree->son = RS_restore_tree(file);
    fclose(file);

    return tree;
}

void chained_insert(struct chained *list, char *file)
{
    struct stat fs;
    stat(file, &fs);
    struct chained *node = malloc(sizeof(struct chained));
    strcpy(node->path, file);
    node->mtime = fs.st_mtime;
    struct chained *temp = list;
    while (temp->next)
    {
        if (temp->next->mtime > node->mtime)
        {
            break;
        }
        temp = temp->next;
    }
    node->next = temp->next;
    temp->next = node;
}

struct chained *RS_create_save_list(char *save_dir)
{
    DIR *saves = opendir(save_dir);
    struct dirent *next;
    char nextname[2048];
    strcpy(nextname, save_dir);
    char *start = nextname + strlen(save_dir);
    struct chained *list = calloc(1, sizeof(struct chained));
    while ((next = readdir(saves)))
    {
        switch (next->d_type)
        {
            case DT_REG:
                strcpy(start, next->d_name);
                chained_insert(list, nextname);
                break;
            default:
                break;
        }
    }
    closedir(saves);
    return list;
}

void RS_free_save_list(struct chained *list)
{
    if (list->next)
    {
        RS_free_save_list(list->next);
    }
    free(list);
}

void RS_restore_content(FILE *src, off_t offset, FILE *dst)
{
    fseek(src, offset, SEEK_SET);

    size_t length;
    fread(&length, sizeof(size_t), 1, src);

    char buf[2048];
    int r;
    int w;
    size_t total = 0;
    
    while (total < length)
    {
        if (total + 2048 > length)
        {
            r = fread(buf, sizeof(char), length - total, src);
            w = fwrite(buf, sizeof(char), r, dst);
            if (w < r)
                err(33, "RS_restore_content: failed to write all bytes");
            break;
        }
        r = fread(buf, sizeof(char), 2048, src);
        w = fwrite(buf, sizeof(char), r, dst);
        if (w < r)
            err(33, "RS_restore_content: failed to write all bytes");
        total += r;
    }
}

void RS_restore_from_restore_tree(struct restore_tree *tree)
{
    if (tree->son == NULL)
    {
        FILE *src = fopen(tree->data->src);
        FILE *dst = fopen(tree->data->file, "w");
        RS_restore_content(src, offset, dst);
        fclose(src);
        fclose(dst);
        chmod(tree->data->file, tree->data->mode);
        struct utimbuf buf;
        buf.actime = time(NULL);
        buf.modtime = tree->data->mtime;
        utime(tree->data->file, buf);
    }
    else
    {
        mkdir(tree->data->file, tree->data->mode);
        struct restore_tree *temp = tree->son;
        while (temp)
        {
            RS_restore_from_restore_tree(temp);
            temp = temp->sibling;
        }
    }
}

void FILESYSTEM_restore_save(char *savedir)
{
    struct chained *list = RS_create_save_list(savedir);
    struct chained *temp = list->next;
    struct restore_tree *rt = calloc(1, sizeof(struct restore_tree));
	while (temp)
    {
        struct meta_tree *temptree = FILESYSTEM_SAVE_restore_metatree_from_save(temp->path);
        RS_update_restore_tree_from_mt(rt, temptree, temp->path);
        FILESYSTEM_free_metatree(temptree);
        temp = temp->next;
    }
    RS_free_save_list(list);
    RS_restore_from_restore_tree(rt);
    RS_free_restore_tree(rt);
}

void RS_restore_from_meta_tree(struct meta_tree *tree, FILE *src)
{
    if (tree->son == NULL)
    {
        FILE *dst = fopen(tree->data->path, "w");
        RS_restore_content(src, tree->data->file_content, dst);
        fclose(dst);
    }
    else
    {
        mkdir(tree->data->path, 0666);
        struct meta_tree *temp = tree->son;
        while (temp)
        {
            RS_restore_from_meta_tree(temp, src);
            temp = temp->sibling;
        }
    }
}

void FILESYSTEM_restore_original_save(char *save)
{
    struct meta_tree *tree = FILESYSTEM_SAVE_restore_metatree_from_save(save);
    FILE *file = fopen(save, "r");
    RS_restore_from_meta_tree(tree->son, file);
    fclose(file);
    FILESYSTEM_free_metatree(tree);
}
