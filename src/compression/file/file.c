#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>

#include "../struct.h"
#include "../lz78/dico.h"
#include "../lz78/lz78.h"
#include "../struct/converter.h"


void load_data_file(char *file_path, unsigned char **data_file, size_t *len_data)
{
    struct stat st;
    if (stat(file_path, &st) == 0)
        *len_data = st.st_size;
    else
        errx(EXIT_FAILURE, "File data not found");
    int DATA_FILE = open(file_path, O_RDONLY);
    if (DATA_FILE == -1)
        errx(EXIT_FAILURE, "Fail to open data file");
    *data_file = malloc(sizeof(unsigned char) * (*len_data));
    int r = read(DATA_FILE, *data_file, *len_data);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to read data file");
    close(DATA_FILE);
    return;
}

void load_dico_file(struct dico **table, char *path_dico)
{
    /* Check if file exist */
    if (access(path_dico, W_OK) == -1)
    {
        *table = new_dico(1, 0);
    }
    else
    {
        /* Structure of dico file:
         * 0-3 B -> nb of case in dico (here table->taux)
         * 4-(taux + 4) -> list of letter in table->letter
         * (taux + 5)-end -> list of size_t element in table->vector
         *
         * Each size_t is coded on 4B in file dico
         */

        int DICO = open(path_dico, O_RDONLY);
        if (DICO == -1)
            errx(EXIT_FAILURE, "Fail to open dico file");
        struct stat st;
        stat(path_dico, &st);
        size_t len_dico = st.st_size;
        char *tmp_dico = malloc(sizeof(char) * len_dico);
        int r = read(DICO, tmp_dico, len_dico);
        if (r == -1)
            errx(EXIT_FAILURE, "Fail to read dico file");
        close(DICO);
        char *buffer = malloc(sizeof(char) * 4);

        //table->taux + table->len
        for (int i = 0; i < 4; ++i)
        {
            buffer[i] = tmp_dico[i];
        }
        size_t taux = file_to_len(buffer);
        size_t len = 1;
        while (len < taux)
        {
            len *= 2;
        }
        //printf("table->taux = %ld\n", taux);
        *table = new_dico(len, taux);

        //table->letter
        for (size_t i = 0; i < (*table)->taux; ++i)
        {
            (*table)->letter[i] = tmp_dico[i + 4];
        }

        //table->vector
        for (size_t i = 0; i < (*table)->taux; ++i)
        {
            /*
             * Each size_t vector are store on 4B
             */
            for (ssize_t j = 0; j < 4; ++j)
            {
                buffer[j] = tmp_dico[((*table)->taux + 4) + i * 4 + j];
            }
            ssize_t vect = file_to_lens(buffer);
            //printf("table->vector[%ld] = %ld | table->letter[%ld] = %d\n",
            //        i, vect, i, (*table)->letter[i]);
            (*table)->vector[i] = vect;
        }
        free(tmp_dico);
        free(buffer);
    }
}

void write_data_file(char *data, ssize_t len_data, char *path_file)
{
    int DATA_FILE = open(path_file, O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (DATA_FILE == -1)
        errx(EXIT_FAILURE, "Fail to create temp data file");
    //printf("len_data = %ld\n", len_data);
    for (ssize_t i = 0; i < len_data; ++i)
    {
        //printf("%c\n",data[i]);
    }//printf("\n");
    int r = write(DATA_FILE, data, (size_t)len_data);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to write temp data file");
    close(DATA_FILE);
    free(data);
    return;
}

void write_dico_file(struct dico *table, char *path_dico)
{
    int DICO = open(path_dico, O_WRONLY | O_TRUNC |O_CREAT, 0666);
    if (DICO == -1)
        errx(EXIT_FAILURE, "Fail to open dico file");
    int r = 0;

    //Write len dico (0 -> 3 [4 B])
    char *nom = len_to_file(table->taux);
    r = write(DICO, nom, 4);
    free(nom);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to write len dico in file");

    //Write letter list (4 -> table->taux + 4 [table->taux Bytes])
    r = write(DICO, (char *)(table->letter), table->taux);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to write letters in file");
    //printf("dico->taux = %ld\n", table->taux);
    //Write vector of dico (4B per size_t number => (4 * taux)B)
    for (size_t i = 0; i < table->taux; ++i)
    {
        //printf("dico->vector[%ld] = %ld | dico->letter[%ld] = %d\n", i, table->vector[i],
      //          i, table->letter[i]);
        char *buf = len_to_file(table->vector[i]);
        r = write(DICO, buf, 4);
        if (r == -1)
            errx(EXIT_FAILURE, "Fail to write size_t vector in file");
        free(buf);
    }
    close(DICO);
}
