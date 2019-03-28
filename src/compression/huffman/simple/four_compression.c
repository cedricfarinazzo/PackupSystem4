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
    {dataOUT = malloc(sizeof(unsigned char)*((len_IN/8) + 7));}
    else
    {dataOUT = malloc(sizeof(unsigned char) *((len_IN / 8) + 8));}
    dataOUT[0] = 1;
    for (int i = 1; i < LEN_DATA; ++i)
    {
        dataOUT[1 + (LEN_DATA - i)] = (len_IN /
                (int)pow(10, (i - 1))) % 10;
    }
    int act = LEN_DATA;
    if (align == 0) {dataOUT[act++] = len_IN % 10;}
    if (align != 0) {dataOUT[act++] = (len_IN % 10) + 1;}
    for (int i = 0; i < len_IN / 8; ++i)
        dataOUT[i] = 0;
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
    if (align == 0) {dataOUT = malloc(sizeof(unsigned char)*(len_IN/4)+7);}
    else {dataOUT = malloc(sizeof(unsigned char) * (len_IN/4) + 8);}
    //Nombre de caractere
    dataOUT[0] = 2;
    //Align
    dataOUT[1] = align;
    //Chaine compressee
    unsigned char bindata[len_IN + align];
    for (int i = 0; i < len_IN; ++i)
    {
        if (dataIN[i] == chareq[0])
            bindata[i] = 0;
        else
            bindata[i] = 1;
    }
    for (int i = 0; i < align; ++i)
        bindata[len_IN + i] = 0;
    int act = 2;
    //Bin to char
    unsigned char buf[8];
    for (int i = 0; i < len_IN + align; ++i)
    {
        if (i % 8 == 0 && i != 0)
        {
            for (int j = 0; j < 8; ++j)
            {
                dataOUT[act] = 0;
                dataOUT[act] += ((unsigned char)(pow(2, j)) * buf[7 - j]);
                buf[7 - j] = 0;
            }
            ++act;
        }
        buf[i % 8] = bindata[i];
    }
    //Caracteres
    dataOUT[act++] = chareq[0];
    dataOUT[act++] = chareq[1];
    struct huff_out *retour = malloc(sizeof(struct huff_out));
    if (align == 0) {retour->len = (len_IN / 4) + 7;}
    else {retour->len = (len_IN / 4) + 8;}
    retour->dataOUT = dataOUT;
    return retour;
}

struct huff_out *simple_compression(struct freqlist *freqList,
        unsigned char *dataIN, int len_IN)
{
    int longueur = len_list(freqList->car);
    if (longueur == 1)
        return one_compression(freqList, dataIN, len_IN, longueur);
    if (longueur == 2)
        return one_compression(freqList, dataIN, len_IN, longueur);
    errx(EXIT_FAILURE, "Fonction en cours de realisation");
}
