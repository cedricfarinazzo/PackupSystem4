#include <err.h>
#include <stdlib.h>

#include "bintree.h"

struct bintree *new_tree(char n) {
    struct bintree *b_new = malloc(sizeof(struct bintree));
    if (b_new != NULL)
    {
        b_new->key = n;
        b_new->left = NULL;
        b_new->right = NULL;
    }
    else
    {
        errx(1, "No free memory !");
    }
    return b_new;
}

struct binlist *new_binlist()
{
    struct binlist *l_new = malloc(sizeof(struct binlist));
    if (l_new != NULL)
    {
        l_new->first = NULL;
    }
    else
    {
        errx(1, "No free memory !");
    }
}

void insert_right(struct bintree *b, char n) {
    struct bintree *b_right = malloc(sizeof(struct bintree));
    if (b_right != NULL)
    {
        b_right->key = n;
        b_right->left = NULL;
        b_right->right = NULL;
    }
    else
    {
        errx(1, "No free memory !");
    }
    b->right = b_right;
}

void insert_left(struct bintree *b, char n)
{
    struct bintree *b_left = malloc(sizeof(struct bintree));
    if (b_left != NULL)
    {
        b_left->key = n;
        b_left->left = NULL;
        b_left->right = NULL;
    }
    else
    {
        errx(1, "No free memory !");
    }
    b->left = b_left;
}

void insert_binlist(struct binlist *l, struct bintree *b)
{
    struct binele *e_new = malloc(sizeof(struct binele));
    if (e_new == NULL)
    {
        errx(1, "No free memory !");
    }
    e_new->key = b;
    e_new->next = NULL;
    struct binele *actual = l->first;
    while (actual->next != NULL)
    {
        actual = actual->next;
    }
    actual->next = e_new;
    free(actual);
}

void bin_free(struct bintree *b)
{
    if (b->left != NULL) {
        bin_free(b->left);
    }
    if (b->right != NULL) {
        bin_free(b->right);
    }
    free(b);
}

void binele_free(struct binele *e)
{
    if (e->next != NULL)
    {
        binele_free(e->next);
    }
    free(e);
}

void binlist_free(struct binlist *b)
{
    if (b->first != NULL)
    {
        binele_free(b->first);
    }
    free(b);
}
