#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../../liste/liste.h"
#include "../huffman.h"

#define LEN_DATA 5 //Nombre d'octets pour l'information de la longueur dans la
//chaine compressee
struct huff_out *one_compression(struct freqlist *freqList, int len_IN)
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
        unsigned char *dataIN, int len_IN)
{
    //Passage en full static
    unsigned char chareq[2];
    chareq[0] = freqList->car->first->key;
    chareq[1] = freqList->car->last->key;
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
    for (int i = 0; i < len_IN; ++i)
    {
        if (dataIN[i] == chareq[0])
        {
            bindata[i] = 0;
        }
        else
        {
            bindata[i] = 1;
        }
    }
    for (int i = len_IN; i < align + len_IN; ++i)
    {
        bindata[i] = 0;
    }
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
    dataOUT[act++] = chareq[0];
    dataOUT[act] = chareq[1];
    struct huff_out *retour = malloc(sizeof(struct huff_out));
    if (align == 0) {retour->len = (len_IN / 4) + 8;}
    else {retour->len = (len_IN / 4) + 9;}
    retour->dataOUT = dataOUT;
    free_freqlist(freqList);
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
    char align = (4 - (len_IN % 4)) % 4;
    unsigned char *dataOUT;
    if (align == 0) {dataOUT = malloc(sizeof(unsigned char)*(len_IN / 2) + 9);}
    else {dataOUT = malloc(sizeof(unsigned char)*(len_IN / 2) + 10);}
    dataOUT[0] = longueur;
    dataOUT[1] = align;
    int compteur = LEN_DATA - 2;
    for (int i = 2; i < LEN_DATA + 1; ++i)
    {
        dataOUT[i] = (len_IN / (int)pow(10, compteur)) % 10;
        --compteur;
    }
    int act = LEN_DATA + 1;
    unsigned char bindata[len_IN * 2 + align];
    for (int i = 0; i < len_IN; ++i)
    {
        if (dataIN[i] != chareq[2] ||(longueur == 4 && dataIN[i] != chareq[3]))
        {
            bindata[2 * i] = 0;
            if (dataIN[i] == chareq[0])
                bindata[2 * i + 1] = 0;
            else
                bindata[2 * i + 1] = 1;
        }
        else 
        {
            bindata[2 * i] = 1;
            if (dataIN[i] == chareq[2])
                bindata[2 * i + 1] = 0;
            else
                bindata[2 * i + 1] = 1;
        }
    }
    for (int i = 2 * len_IN; i < 2 * len_IN + align; ++i)
    {
        bindata[i] = 0;
    }
    unsigned char buf[8];
    for (int i = 0; i < 2 * len_IN + align; ++i)
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
        }
        buf[i % 8] = bindata[i];
    }
    if ((len_IN + align) % 4 == 0)
    {
        dataOUT[act] = 0;
        for (int j = 0; j < 8; ++j)
        {
            dataOUT[act] += ((unsigned char)(pow(2, j)) * buf[7 - j]);
        }
        ++act;
    }
    //Caracteres
    dataOUT[act++] = chareq[0];
    dataOUT[act++] = chareq[1];
    dataOUT[act] = chareq[2];
    struct huff_out *retour = malloc(sizeof(struct huff_out));
    if (align == 0) {retour->len = (len_IN / 4) + 9;}
    else {retour->len = (len_IN / 4) + 10;}
    retour->dataOUT = dataOUT;
    free_freqlist(freqList);
    return retour;
}

struct huff_out *simple_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN)
{
    int longueur = len_list(freqList->car);
    printf("longueur = %d\n", longueur);
    if (longueur == 1)
        return one_compression(freqList, len_IN);
    if (longueur == 2)
        return two_compression(freqList, dataIN, len_IN);
    else
        return triple_compression(freqList, dataIN, len_IN, longueur);
}
