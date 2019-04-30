#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <err.h>

#include <../struct.h>
#include <lz78.h>

bool check_adddico(struct dico *table, unsigned char *accu, int len)
{
    /* This function check if the accu string exist in the dico
     * If already exist, return False
     * Else
     * Add the new letter in the dico and return True
     */
    int point = -1; //Index lettre precedente
    int act = 0; //Pointeur tableau dico
    int act1 = 0; //Pointeur tableau accu
    while (act < dico->taux && act1 < len)
    {
        if (accu[act1] == table->letter[act] && point == dico->vector[act])
        {
            ++act1;
            point = act;
        }
        ++act;
    }
    if (act1 == len)
    {
        /* The string already exist in dico
         * The string is too short
         */
        return false;
    }
    if (dico->len == dico->taux) //If dico is full
        extension_dico(table);
    dico->letter[dico->taux] = accu[len - 1]; //Add new letter
    dico->vector[dico->taux] = point;
    dico->taux += 1;
    return true;
}

void create_dico(struct dico *table, unsigned char *input, int len)
{
    int point = 0; /* Pointer on input string */
    int taille = 1; /* Size of accu, to check in dico */
    while (point < len)
    {
        unsigned char *accu = malloc(sizeof((unsigned char) * taille));
        int actu = 0; /* Pointer on accu string */
        bool isIN = false; /* If add in the dico */
        while (actu < taille || !isIN)
        {
            accu[actu++] = input[point];
            isIN = check_adddico(table, accu, actu);
            ++point;
        }
        if (actu + 1 => taille) /* Raise the size of accu */
            ++taille;
    }
}

void compress_lz78(struct dico *table, unsigned char *input, int len)
{
    /* Arguments :
     * Dictionnary "table"
     * Input string to compress "input"
     * Size of Input string "len"
     * Output string "output"
     * Condition :
     * The dictionnary is already built
     * Return :
     * Size of compressed string
     */
    int point = 0; /* Pointer on input string */
    int taille = 1; /* Size of accu, to check in the dico */
    while (point < len)

}

void principale(unsigned char *input, int len)
{
    struct dico *table = new_dico(len / 4);
    create_dico(table, input, len);
}
