#include "build_restore_tree.h"

struct restore_tree *_init_restore_tree(struct meta_tree *mt, char *savepath)
{
    struct restore_tree *tree = calloc(1, sizeof(struct restore_tree));
    if (mt->data)
    {
        struct restore_data *data = malloc(sizeof(struct restore_data));
        data->mode = mt->data->fs.st_mode;
        data->mtime = mt->data->fs.st_mtime;
        strcpy(data->file, mt->data->path);
        if (mt->data->file_content != 0)
        {
            strcpy(data->src, savepath);
            data->offset = mt->data->file_content;
        }
        else
        {
            data->offset = 0;
        }
        tree->data = data;
    }
    if (mt->son)
    {
        struct restore_tree *t = _init_restore_tree(mt->son, savepath);
        struct meta_tree *temp = mt->son->sibling;
        struct restore_tree *t2 = t;
        while (temp)
        {
            t2->sibling = _init_restore_tree(temp, savepath);
            t2 = t2->sibling;
            temp = temp->sibling;
        }
        tree->son = t;
    }
    return tree;
}

struct meta_tree *_find_mt_node(struct restore_tree *rt, struct meta_tree *mt)
{
    char *file = rt->data->file;
    while (mt)
    {
        if (strcmp(mt->data->path, file))
        {
            return mt;
        }
        mt = mt->sibling;
    }
    return NULL;
}

void _update_restore_tree(struct restore_tree *rt, struct meta_tree *mt, char *savepath)
{
   if (rt->son)
   {
       struct restore_tree *temp = rt->son;
       while (temp)
       {
           if (temp->data->offset)
               continue;
           struct meta_tree *tempmt = _find_mt_node(temp, mt);
           if (tempmt)
           {
               if (tempmt->data->file_content)
               {
                   temp->data->offset = tempmt->data->file_content;
                   strcpy(temp->data->src, savepath);
               }
               _update_restore_tree(temp, tempmt, savepath);
           }
           temp = temp->sibling;
       }
   }
}

void RS_update_restore_tree_from_mt(struct restore_tree *rt, struct meta_tree *mt, char *savepath)
{
    if (rt->son == NULL)
    {
        rt->son = _init_restore_tree(mt->son, savepath);
    }
    else
    {
        _update_restore_tree(rt, mt, savepath);
    }
}

void _free_restore_data(struct restore_data *data)
{
    free(data);
}

void RS_free_restore_tree(struct restore_tree *rt)
{
    if (rt->data)
        _free_restore_data(rt->data);
    if (rt->son)
    {
        struct restore_tree *child = rt->son;
        struct restore_tree *temp;
        while (child)
        {
            temp = child->sibling;
            RS_free_restore_tree(child);
            child = temp;
        }
    }
    free(rt);
}
