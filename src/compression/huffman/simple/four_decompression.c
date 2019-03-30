#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

#include "../../struct.h"
#include "../huffman.h"

#define LEN_DATA 4

void char_to_bin(unsigned char *bindata, int point, unsigned char letter)
{
    unsigned char tmp = letter;
    for (int i = 7; i >= 0; --i)
    {
        if (tmp < power(2, i))
        {
            bindata[point++] = 0;
        }
        else
        {
            bindata[point++] = 1;
        }
        tmp = tmp % power(2, i);
    }
}

struct huff_out *one_decompression(unsigned char *bindata, int len,
        unsigned char *chareq, char align)
{
    unsigned char *dataOUT = malloc(sizeof(unsigned char) * (len * 8 - align));
    for (int i = 0; i < len * 8 - align; ++i)
    {
        if (bindata[i] == 0)
            dataOUT[i] = chareq[0];
        else
            dataOUT[i] = chareq[1];
    }
    struct huff_out *retour = malloc(sizeof(struct huff_out));
    retour->len = len - align;
    retour->dataOUT = dataOUT;
    printf("%d\n", retour->dataOUT[0]);
    return retour;
}

struct huff_out *dual_decompression(unsigned char *bindata, int len,
        unsigned char *chareq, char align)
{
    unsigned char *dataOUT = malloc(sizeof(unsigned char)*((len - align) / 2));
    int j = 0;
    for (int i = 0; i < len - align; i += 2)
    {
        if (bindata[i] == 0)
        {
            if (bindata[i + 1] == 0)
                dataOUT[j] = chareq[0];
            else
                dataOUT[j] = chareq[1];
        }
        else
        {
            if (bindata[i + 1] == 0)
                dataOUT[j] = chareq[2];
            else
                dataOUT[j] = chareq[3];
        }
        ++j;
    }
    struct huff_out *retour = malloc(sizeof(struct huff_out));
    retour->len = len - align;
    retour->dataOUT = dataOUT;
    return retour;
}

struct huff_out *simple_decompression(unsigned char *dataIN, int len_IN)
{
    int longueur = dataIN[0];
    char align = dataIN[1];
    unsigned char chareq[longueur];
    for (int i = 0; i < longueur; ++i)
    {
        chareq[longueur - 1 -i] = dataIN[len_IN - 1 -i];
    }
    int act = 6;
    unsigned char bindata[(len_IN - longueur - 6) * 8];
    int actbin = 0;
    for (int i = 0; i < len_IN - longueur - 6; ++i)
    {
        char_to_bin(bindata, actbin, dataIN[act++]);
        actbin += 8;
    }
    if (longueur < 3)
    {
        struct huff_out *retour = one_decompression(bindata, len_IN-longueur -6, chareq, align);
        printf("test = %s\n", retour->dataOUT[0]);
        return retour;
    }
    else
    {
        return dual_decompression(bindata, len_IN-longueur-6, chareq, align);
    }
}
