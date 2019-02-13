#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "bintree.h"
#include "liste.h"

struct freqlist {
    struct liste *freq;
    struct liste *car;
} freqlist;

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

int principale(char dataIN[])
{
    struct freqlist *freqList = buildFrequenceList(dataIN);
    return 0;
}
