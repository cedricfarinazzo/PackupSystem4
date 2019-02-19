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

void print_freqlist(struct freqlist *freqList)
{
    struct element *test_freq = freqList->freq->first;
    struct element *test_car = freqList->car->first; 
    printf("FreqList = ");
    while (test_freq != NULL)
    {
        printf("<%d> <%d>", test_freq->key, test_car->key);
        test_freq = test_freq->next;
        test_car = test_car->next;
    }
    printf("\n");
}

void print_listes(struct liste *listee)
{
    struct element *test = listee->first;
    printf("Liste = ");
    while(test != NULL)
    {
        printf("<%d>", test->key);
        test = test->next;
    }
    printf("\n");
}

//Compression

struct freqlist* buildFrequenceList(char dataIN[])
{
    struct liste *charList = new_liste();
    struct liste *frequencyList = new_liste();
    int i = 0;
    struct element *ele;
    struct element *car;
    while (dataIN[i] != '\0')
    {
        ele = frequencyList->first;
        car = charList->first;
        while (car != NULL && car->key != dataIN[i])
        {
            car = car->next;
            ele = ele->next;
        }
        if (car != NULL)
        {
            ele->key += 1;
        }
        else
        {
            insert(charList, dataIN[i]);
            insert(frequencyList, 1);
        }
        i++;
    }
    struct freqlist *result = malloc(sizeof(struct freqlist));
    result->freq = frequencyList;
    result->car = charList;
    return result; 
}

void print_bintree(struct bintree *tree)
{
    if (tree != NULL)
    {
        char tmp = tree->key;
        printf("%d\n", tmp);
        printf("Passage a gauche\n");
        print_bintree(tree->left);
        printf("Passage a droite\n");
        print_bintree(tree->right);
    }
}

struct bintree *buildHuffmantree(struct freqlist *Freq)
{
    //Sorting liste
    struct liste *charsort = new_liste();
    struct liste *freqsort = new_liste();
    struct huffele *temp;
    while(Freq->freq->first != NULL)
    {
        temp = min_pop(Freq);
        printf("temp->car = %d\ntemp->freq = %d\n", temp->car, temp->freq);
        insert(charsort, temp->car);
        insert(freqsort, temp->freq);
    }
    free_freqlist(Freq);
    //Build huffmantree2
    struct bintree *H = new_tree(0);
    H->left = new_tree(0);
    H->right = new_tree(0);
    struct element *pointeur = charsort->last;
    if (pointeur->prec != NULL)
    {
        H->left->left = new_tree(pointeur->key);
        H->left->right = new_tree(pointeur->prec->key);
    }
    pointeur = pointeur->prec->prec;
    struct bintree *point_tree = H->right;
    while (pointeur != NULL && pointeur->prec != NULL)
    {
        point_tree->left = new_tree(0);
        point_tree->left->left = new_tree(pointeur->key);
        point_tree->left->right = new_tree(pointeur->prec->key);
        point_tree->right = new_tree(0);
        point_tree = point_tree->right;
        pointeur = pointeur->prec->prec;
    }
    if (pointeur != NULL)
    {
        point_tree->key = pointeur->key;
    }
    else
    {
        point_tree = H->right;
        while (point_tree->right->right->right != NULL)
        {
            point_tree = point_tree->right;
        }
        bin_free(point_tree->right->right);
        struct bintree *tmp = point_tree->right->left;
        free(point_tree->right);
        point_tree->right = tmp;
    }
    struct bintree *test = H;
    print_bintree(test);
    return H;
}
/*
void __table_codage(struct liste *prefixe, struct bintree *huffman,
        struct liste *table)
{
    if (huffman->right == NULL && huffman->left == NULL)
    {
        printf("Ici\n");
        insert(table, huffman->key);
        insertr(table, prefixe);
    }
    else
    {
        printf("La\n");
        insert(prefixe, 0);
        __table_codage(prefixe, huffman->left, table);
        insert(prefixe, 1);
        __table_codage(prefixe, huffman->right, table);
    }
}
*/
void __codage_table(struct liste *table, struct liste *prefixe,
        struct bintree *pt, char pref)
{
    insert(prefixe, pref);
    if (pt->left == NULL && pt->right == NULL)
    {
        insertr(table, prefixe);
        insert(table, pt->key);
        printf("Table = ");
        print_listes(table);
        del_last(prefixe);
    }
    else
    {
        __codage_table(table, prefixe, pt->left, 0);
        printf("Prefixe = ");
        print_listes(prefixe);
        //del_last(prefixe);
        __codage_table(table, prefixe, pt->right, 1);
        printf("Prefixe ante = \n");
        print_listes(prefixe);
        del_last(prefixe);
    }
}

void codage_table(struct bintree *huffman, struct liste *table)
{
    struct liste *prefixe = new_liste();
    struct bintree *pt = huffman;
    __codage_table(table, prefixe, pt, 0);
    print_listes(table);    
    liste_free(prefixe);
}

struct liste *encode_data(char dataIN[], char* table)
{
    if (dataIN == NULL)
    {
        errx(4, "The data is empty");
    }
    int len = strlen(table);
    int datalen = strlen(dataIN);
    int isOK = 1;
    int j = 0;
    char charactere = 'a';
    struct liste *text_encode = new_liste();
    char *binaire = malloc(sizeof(char) * 2);
    binaire[0] = '0';
    binaire[1] = '1';
    for (int i = 0; i < datalen; i++)
    {
        charactere = dataIN[i];
        while (j < len && isOK)
        {
            if (table[j] == charactere)
            {
                j++;
                while (table[j] == binaire[0] || table[j] == binaire[1])
                {
                    insert(text_encode, table[j]);
                    j++;
                }
                isOK = 0;
                j = 0;
            }
            else
            {
                j++;
            }
        }
    }
    return text_encode;
}

struct liste *_to_bin(char n)
{
    struct liste *output = new_liste();
    int i = 0;
    while (n != 0)
    {
        struct element *place = malloc(sizeof(struct element));
        if (n % 2 == 1)
        {
            place->key = 1;
        }
        else
        {
            place->key = 0;
        }
        place->prec = NULL;
        if (output->first == NULL)
        {
            output->first = place;
            output->last = place;
            place->next = NULL;
        }
        else
        {
            place->next = output->first;
            output->first = place;
            place->next->prec = place;
        }
        n = n / 2;
        i++;
    }
    while (i != 8)
    {
        struct element *zero = malloc(sizeof(struct element));
        zero->key = 0;
        zero->next = output->first;
        output->first->prec = zero;
        output->first = zero;
        i++;
    }
    return output;
}

void __codage_tree(struct liste *chaine, struct bintree *huffman)
{
    if (huffman->right == NULL && huffman->left == NULL)
    {
        insert(chaine, '1');
        insertr(chaine, _to_bin(huffman->key));
    }
    else
    {
        insert(chaine, '0');
        __codage_tree(chaine, huffman->left);
        __codage_tree(chaine, huffman->right);
    }
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        errx(EXIT_FAILURE, "2 args pls");
    }
    char *dataIN = argv[1];
    struct freqlist *freqList = buildFrequenceList(dataIN);
    if (freqList == NULL || freqList->freq->first == NULL ||
            freqList->freq->first == NULL)
    {
        errx(4, "FreqList is NULL");
    }
    printf("Freqlist success !\n");
    
    struct bintree *huffman = buildHuffmantree(freqList);
    if (huffman == NULL)
    {
        errx(4, "Huffman tree is NULL");
    }
    printf("Huffman tree build !\n");
    //struct liste *prefixe = new_liste();
    struct liste *table = new_liste();
    printf("Debut Table edit\n");
    codage_table(huffman, table);
    //__table_codage(prefixe, huffman, table);
    printf("table success !\n");
    printf("Table = %s\n", liste_to_string(table));
    //liste_free(prefixe);
    if (table == NULL || table->first == NULL)
    {
        errx(4, "table is NULL");
    }
    //liste chainee vers tableau pour la table
    char* static_table = liste_to_string(table);
    struct liste *encoding_data = new_liste();
    encoding_data = encode_data(dataIN, static_table);
    struct liste *encode_tree = new_liste();
    struct bintree *root = huffman;
    __codage_tree(encode_tree, root);
    free(root);
    char *dataenco = liste_to_string(encoding_data);
    char *treeenco = liste_to_string(encode_tree);
    printf("Data = %s\nTree = %s\n", dataenco, treeenco);
    return 0;
}
