#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include "../struct.h"
#include "../struct/converter.h"
#include "../file/file.h"
#include "../liste/pylist.h"
#include "dico.h"

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

size_t check_adddico(struct dico *table, unsigned char *accu, int len)
{
    /*
     * This function check if the accu string exist in the dico
     * If already exist, return False
     * Else
     * Add the new letter in the dico and return True
     */
    ssize_t point = -1; //Index lettre precedente
    size_t act = 0; //Pointeur tableau dico
    int act1 = 0; //Pointeur tableau accu
    while (act < table->taux && act1 < len)
    {
        //printf("accu[0] = %c\n", accu[0]);
        if (accu[act1] == table->letter[act] && point == table->vector[act])
        {
            //printf("accu[%d] = %c | table->letter[%ld] = %c\n",
            //        act1, accu[act1], act, table->letter[act]);
            //printf("point = %ld | table->vector[%ld] = %ld\n",
            //        point, act, table->vector[act]);
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
    if (table->len == table->taux) //If dico is full
        extension_dico(table);
    table->letter[table->taux] = accu[len - 1]; //Add new letter
    table->vector[table->taux] = point;
    table->taux += 1;
    return table->taux - 1;
}

int endcheck_dico(struct dico *table, unsigned char *accu, int len)
{
    /* This function return the index of accu string even
     * if it already exists
     */
    ssize_t point = -1;
    size_t act = 0;
    int act1 = 0;
    while (act < table->taux && act1 < len)
    {
        if (accu[act1] == table->letter[act] && point == table->vector[act])
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
            //printf("input[%d] = %c\n", point, input[point]);
            accu[actu++] = input[point];
            isIN = check_adddico(table, accu, actu);
            //printf("isIN = %d\n", isIN);
            if (isIN != -2)
            {
                //printf("addpy(%d)\n", isIN);
                addpy(out, isIN);
            }
            point++;
        }
        real += actu;
        if (actu + 1 >= taille) /* Raise the size of accu */
            ++taille;
        free(accu);
    }
    unsigned char *accu = malloc(sizeof(unsigned char) * (len - real));
    for (int i = real; i < len; ++i)
    {
        accu[i - real] = input[i];
    }
    int rt = endcheck_dico(table, accu, real);
    //printf("addpy(%d)\n", rt);
    if (rt >= 0)
        addpy(out, rt);
    free(accu);
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
        unsigned char **output)
{
    struct pylist *tmp = new_py();
    ssize_t act_index = index;
    while (act_index >= 0)
    {
        //printf("table->vector[%ld] = %ld\n", act_index, table->vector[act_index]);
        addpy(tmp, table->letter[act_index]);
        act_index = table->vector[act_index];
        //printf("table->letter[%ld] = %d\n", index, table->letter[index]);
    }
    *output = malloc(sizeof(unsigned char) * tmp->len);
    struct pyele *actu = tmp->begin;
    for (size_t i = 0; i < tmp->len; ++i)
    {
        (*output)[tmp->len - i- 1] = actu->key;
        actu = actu->next;
    }
    size_t len = tmp->len;
    freepy(tmp);
    return len;
}

size_t dico_to_data(struct dico *table, unsigned char **out,
        struct pylist *data)
{
    size_t point = 0; //Pointeur on data
    size_t tmp_len;
    struct pyele *actu = data->begin;
    struct pylist *output = new_py();
    for (;point < data->len; ++point)
    {
        unsigned char *tmp = NULL;
        tmp_len = search_in_dico(table, actu->key, &tmp);
        for (size_t i = 0; i < tmp_len; ++i)
        {
            //printf("[%c]", tmp[i]);
            addpy(output, tmp[i]);
        }
        free(tmp);
        actu = actu->next;
    }
    //printf("\n");
    *out = malloc(sizeof(unsigned char) * output->len);
    actu = output->begin;
    for (size_t i = 0; i < output->len; ++i)
    {
        (*out)[i] = actu->key;
        actu = actu->next;
    }
    size_t len = output->len;
    freepy(output);
    return len;
}

void compress_lz78(char *data_path, char *dico_path, char *tmp_path)
{
    size_t len_data = 0;
    unsigned char *data = NULL;
    load_data_file(data_path, &data, &len_data);
    //printf("Len_input = %ld\n", len_data);
    struct dico *table = NULL;
    load_dico_file(&table, dico_path);
    struct pylist *data_c_l = new_py();
    create_dico(table, data, len_data, data_c_l);
    free(data);
    write_dico_file(table, dico_path);
    free_dico(table);
    unsigned char *data_compress = NULL;
    //struct pyele *test = data_c_l->begin;
    //size_t w = 0;
    //while (test != NULL)
    //{
    //    printf("datacmp[%ld] = %ld\n", w, test->key);
    //    test = test->next;
    //    ++w;
    //}
    len_data = pylist_to_string(data_c_l, &data_compress);
    //printf("Len_data_compr = %ld\n", len_data);
    freepy(data_c_l);
    write_data_file((char *)(data_compress), len_data, tmp_path);
    return;
}

void decompress_lz78(char *data_path, char *dico_path, char *out_path)
{
    unsigned char *tdata = NULL;
    size_t lendata = 0;
    load_data_file(data_path, &tdata, &lendata);
    //printf("data_cmp = [");
    //for (size_t o = 0; o < lendata; ++o)
    //{
        //printf("%d,", tdata[o]);
    //}//printf("\n");
    //printf("Len-data-compress = %ld\n", lendata);
    struct dico *table = NULL;
    load_dico_file(&table, dico_path);
    struct pylist *data = new_py();
    data->len = string_to_pylist(data, tdata, lendata);
    free(tdata);
    unsigned char *data_decomp = NULL;
    size_t lendatad = dico_to_data(table, &data_decomp, data);
    write_data_file((char*)(data_decomp), lendatad, out_path);
    freepy(data);
    free_dico(table);
    return;
}
