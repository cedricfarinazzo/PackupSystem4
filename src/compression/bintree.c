#include <err.h>
#include <stdlib.h>

bintree new_tree(char n) {
    bintree *b_new = malloc(sizeof(struct bintree));
    if (b_new != NULL)
    {
        b_new->key = n;
        b_new->left = NULL;
        b_new->right = NULL;
    }
    else
    {
        errx(1, "No free memory !")
    }
    return b_new;
}

bintree insert_right(bintree b, char n) {
    bintree *b_right = malloc(sizeof(struct bintree));
    if (b_right != NULL)
    {
        b_right->key = n;
        b_right->left = NULL;
        b_right->right = NULL;
    }
    else
    {
        errx(1, "No free memory !")
    }
    b->right = b_right;
    return b;
}

bintree insert_left(bintree b, char n)
{
    bintree *b_left = malloc(sizeof(struct bintree));
    if (b_left != NULL)
    {
        b_left->key = n;
        b_left->left = NULL;
        b_left->right = NULL;
    }
    else
    {
        errx(1, "No free memory !")
    }
    b->left = b_left;
    return b;
}

void bin_free(bintree b)
{
    if (b->left != NULL) {
        bin_free(b->left);
    }
    if (b->right != NULL) {
        bin_free(b->right);
    }
    free(b);
}
