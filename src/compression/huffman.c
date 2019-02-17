#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "bintree.h"
#include "liste.h"
#include "huffman.h"
#include "struct.h"

void free_freqlist(struct freqlist *Freqlist)
{
    if (Freqlist->freq != NULL)
    {
        liste_free(Freqlist->freq);
    }
    if (Freqlist->car != NULL)
    {
        liste_free(Freqlist->car);
    }
    free(Freqlist);
}

//Compression

struct freqlist* buildFrequenceList(char dataIN[])
{
    struct liste *charList = new_liste();
    struct liste *frequencyList = new_liste();
    int i = 0;
    struct element *ele;
    while (dataIN[i] != '\0')
    {
        ele = frequencyList->first;
        while (ele != NULL && ele->key != dataIN[i])
        {
            ele = ele->next;
        }
        if (ele != NULL)
        {
            ele->key += 1;
        }
        else
        {
            insert(charList, dataIN[i]);
            insert(frequencyList, 1);
        }
    }
    free(ele);
    struct freqlist *result = malloc(sizeof(struct freqlist));
    result->freq = frequencyList;
    result->car = charList;
    return result; 
}

/*
void max_list_insert_tree(struct freqlist Frequ, struct bintree huffman)
{
    struct element *frq = Frequ->freq->first;
//    struct element *car = Freq->car->first;
    char max = frq->key;
    while (frq->next != NULL)
    {
        if (frq->key > max)
        {
            max = frq->key;
        }
        frq = frq->next;
    }
    frq = Frequ->freq->first;
    while (frq->key != max)
    {
        frq = frq->next;
    }  
}*/

struct bintree *buildHuffmantree(struct freqlist *Freq)
{
    //Sorting liste
    struct liste *charsort = new_liste();
    struct liste *freqsort = new_liste();
    struct huffele *temp;
    while(Freq->freq->first != NULL)
    {
        temp = min_pop(Freq);
        insert(charsort, temp->car);
        insert(freqsort, temp->freq);
    }
    free_freqlist(Freq);
    //Build huffmantree
    struct bintree *T = new_tree(freqsort->last->key);
    struct bintree *A = new_tree(freqsort->last->key);
    struct bintree *B = new_tree(freqsort->last->prec->key);
    T->left = B;
    T->right = A;

    struct bintree *huffman = new_tree(0);
    struct element *act_car = freqsort->last->prec->prec;
    while (act_car->prec->prec != NULL)
    {
        B->key = T->key;
        B->left = T->left;
        B->right = T->right;
        A->key = act_car->key;
        T->left = A;
        T->right = B;
        act_car = act_car->prec;
    }
    huffman->right = T;
    B->key = freqsort->first->key;
    A->key = freqsort->first->next->key;
    B->left = NULL;
    B->right = NULL;
    T->left = B;
    T->right = A;
    huffman->left = T;
    return huffman;
}

void __table_codage(struct liste *prefixe, struct bintree *huffman,
        struct liste *table)
{
    if (huffman->right == NULL && huffman->left == NULL)
    {
        insert(table, huffman->key);
        char * tmp = liste_to_string(prefixe);
        insertr(table, tmp);
    }
    else
    {
        insert(prefixe, 0);
        __table_codage(prefixe, huffman->left, table);
        insert(prefixe, 1);
        __table_codage(prefixe, huffman->right, table);
    }
}
/*
void encode_data(struct bintree *huffman, char dataIN[],
        struct liste *table)
{
    if (huffman != NULL)
    {
        errx(4, "The tree is empty");
    }
    if (dataIN == NULL)
    {
        errx(4, "The data is empty");
    }
    int len = len_list(table);
    int isOK = 1;
    char charactere = "a";
    struct liste *text_encode = new_liste();
}*/

int principale(char dataIN[])
{
    struct freqlist *freqList = buildFrequenceList(dataIN);
    if (freqList == NULL || freqList->freq->first == NULL ||
            freqList->freq->first)
    {
        errx(4, "FreqList is NULL");
    }
    struct bintree *huffman = buildHuffmantree(&freqlist);
    if (huffman == NULL)
    {
        errx(4, "Huffman tree is NULL");
    }
    struct liste *prefixe = new_liste();
    struct liste *table = new_liste();
    __table_codage(prefixe, huffman, table);
    liste_free(prefixe);
    if (table == NULL || table->first == NULL)
    {
        errx(4, "table is NULL");
    }


    return 0;
}
