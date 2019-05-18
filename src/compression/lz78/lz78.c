#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include <../struct.h>
#include <lz78.h>

/* For compress by dico:
 * Input :
 * char*
 * Len of char*
 * Output :
 * Create dico file
 * char*
 * len of char*
 */

/* In dico file:
 * Header:
 * Taux in 256 base on 4 Bytes
 * Letter (1 Byte per letter) on Taux Bytes
 * Vector (4 Bytes per size_t units) on 4 * Taux Bytes
 */ 

int check_adddico(struct dico *table, unsigned char *accu, int len)
{
    /*
     * This function check if the accu string exist in the dico
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
        /*
         * The string already exist in dico
         * The string is too short
         */
        return -2;
    }
    if (dico->len == dico->taux) //If dico is full
        extension_dico(table);
    dico->letter[dico->taux] = accu[len - 1]; //Add new letter
    dico->vector[dico->taux] = point;
    dico->taux += 1;
    return point;
}

int endcheck_dico(struct dico *table, unsigned char *accu, int len)
{
    /* This function return the index of accu string even
     * if it already exists
     */
    int point = -1;
    int act = 0;
    int act1 = 0;
    while (act < dico->taux && act1 < len)
    {
        if (accu[act1] == table->letter[act] && point == dico->vector[act])
        {
            ++act1;
            point = act;
        }
        ++act;
    }
    return point;
}

void create_dico(struct dico *table, unsigned char *input, int len,
        struct pylist *out)
{
    int point = 0; /* Pointer on input string */
    int taille = 1; /* Size of accu, to check in dico */
    int real = 0; /* To know the numbers letters remains at the end */
    while (point < len)
    {
        unsigned char *accu = malloc(sizeof(unsigned char) * taille);
        int actu = 0; /* Pointer on accu string */
        int isIN = -2; /* If add in the dico */
        while (actu < taille || isIN == -2)
        {
            accu[actu++] = input[point];
            isIN = check_adddico(table, accu, actu);
            ++point;
            if (isIN != -2)
                addpy(out, isIN);
        }
        real += actu;
        if (actu + 1 => taille) /* Raise the size of accu */
            ++taille;
        free(accu);
    }
    unsigned char *accu = malloc(sizeof(unsigned char) * (len - real));
    for (int i = real; i < len; ++i)
    {
        accu[i - real] = input[i];
    }
    addpy(out, endcheck_dico(table, accu, real));
}

void list_to_string(struct pylist *py, unsigned char *output)
{
    /* Converter from char list to pylist */

    output = malloc(sizeof(unsigned char) * py->len);
    struct pyele *actuel = py->begin; /* Pointer in py */
    int point = 0; /* Pointer in output */
    while (actuel != NULL)
    {
        output[point] = actuel->key;
        ++point;
        actuel = actuel->next;
    }
}

char *len_to_file(size_t len)
{
    /*
     * The orders of output numbers is inversed
     * Converter from base 10 to base 256 
     */
    char *flen = malloc(sizeof(char) * 4);
    flen[3] = len / (pow(256, 3));
    len -= flen[3] * pow(256, 3);
    flen[2] = len / (pow(256, 2));
    len -= flen[2] * pow(256, 2);
    flen[1] = len / 256;
    len -= flen * 256;
    flen[0] = len % 256;
    return flen;
}

size_t file_to_len(char *f_len)
{
    /*
     * The orders of input numbers is inversed
     * Converter from base 256 to base 10
     */
    size_t len = 0;
    size_t accu = 0;
    for (int i = 0; i < 4; ++i)
    {
        accu = f_len[i];
        if (accu < 0)
            len += ((128 - accu) + 128) * (pow(255,i));
        else
            len += accu;
    }
    return len;
}

unsigned char *dico_name(unsigned char *file)
{
    unsigned char *name = malloc(sizeof(unsigned char) * 5);
    for (int i = 0; i < 5; ++i)
    {
        name[i] = file[i + 4];
    }
    return name;
}

void compress_lz78(unsigned char *output, unsigned char *input, int len
        char *dico_path)
{
    /* Arguments :
     * Dictionnary path "dico_path"
     * Input string to compress "input"
     * Size of Input string "len"
     * Output string "output"
     * Return :
     * Size of compressed string
     */

    int point = 0; /* Pointer on input string */
    int taille = 1; /* Size of accu, to check in the dico */
    struct pylist *out = new_py();
    create_dico(table, input, len, out);
    list_to_string(out, output); /* Converter dynamic to static */
    int len_out = py->len; /* Len to output string */
    freepy(out); /* Free pylist */


}

void decompress_lz78(char *dico_path, char *data_path)
{
    
}

void principale(unsigned char *input, int len)
{
    struct dico *table = new_dico(len / 4);
    create_dico(table, input, len);
}
