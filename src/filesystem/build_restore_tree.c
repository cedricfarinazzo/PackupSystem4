#include "build_restore_tree.h"

struct restore_tree *_init_restore_tree(struct meta_tree *mt, char *savepath)
{
    /*
    allocating node
    */

    struct restore_tree *tree = calloc(1, sizeof(struct restore_tree));
    
    /*
    checking if the node is the sentinel, if so do nothing else init data of restore tree
    */

    if (mt->data)
    {
        /*
        allocating data for the node
        */

        struct restore_data *data = malloc(sizeof(struct restore_data));

        /*
        saving mode, file and mtime in data
        */

        data->mode = mt->data->fs.st_mode;
        data->mtime = mt->data->fs.st_mtime;
        strcpy(data->file, mt->data->path);

        /*
        if meta tree was restored from a save and the file of the node was saved in this save, 
        save the name of it and the offset at which it was saved
        */

        if (mt->data->file_content != 0)
        {
            strcpy(data->src, savepath);
            data->offset = mt->data->file_content;
        }

        /*
        else just set the offset to 0
        */

        else
        {
            data->offset = 0;
        }

        /*
        link data to the node
        */

        tree->data = data;
    }

    /*
    if the node is representing a file, call recursively on his children
    */

    if (mt->son)
    {

        /*
        call recursively the first time
        */

        struct restore_tree *t = _init_restore_tree(mt->son, savepath);
        
        /*
        loop on all siblings
        */

        struct meta_tree *temp = mt->son->sibling;
        struct restore_tree *t2 = t;
        while (temp)
        {
            t2->sibling = _init_restore_tree(temp, savepath);
            t2 = t2->sibling;
            temp = temp->sibling;
        }

        /*
        link the children to the node
        */

        tree->son = t;
    }

    return tree;
}

struct meta_tree *_find_mt_node(struct restore_tree *rt, struct meta_tree *mt)
{
    /*
    get restore tree node filename
    */

    char *file = rt->data->file;
    
    /*
    loop on meta tree level until the right meta_tree node is found
    */

    while (mt)
    {
        /*
        if the meta tree node is found return it else continue
        */

        if (mt->data && strcmp(mt->data->path, file) == 0)
        {
            return mt;
        }

        mt = mt->sibling;
    }

    return NULL;
}

void _update_restore_tree(struct restore_tree *rt, struct meta_tree *mt, char *savepath)
{
    /*
    if there is a child, treat it and its siblings, no need to treat current node as it was taken care of
    in previous call.
    */

    if (rt->son)
    {
        /*
        loop on children of node
        */

        struct restore_tree *temp = rt->son;
        while (temp)
        {
            /*
            if the save is already saved, skip this rt node
            */

            if (temp->data->offset != 0)
            {
                temp = temp->sibling;
                continue;
            }

            /*
            find the meta tree node linked to this rt node through file name
            */

            struct meta_tree *tempmt = _find_mt_node(temp, mt->son);
            
            /*
            if it is found check if the file was saved in this save and call recursively on it
            */

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
    /*
    if the tree was initialized, call update, else call init
    */

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
    /*
    just free the memory space
    */

    free(data);
}

void RS_free_restore_tree(struct restore_tree *rt)
{
    /*
    if not in the sentinel, free data
    */

    if (rt->data)
        _free_restore_data(rt->data);

    /*
    if there are children, free them recursively
    */

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

    /*
    free node space
    */

    free(rt);
}
