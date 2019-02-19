#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "bintree.h"
#include "liste.h"
#include "huffman.h"
#include "struct.h"
#include <math.h>

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

void print_chare(char *output, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d", output[i]);
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
    //struct bintree *test = H;
    //print_bintree(test);
    return H;
}

void __codage_table(struct liste *table, struct liste *prefixe,
        struct bintree *pt, char pref)
{
    insert(prefixe, pref);
    if (pt->left == NULL && pt->right == NULL)
    {
        insert(table, pt->key);
        inserts(table, prefixe, 2);
        del_last(prefixe);
    }
    else
    {
        __codage_table(table, prefixe, pt->left, 0);
        __codage_table(table, prefixe, pt->right, 1);
        del_last(prefixe);
    }
}

void codage_table(struct bintree *huffman, struct liste *table)
{
    struct liste *prefixe = new_liste();
    struct bintree *pt = huffman;
    __codage_table(table, prefixe, pt, 0);
    liste_free(prefixe);
}

struct liste *encode_data(char dataIN[], char* table, int len)
{
    if (dataIN == NULL)
    {
        errx(4, "The data is empty");
    }
    int datalen = strlen(dataIN);
    int isOK = 1;
    int j = 0;
    char charactere = 'a';
    struct liste *text_encode = new_liste();
    for (int i = 0; i < datalen; i++)
    {
        charactere = dataIN[i];
        isOK = 1;
        while (j < len && isOK)
        {
            if (table[j] == charactere)
            {
                j++;
                while (j < len && (table[j] == 0 || table[j] == 1))
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
        output->first->prec = malloc(sizeof(struct element));
	output->first->prec->key = 0;
	output->first->prec->prec = NULL;
	output->first->prec->next = output->first;
	output->first = output->first->prec;
	++i;
    }
    return output;
}

void __codage_tree(struct liste *table, struct liste *prefixe,
        struct bintree *pt, char pref)
{
  
    insert(prefixe, pref);
    if (pt->left == NULL && pt->right == NULL)
    {
	struct liste *temp = _to_bin(pt->key);
        insertr(table, temp);
	liste_free(temp);
        inserts(table, prefixe, 2);
        del_last(prefixe);
    }
    else
    {
        __codage_tree(table, prefixe, pt->left, 0);
        __codage_tree(table, prefixe, pt->right, 1);
        del_last(prefixe);
	
    }
}

void codage_tree(struct bintree *huffman, struct liste *table)
{
    struct liste *prefixe = new_liste();
    struct bintree *pt = huffman;
    __codage_tree(table, prefixe, pt, 0);
    liste_free(prefixe);
}

int max_prof(struct bintree *H)
{
    int max = 0;
    while(H->right != NULL)
    {
        ++max;
	H = H->right;
    }
    return max;
}

void output_tree(struct liste *table, struct encod_tree *output, struct bintree *H)
{
    int len_data = len_list(table);
    if (len_data % 8 != 0){len_data = (len_data / 8) + 1;}
    else {len_data = len_data / 8;}
    char *data = calloc(len_data + 1, sizeof(char));
    char *buf = malloc(sizeof(char) * 8);
    char tmp = 0;
    int i = 0;
    int c = 0;
    struct element *actual = table->first;
    while (actual != NULL)
    {
        
        if (i == 8)
	{
    	    --i;
	    for (int j = 0; j < 8; ++j)
	    {
	        tmp += (pow(2, j) * buf[i]);
		--i;
	    }
	    data[c] = tmp;
	    ++c;
	    i = 0;
	}
	buf[i] = actual->key;
	++i;
	actual = actual->next;
    }
    --i;
    char align = 0;
    while (i != 8)
    {
        ++align;
        buf[i] = 0;
	++i;
    }
    --i;
    for (int j = 0; j < 8; ++j)
    {
        tmp += (pow(2, j) * buf[i]);
        --i;
    }
    output->align = align;
    output->prof = max_prof(H);
    output->len = len_data;
    output->data = data;
}

void output_data(struct liste *datai, struct encod_data *output)
{
    //Char to full bin
    struct element *actual = datai->first;
    struct element *temporaire = datai->first;
    struct liste *liste_temp;
    while (actual != NULL)
    {
        if (actual->key != 0 && actual->key != 1)
	{
            liste_temp = _to_bin(actual->key);
	    temporaire = actual->next;
	    actual->prec->next = liste_temp->first;
	    actual->next->prec = liste_temp->last;
	    liste_temp->first->prec = actual->prec;
	    liste_temp->last->next = actual->next;
            actual->next = NULL;
	    actual->prec = NULL;
	    free(actual);
	    actual = temporaire;
	}
	else
	{
	    actual = actual->next;
	}
    }
    int len_data = len_list(datai);
    if (len_data % 8 != 0){len_data = (len_data / 8) + 1;}
    else {len_data = len_data / 8;}
    char *data = calloc(len_data + 1, sizeof(char));
    char *buf = malloc(sizeof(char) * 8);
    char tmp = 0;
    int i = 0;
    int c = 0;
    actual = datai->first;
    while (actual != NULL)
    {
        
        if (i == 8)
	{
    	    --i;
	    for (int j = 0; j < 8; ++j)
	    {
	        tmp += (pow(2, j) * buf[i]);
		--i;
	    }
	    data[c] = tmp;
	    ++c;
	    i = 0;
	}
	buf[i] = actual->key;
	++i;
	actual = actual->next;
    }
    --i;
    char align = 0;
    while (i != 8)
    {
        ++align;
        buf[i] = 0;
	++i;
    }
    --i;
    for (int j = 0; j < 8; ++j)
    {
        tmp += (pow(2, j) * buf[i]);
        --i;
    }
    output->align = align;
    output->len = len_data;
    output->data = data;
}

int un_truc_explixcite(char *dataIN[])
{
    //Debut Freqlist
    struct freqlist *freqList = buildFrequenceList(dataIN);
    //Test exception
    if (freqList == NULL || freqList->freq->first == NULL ||
            freqList->freq->first == NULL)
    {
        errx(4, "FreqList is NULL");
    }
    //Fin Freqlist

    //Debut Bintree
    struct bintree *huffman = buildHuffmantree(freqList);
    //Test exception
    if (huffman == NULL)
    {
        errx(4, "Huffman tree is NULL");
    }
    //Fin Bintree

    //Debut table
    struct liste *table = new_liste();
    codage_table(huffman, table);
    //Test exception
    if (table == NULL || table->first == NULL)
    {
        errx(4, "table is NULL");
    }
    //Fin table

    //Passage de la table en static
    int len_table = len_list(table);
    char* static_table = liste_to_string(table);
    //Fin table statique

    //Debut encodage donnees
    struct liste *encoding_data = new_liste();
    encoding_data = encode_data(dataIN, static_table, len_table);
    //Fin encodage donnees

    //Debut encodage arbre
    struct liste *encoding_tree = new_liste();
    codage_tree(huffman, encoding_tree);
    //Convert bin vers char
    struct encod_tree *char_tree = malloc(sizeof(struct encod_tree));
    output_tree(encoding_tree, char_tree, huffman);
    //Fin encodage arbre

    //Output Data
    struct encod_data *char_data = malloc(sizeof(struct encod_data));
    output_data(encoding_data, char_data);

    //Rendu soutenance
    printf("\n\n");
    printf("Input data = %s\n", dataIN);
    printf("Output encoding data = %s\n", char_data->data);
    printf("Output encoding tree = %s\n", char_tree->data);

    int ratio = strlen(char_data->data) + strlen(char_tree->data);
    ratio *= 100;
    ratio = ratio / strlen(dataIN);
    printf("Ratio = %d%%\n", ratio);

    //Deallocation de toutes les struct
    bin_free(huffman);
    liste_free(table);
    liste_free(encoding_data);
    liste_free(encoding_tree);
    free(char_tree->data);
    free(char_tree);
    free(char_data->data);
    free(char_data);
  
    return 0;
}
