#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <../struct.h>
#include <lz78.h>

void adddico(struct dico *table, unsigned char *accu, int len)
{
    
}

bool isindico(struct dico *table, unsigned char *accu, int len)
{
    
}

void create_dico(struct dico *table, unsigned char *input, int len)
{
    int point = 0;
    int taille = 1;
    while (point < len)
    {
        unsigned char *accu = malloc(sizeof((unsigned char) * taille));
        int actu = 0;
        bool isIN = 
        while (actu < taille || !is
        accu[actu++] = input[point];
        
    }
}

void principale(unsigned char *input, int len)
{
    struct dico *table = new_dico(len / 4);
    create_dico(table, intput, len);
}
