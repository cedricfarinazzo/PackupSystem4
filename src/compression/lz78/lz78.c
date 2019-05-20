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

/*
 * In dico file:
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
    return dico->taux - 1;
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

void create_dico(struct dico *table, unsigned char *input, ssize_t len,
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
        while (actu < taille && isIN == -2)
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
    /* Converter from pylist to uchar list */

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

size_t pylist_to_string(struct pylist *py, unsigned char *output)
{
    /* Convert pylist (size_t*) to uchar* 
     * Each size_t will encoded on 4B 
     */
    output = malloc(sizeof(unsigned char) * py->len);
    struct pyele *actuel = py->begin; /* Pointer for py */
    int point = 0; /* Pointeur for output */
    unsigned char buf = malloc(sizeof(unsigned char) * 4);
    while (actuel != NULL)
    {
        buf = (unsigned char)len_to_file(actuel->key);
        actuel = actuel->next;
        for (int i = 0; i < 4; ++i)
        {
            output[point * 4 + i] = buf[i];
        }
        ++point;
    }
    free(buf);
    return py->len;
}

size_t string_to_pylist(struct pylist *py, unsigned char *input, size_t len)
{
    /* Converter uchar* to pylist (size_t*)
     * Each size_t produce from 4 uchar
     */
    size_t point = 0;
    unsigned char *buf = malloc(sizeof(unsigned char) * 4);
    while (point < len)
    {
        for (int i = 0; i < 4; ++i)
        {
            buf[i] = input[point++];
        }
        addpy(py, file_to_len(buf));
    }
    free(buf);
    return py->len;
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

size_t search_in_dico(struct dico *table, size_t index,
        unsigned char *output)
{
    struct pylist *tmp = new_py();
    size_t act_index = index;
    while (act_index > 0)
    {
        addpy(tmp, table->letter[index]);
        act_index = table->vector[act_index];
    }
    unsigned char *output = malloc(sizeof(unsigned char) * tmp->len);
    struct pyele *actu = tmp->begin;
    for (size_t i = 0; i < tmp->len; ++i)
    {
        output[tmp->len - i- 1] = actu->key;
        actu = actu->next;
    }
    size_t len = tmp->len
    freepy(tmp);
    return len;
}

size_t dico_to_data(struct dico *table, unsigned char *out,
        struct pylist *data)
{
    size_t point = 0; //Pointeur on data
    size_t tmp_len;
    struct pyele *actu = data->begin;
    struct pylist *output = new_py();
    for (;point < data->len; ++point)
    {
        unsigned char *tmp;
        tmp_len = search_in_dico(table, actu->key, tmp);
        for (size_t i = 0; i < tmp_len; ++i)
        {
            addpy(output, tmp[i]);
        }
        free(tmp);
        actu = actu->next;
    }
    out = malloc(sizeof(unsigned char) * output->len);
    actu = output->begin;
    for (size_t i = 0; i < output->len; ++i)
    {
        out[i] = actu->key;
        actu = actu->next;
    }
    size_t len = output->len;
    freepy(output);
    return len;
}

void compress_lz78(char *data_path, char *dico_path, char *tmp_path)
{
    size_t len_data;
    unsigned char *data;
    load_data_file(data_path, data, len_data);
    struct dico *table;
    load_dico_file(table, dico_path);
    struct pylist *data_c_l = new_py();
    create_dico(table, data, len_data, data_c_l);
    free(data);
    write_dico_file(table, dico_path);
    free_dico(table);
    unsigned char *data_compress;
    len_data = pylist_to_string(data_c_l, data_compress);
    freepy(data_c_l);
    write_data_file((char *)(data_compress), len_data, tmp_path);
    free(data_compress);
    return;
}

void decompress_lz78(char *data_path, char *dico_path, char *out_path)
{
    unsigned char *tdata;
    size_t lendata;
    load_data_file(data_path, tdata, lendata);
    struct dico *table;
    load_dico_file(table, dico_path);
    struct pylist *data= new_py();
    data->len = string_to_pylist(data, tdata, lendata);
    free(tdata);
    unsigned char *data_decomp;
    size_t lendatad = dico_to_data(table, data_decomp, data);
    write_data_file((char*)(data_decomp), lendatad, data_out);
    free(data);
    free_dico(table);
    return;
}
