#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include <../struct.h>
#include <lz78.h>

/* In dico file:
 * Header:
 * Taux in 256 base on 4 Bytes
 * Letter (1 Byte per letter) on Taux Bytes
 */ 

int check_adddico(struct dico *table, unsigned char *accu, int len)
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
    /* Converter from char list to pyslit */

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

//Path with the name but without the extension
void dico_to_file(struct dico *table, char *path)
{
    strcat(path, ".dic");
    int *DICO = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (DICO == -1)
        errx(EXIT_FAILURE, "Fail to create dico file");
    int r = 0;
    //Write len dico (0 -> 3 [4 B])
    r = write(DICO, len_to_file(table->taux), 4);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to write dico file");
    //Write letter list [table->taux Bytes]
    r = write(DICO, (char *)(table->letter), taux);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to write dico file");
    //Write vector of dico (4B per size_t unit => (4 * taux)B)
    for (size_t i = 0; i < table->taux; ++i)
    {
        r = write(DICO, len_to_file(table->vector[i]), 4);
        if (r == -1)
            errx(EXIT_FAILURE, "Fail to write dico file");
    }
}

/*
 * Path => from the root
 * index => Name for the dico file without the extension
 * table => dico to convert
 */
void file_to_dico(struct dico *table, char *index, char *path)
{
    /* Check file exist */
    char path_dico[5 + strlen(path) + 4];
    strcat(path_dico, path);
    strcat(path_dico, index);
    strcat(path_dico, ".dic");
    if (access(path_dico, W_OK) == -1)
    {
        table = new_dico();
    }
    else
    {
        int *DICO = open(path_dico, O_RDONLY, 0666);
        size_t len_dico = findSize(path_dico); /* Taille totale du fichier */
        char *tmp_dico = malloc(sizeof(char) * len_dico + 1);
        ssize_t state = read(DICO, tmp_dico, len_dico);
        if (state != 0)
            errx(EXIT_FAILURE, "Fail to read dico file");
        char *buf = malloc(sizeof(char) * 4);
        for (int i = 0; i < 4; ++i)
            buf[i] = tmp_dico[i];
        size_t taux = file_to_len(buf);
        size_t len = 1;
        while (len < taux)
            len *= 2;
        table = new_dico(len);
        table->taux = taux;
        //+ 4 -> Bytes for len
        //* 5 -> letter * taux + pointeur * taux * 4
        if (len_dico != table->taux * 5 + 4)
            errx(EXIT_FAILURE, "Erreur dans la longueur du fichier .dic");
        for (size_t i = 0; i < table->taux; ++i)
        {
            table->letter[i] = tmp_dico[i + 4];
        }
        for (size_t i = 0; i < table->taux; ++i)
        {
            /*
             * Les nombres qui pointent sont des entiers stockes
             * sur 4 octets pour eviter un overflow
             */
            for (int x = 0; x < 4; ++x)
            {
                buf[x] = tmp_dico[i * 4 + x];
            }
            table->vector[i] = file_to_len(buf);
        }
        free(buf);
    }
}

void compress_lz78(unsigned char *index, unsigned char *input, int len)
{
    /* Arguments :
     * Dictionnary "table"
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
    unsigned char *output;
    list_to_string(out, output); /* Converter dynamic to static */
    int len_out = py->len; /* Len to output string */
    freepy(out); /* Free pylist */

}

void principale(unsigned char *input, int len)
{
    struct dico *table = new_dico(len / 4);
    create_dico(table, input, len);
}
