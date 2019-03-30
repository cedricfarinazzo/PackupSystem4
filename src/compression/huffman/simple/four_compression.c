#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../../liste/liste.h"
#include "../huffman.h"

#define LEN_DATA 5
struct huff_out *one_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN, int longueur)
{
    char align = 8 - (len_IN % 8);
    unsigned char *dataOUT;
    if (align == 0)
    {dataOUT = malloc(sizeof(unsigned char)*((len_IN/8) + 8));}
    else
    {dataOUT = malloc(sizeof(unsigned char) *((len_IN / 8) + 9));}
    dataOUT[0] = 1;
    printf("align = dataOUT[1] = %d\n", align);
    dataOUT[1] = align;
    for (int i = 2; i < LEN_DATA; ++i)
    {
        dataOUT[1 + (LEN_DATA - i)] = (len_IN /
                (int)pow(10, (i - 1))) % 10;
    }
    int act = LEN_DATA;
    if (align == 0) {dataOUT[act++] = len_IN % 10;}
    if (align != 0) {dataOUT[act++] = (len_IN % 10) + 1;}
    for (int i = 0; i < len_IN / 8; ++i)
        dataOUT[act + i] = 0;
    if (align != 0) {dataOUT[act + len_IN / 8] = 0;}
    act += len_IN / 8;
    dataOUT[++act] = freqList->car->first->key;
    free_freqlist(freqList);
    struct huff_out *retour = malloc(sizeof(struct huff_out));
    if (align == 0) {retour->len = (len_IN / 8) + 7;}
    else {retour->len = (len_IN / 8) + 8;}
    retour->dataOUT = dataOUT;
    return retour;
}

struct huff_out *two_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN, int longueur)
{
    //Passage en full static
    unsigned char bineq[2];
    bineq[0] = 0;
    bineq[1] = 1;
    unsigned char chareq[2];
    chareq[0] = freqList->car->first->key;
    chareq[1] = freqList->car->last->key;
    free_freqlist(freqList);
    //Construction de la chaine de sortie
    char align = 8 - (len_IN % 8);
    unsigned char *dataOUT;
    if (align == 0) {dataOUT = malloc(sizeof(unsigned char)*(len_IN/4)+8);}
    else {dataOUT = malloc(sizeof(unsigned char) * (len_IN/4) + 9);}
    //Nombre de caractere different
    dataOUT[0] = 2;
    //Align
    dataOUT[1] = align;
    //Longueur sur 5 octets
    for (int i = 2; i < LEN_DATA; ++i)
    {
        dataOUT[1 + (LEN_DATA - i)] = (len_IN / (int)pow(10, (i - 1))) % 10;
    }
    int act = LEN_DATA;
    if (align == 0){dataOUT[act++] = len_IN % 10;}
    if (align != 0){dataOUT[act++] = (len_IN % 10) + 1;}
    //Chaine compressee
    unsigned char bindata[len_IN + align];
    printf("Bindata = ");
    for (int i = 0; i < len_IN; ++i)
    {
        if (dataIN[i] == chareq[0])
        {
            //printf("dataIN[%d] = %d ; chareq[0] = %d\n", i, dataIN[i], chareq[0]);
            bindata[i] = 0;
            printf("bindata[%d] = 0", i);
        }
        else
        {
            //printf("dataIN[%d] = %d ; chareq[1] = %d\n", i, dataIN[i], chareq[1]);
            bindata[i] = 1;
            printf("bindata[%d] = 1", i);
        }
    }
    for (int i = len_IN; i < align + len_IN; ++i)
    {
        bindata[i] = 0;
    }
    for (int i = 0; i < len_IN + align; ++i)
        printf("bindata[%d] = %d\n", i, bindata[i]);
    unsigned char buf[8];
    for (int i = 0; i < len_IN + align; ++i)
    {
        if (i % 8 == 0 && i != 0)
        {
            dataOUT[act] = 0;
            for (int j = 0; j < 8; ++j)
            {
                dataOUT[act] += ((unsigned char)(pow(2, j)) * buf[7 - j]);
                buf[7 - j] = 0;
            }
            ++act;
            printf("act = %d\n", act);
        }
        buf[i % 8] = bindata[i];
    }
    if ((len_IN + align) % 8 == 0)
    {
        dataOUT[act] = 0;
        for (int j = 0; j < 8; ++j)
        {
            dataOUT[act] += ((unsigned char)(pow(2, j)) * buf[7 - j]);
            buf[7 - j] = 0;
        }
        ++act;
    }
    //Caracteres
    printf("dataOUT[%d] = %d\n", act, chareq[0]);
    dataOUT[act++] = chareq[0];
    printf("dataOUT[%d] = %d\n", act, chareq[1]);
    dataOUT[act] = chareq[1];
    printf("dataOUT = ");
    for (int i = 0; i < (len_IN / 4) + 8; ++i)
    {
        printf("%d ", dataOUT[i]);
    }
    printf("\n");
    struct huff_out *retour = malloc(sizeof(struct huff_out));
    if (align == 0) {retour->len = (len_IN / 4) + 8;}
    else {retour->len = (len_IN / 4) + 9;}
    retour->dataOUT = dataOUT;
    return retour;
}

struct huff_out *triple_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN, int longueur)
{
    unsigned char chareq[4];
    chareq[0] = freqList->car->first->key;
    chareq[1] = freqList->car->first->next->key;
    chareq[2] = freqList->car->first->next->next->key;
    if (longueur == 4)
        chareq[3] = freqList->car->last->key;
    char align = 8 - (len_IN % 8);
    unsigned char *dataOUT;
    if (align == 0) {dataOUT = malloc(sizeof(unsigned char)*(len_IN / 2) + 7);}
    else {dataOUT = malloc(sizeof(unsigned char)*(len_IN / 2) + 8);}
    dataOUT[0] = longueur;
    dataOUT[1] = align;
    for (int i = 2; i < LEN_DATA + 5; ++i)
    {
        dataOUT[1 + (LEN_DATA - i)] = (len_IN /
                (int)pow(10, (i - 1))) % 10;
    }
    int actual = LEN_DATA + 5;

}


struct huff_out *simple_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN)
{
    int longueur = len_list(freqList->car);
    printf("longueur = %d\n", longueur);
    if (longueur == 1)
        return one_compression(freqList, dataIN, len_IN, longueur);
    if (longueur == 2)
        return two_compression(freqList, dataIN, len_IN, longueur);
    errx(EXIT_FAILURE, "Fonction en cours de realisation");
}
