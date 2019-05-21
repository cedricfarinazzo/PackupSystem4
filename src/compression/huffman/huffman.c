#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include "../tree/bintree.h"
#include "../liste/liste.h"
#include "huffman.h"
#include "../struct.h"
#include "simple/four_compression.h"
#include "simple/four_decompression.h"
#include <math.h>

#define LEN_DATA 5

void free_out(struct huff_out *data)
{
    free(data->dataOUT);
    free(data);
}

void free_freqlist(struct freqlist *Freqlist)
{
    if (Freqlist->freq != NULL)
    {
        free_list_int(Freqlist->freq);
    }
    if (Freqlist->car != NULL)
    {
        liste_free(Freqlist->car);
    }
    free(Freqlist);
}

void print_freqlist(struct freqlist *freqList)
{
    struct ele_int *test_freq = freqList->freq->first;
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

void print_chare(unsigned char *output, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d ", output[i]);
    }
    printf("\n");
}

void strcpyh(unsigned char *dest, const unsigned char *src, int n)
{
    for (int i = 0; i < n; ++i)
    {
        dest[i] = src[i];
        if (dest[i] != src[i])
            errx(EXIT_FAILURE, "Le caractere n'a pas ete deplace!");
    }
}

//Compression

struct freqlist* buildFrequenceList(unsigned char dataIN[], int len)
{
    struct liste *charList = new_liste();
    struct list_int *frequencyList = new_list();
    int i = 0;
    struct ele_int *ele;
    struct element *car;
    while (i < len)
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
            insertint(frequencyList, 1);
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
        unsigned char tmp = tree->key;
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
    struct huffele *temp = malloc(sizeof(struct huffele));;
    while(Freq->freq->first != NULL)
    {
        min_pop(Freq, temp);
        insert(charsort, temp->car);
        insert(freqsort, temp->freq);
    }
    free_freqlist(Freq);
    //Build huffmantree
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
    free(temp);
    liste_free(freqsort);
    liste_free(charsort);
    return H;
}

void __codage_table(struct liste *table, struct liste *prefixe,
        struct bintree *pt, unsigned char pref)
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

struct liste *encode_data(unsigned char dataIN[], unsigned char* table, int len,
        int datalen)
{
    if (dataIN == NULL)
    {
        errx(4, "The data is empty");
    }
    int isOK = 1;
    int j = 0;
    unsigned char charactere = 'a';
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
        struct bintree *pt, unsigned char pref)
{
    insert(prefixe, pref);
    if (pt->left == NULL && pt->right == NULL)
    {
        struct liste *temp = _to_bin(pt->key);
        insertr(table, temp);
        insertr(table, prefixe);
        liste_free(temp);
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
    __codage_tree(table, prefixe, pt->left, 0);
    struct liste *prefixe2 = new_liste();
    __codage_tree(table, prefixe2, pt->right, 1);
    liste_free(prefixe);
    liste_free(prefixe2);
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
    unsigned char *data = calloc(len_data + 1, sizeof(unsigned char));
    unsigned char *buf = malloc(sizeof(unsigned char) * 8);
    unsigned char tmp = 0;
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
                buf[i] = 0;
                --i;
            }
            data[c] = tmp;
            ++c;
            tmp = 0;
            i = 0;
        }
        buf[i] = actual->key;
        ++i;
        actual = actual->next;
    }
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
    data[c] = tmp;
    output->align = align;
    output->prof = max_prof(H);
    output->len = len_data;
    output->data = data;
    free(buf);
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
    output->data = calloc(len_data + 1, sizeof(unsigned char));
    unsigned char *buf = malloc(sizeof(char) * 8);
    unsigned char tmp = 0;
    int i = 0;
    int c = 0;
    actual = datai->first;
    while (actual != NULL)
    {

        if (i == 8)
        {
            --i;
            tmp = 0;
            for (int j = 0; j < 8; ++j)
            {
                tmp += (pow(2, j) * buf[i]);
                buf[i] = 0;
                --i;
            }
            output->data[c] = tmp;
            ++c;
            i = 0;
        }
        buf[i] = actual->key;
        ++i;
        actual = actual->next;
    }
    char align = 0;
    while (i != 8)
    {
        ++align;
        buf[i] = 0;
        ++i;
    }
    --i;
    tmp = 0;
    for (int j = 0; j < 8; ++j)
    {
        tmp += (pow(2, j) * buf[i]);
        --i;
    }
    output->data[c] = tmp;
    output->align = align;
    output->len = len_data;
    free(buf);
}

struct huff_out *compression(unsigned char *dataIN, int len_IN)
{
    //Debut Freqlist
    struct freqlist *freqList = buildFrequenceList(dataIN, len_IN);
    //Test exception
    if (freqList == NULL || freqList->freq->first == NULL ||
            freqList->freq->first == NULL)
    {
        errx(4, "FreqList is NULL");
    }
    //Fin Freqlist

    if (len_list(freqList->car) < 5)
    {
        return simple_compression(freqList, dataIN, len_IN);
    }
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
    unsigned char static_table[len_table];
    liste_to_string(table, static_table);
    //Fin table statique

    //Debut encodage donnees
    struct liste *encoding_data;
    encoding_data = encode_data(dataIN, static_table, len_table, len_IN);
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
    //Mise en forme de la chaine output
    //Huffman tree
    unsigned char *output = malloc(sizeof(unsigned char *) *
            (13 + char_data->len + char_tree->len));
    output[0] = 202;
    output[1] = char_tree->align;
    output[2] = char_tree->prof;
    for (int i = 1; i <= LEN_DATA; ++i)
    {
        output[3 + (LEN_DATA - i)] = (char_tree->len /
                (int)pow(10, (i - 1))) % 10;
    }
    int actuel_out = 3 + LEN_DATA;
    strcpyh(&(output[actuel_out]), char_tree->data, char_tree->len);
    actuel_out += char_tree->len;

    //Char data
    output[actuel_out++] = char_data->align;
    for (int i = 1; i <= LEN_DATA; ++i)
    {
        output[actuel_out + (LEN_DATA - i)] = (char_data->len /
                (int)pow(10, (i - 1))) % 10;
    }
    actuel_out += LEN_DATA;
    strcpyh(&(output[actuel_out]), char_data->data, char_data->len);
    actuel_out += char_data->len;

    struct huff_out *OUTPUTE = malloc(sizeof(struct huff_out));
    OUTPUTE->dataOUT = output;
    OUTPUTE->len = actuel_out;
    //Deallocation de toutes les struct
    bin_free(huffman);
    liste_free(table);
    liste_free(encoding_data);
    liste_free(encoding_tree);
    free(char_tree->data);
    free(char_tree);
    free(char_data->data);
    free(char_data);
    return OUTPUTE;
}

void insert_inplace(struct element *prec, struct element *next,
        struct liste *liste, char n)
{
    if (prec == NULL)
    {
        if (next == NULL)
        {
            liste->first = malloc(sizeof(struct element));
            liste->last = liste->first;
            liste->first->next = NULL;
            liste->first->prec = NULL;
            liste->first->key = n;
        }
        else
        {
            next->prec = malloc(sizeof(struct element));
            next->prec->prec = NULL;
            next->prec->next = next;
            next->prec->key = n;
            liste->first = next->prec;
        }
    } else if (next == NULL)
    {
        prec->next = malloc(sizeof(struct element));
        prec->next->prec = prec;
        prec->next->next = NULL;
        prec->next->key = n;
        liste->last = prec->next;
    }
    else
    {
        prec->next = malloc(sizeof(struct element));
        prec->next->prec = prec;
        prec->next->next = next;
        prec->next->key = n;
        next->prec = prec->next;
    }
}
void liste_bin_to_char(struct liste *liste, struct element *debut)
{
    unsigned char n = 0;
    for (int i = 7; i >= 0; --i)
    {
        n += ((unsigned char)debut->key * pow(2, i));
        debut = debut->next;
        del_in(debut->prec);
    }
    insert_inplace(debut, debut->next, liste, n);
}

unsigned char bin_to_char(unsigned char *data, int actual)
{
    unsigned char n = 0;
    for (int i = 7; i >= 0; --i)
    {
        n += (data[actual] * pow(2, i));
        ++actual;
    }
    return n;
}

int rebuild_tree(unsigned char *data, int actual, unsigned char key,
        struct bintree *huffman, int prof)
{
    int act_prof = 1;
    int tour = 0;
    struct bintree *huff_act = huffman;
    while (prof != act_prof)
    {
        if (huff_act->key != 0 && huff_act->key != 1)
        {
            printf("prof = %d, act_prof = %d\n", prof, act_prof);
            errx(EXIT_FAILURE, "Tentative d'insertion sur une feuille");
        }
        if (data[actual] == 0)
        {
            if (huff_act->left == NULL)
            {
                insert_left(huff_act, 0);
            }
            huff_act = huff_act->left;
        }
        if (data[actual] == 1)
        {
            if (huff_act->right == NULL)
            {
                insert_right(huff_act, 1);
            }
            huff_act = huff_act->right;
        }
        ++tour;
        ++act_prof;
        ++actual;
    }
    if ((data[actual] == 0 && huff_act->left != NULL) || (data[actual] == 1
                && huff_act->right != NULL))
    {
        printf("huff_act->right->key = %d huff_act->left->key = %d\n", 
                huff_act->right->key, huff_act->right->key);
        errx(4, "Houston on a un probleme!");

    }
    else
    {
        if (data[actual] == 0)
            insert_left(huff_act, key);
        else
            insert_right(huff_act, key);
    }
    return actual + 1;
}

unsigned char power(unsigned char n, unsigned char p)
{
    unsigned char result = 1;
    for (unsigned char i = 0; i < p; ++i)
    {
        result *= n;
    }
    return result;
}

void data_to_bin(unsigned char *dest, unsigned char *src, int len)
{
    int tmp;
    unsigned char tmp2;
    for (int i = 0; i < len; ++i)
    {
        tmp = 0;
        tmp2 = src[i];
        for (int j = 7; j >= 0; --j)
        {
            if (tmp2 < power(2, j))
                dest[i * 8 + tmp] = 0;
            else
                dest[i * 8 + tmp] = 1;
            tmp2 = tmp2 % power(2, j);
            ++tmp;
        }
    }
}

int length_tree(int prof)
{
    int length = 0;
    for (int i = 2; i <= prof; ++i)
    {
        length += (2 * i);
    }
    length += (2 * prof);
    return length;
}

struct bintree *decode_tree(unsigned char *data, int len, int prof, char align,
        int nb_char)
{
    int actuel_prof = 2;
    struct bintree *huffmantree = new_tree(0);
    unsigned char key;
    unsigned char *bindata = malloc(sizeof(unsigned char) * (len * 8));
    for (int i = 0; i < (len * 8); ++i)
        bindata[i] = 0;
    data_to_bin(bindata, data, len);
    int actuel = 0;
    while (actuel_prof < prof)
    {
        key = bin_to_char(bindata, actuel);
        actuel += 8;
        actuel = rebuild_tree(bindata, actuel, key,  huffmantree, actuel_prof);
        key = bin_to_char(bindata, actuel);
        actuel += 8;
        actuel = rebuild_tree(bindata, actuel, key, huffmantree, actuel_prof);
        ++actuel_prof;
    }

    if (len * 8 - align > length_tree(prof) + prof * 16)
    {
        for (int i = 0; i < 2; ++i)
        {
            key = bin_to_char(bindata, actuel);
            actuel += 8;
            actuel = rebuild_tree(bindata, actuel, key, huffmantree, prof);
        }
        for (int i = 0; i < 2; ++i)
        {
            key = bin_to_char(bindata, actuel);
            actuel += 8;
            actuel = rebuild_tree(bindata, actuel, key, huffmantree, prof + 1);
        }
        key = bin_to_char(bindata, actuel);
        actuel += 8;
        actuel = rebuild_tree(bindata, actuel, key, huffmantree, prof);
    }
    else
    {
        while (actuel < ((len * 8) - align - 8 - prof))
        {
            ++nb_char;
            key = bin_to_char(bindata, actuel);
            actuel += 8;
            actuel = rebuild_tree(bindata, actuel, key, huffmantree, prof);
        }

        key = bin_to_char(bindata, actuel);
        actuel += 8;
        unsigned char end_data[prof];
        for (int i = 0; i < prof; ++i)
        {
            end_data[i] = bindata[i + actuel];
        }
        ++actuel;
        actuel = rebuild_tree(end_data, 0, key, huffmantree, prof);
    }
    free(bindata);
    return huffmantree;
}

int decompressing_data(struct bintree *huffman, unsigned char *data,
        int len, struct liste *decompressed, char align)
{
    int actual = 0;
    unsigned char *bindata = malloc(sizeof(unsigned char) * (len * 8));
    for (int i = 0; i < len * 8; ++i)
        bindata[i] = 0;
    data_to_bin(bindata, data, len);
    int nb_decom = 0;
    struct bintree *actualbin = huffman;
    while (actual < len * 8 - align)
    {
        if (bindata[actual] == 0)
            actualbin = actualbin->left;
        if (bindata[actual] == 1)
            actualbin = actualbin->right;
        if (actualbin->left == NULL && actualbin->right == NULL)
        {
            insert(decompressed, actualbin->key);
            actualbin = huffman;
            ++nb_decom;
        }
        ++actual;
    }
    free(bindata);
    return nb_decom; 
}

struct huff_out *decompression(unsigned char *data, int len_data)
{
    int actual = 1;
    if (data[0] <= 127)
        return simple_decompression(data, len_data);

    struct encod_tree *huffman_cp = malloc(sizeof(struct encod_tree));
    huffman_cp->prof = 0;
    huffman_cp->len = 0;

    //Construction de tree align - par nature align < 8
    huffman_cp->align = data[actual];
    ++actual;

    //Construction de tree prof - par nature prof < 256
    huffman_cp->prof += (int)data[actual++];

    //Construction tree longueur - On part sur len < 10 000
    huffman_cp->len += ((int)data[actual++] * 10000);
    huffman_cp->len += ((int)data[actual++] * 1000);
    huffman_cp->len += ((int)data[actual++] * 100);
    huffman_cp->len += ((int)data[actual++] * 10);
    huffman_cp->len += (int)data[actual++];
    huffman_cp->data = malloc(sizeof(unsigned char *) * huffman_cp->len);
    strcpyh(huffman_cp->data, &(data[actual]), (huffman_cp->len));
    printf("tree len = %d\n", huffman_cp->len);
    actual += huffman_cp->len;

    struct encod_data *data_cp = malloc(sizeof(struct encod_data));
    data_cp->len = 0;

    //Construction de data align - par nature align < 8
    data_cp->align = (int)data[actual++];

    //Construction de tree len - On part sur len < 10 000
    data_cp->len += ((int)data[actual++] * 10000);
    data_cp->len += ((int)data[actual++] * 1000);
    data_cp->len += ((int)data[actual++] * 100);
    data_cp->len += ((int)data[actual++] * 10);
    data_cp->len += (int)data[actual++];

    if (actual + data_cp->len > len_data){
        //printf("Len_data = %d, actual = %d\n", len_data, (actual + data_cp->len));
        //errx(4, "Attention out of range");}
        --data_cp->len;}

    data_cp->data = malloc(sizeof(unsigned char *) * data_cp->len);
    for (int i = 0; i < (data_cp->len); i++){
        data_cp->data[i] = data[actual + i];}

    //Decodage et reconstruction de l'arbre
    struct bintree *huffmantree;
    int nb_char = 0;
    huffmantree = decode_tree(huffman_cp->data, huffman_cp->len,
            huffman_cp->prof, huffman_cp->align, nb_char);

    //Decompression des donnees
    struct liste *liste_out = new_liste();
    int len_out = decompressing_data(huffmantree, data_cp->data, data_cp->len,
            liste_out, data_cp->align);
    struct huff_out *retour = malloc(sizeof(struct huff_out));
    retour->dataOUT = malloc(sizeof(unsigned char) * (len_out + 1));
    retour->dataOUT[len_out] = '\0';
    liste_to_string(liste_out, retour->dataOUT);
    retour->len = len_out;

    //Desallocation des struct
    liste_free(liste_out);
    free(huffman_cp->data);
    free(huffman_cp);
    free(data_cp->data);
    free(data_cp);
    bin_free(huffmantree);
    return retour;
}
