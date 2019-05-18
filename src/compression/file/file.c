#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

#include "../struct.h"
#include "../lz78/dico.h"
#include "../lz78/lz78.h"

void load_data_file(char *file_path, unsigned char *data_file, ssize_t len_data)
{
    len_data = findSize(file_path);
    int *DATA_FILE = open(file_path, O_RDONLY, 0666);
    if (DATA_FILE == -1)
        errx(EXIT_FAILURE, "Fail to open data file");
    data_file = malloc(sizeof(unsigned char) * len_data);
    int r = read(file_path, data_file, len_data);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to read data file");
    close(DATA_FILE);
    return;
}

void load_dico_file(struct dico *table, char *path_dico)
{
    /* Check if file exist */
    int len_path = strlen(path_dico);
    char *path_dico_e = malloc(sizeof(char) * (len_path + 5));
    path_dico_e = strncpy(path_dico_e, path_dico, len_path);
    path_dico_e = strcat(path_dico_e, ".dic", 4);
    if (access(path_dico_e, W_OK) == -1)
    {
        table = new_dico(0, NULL);
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

        int *DICO = open(path_dico_e, O_RDONLY, 0666);
        if (DICO == -1)
            errx(EXIT_FAILURE, "Fail to open dico file");
        size_t len_dico = findSize(path_dico_e);
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
        ssize_t taux = file_to_len(buffer);
        ssize_t len = 1;
        while (len < taux)
        {
            len *= 2;
        }
        table = new_dico(len, taux);

        //table->letter
        for (ssize_t i = 0; i < table->taux; ++i)
        {
            table->letter[i] = tmp_dico[i + 4];
        }

        //table->vector
        for (ssize_t i = 0; i < table->taux; ++i)
        {
            /*
             * Each size_t vector are store on 4B
             */
            for (ssize_t j = 0; j < 4; ++j)
            {
                buffer[j] = tmp_dico[(table->taux + 5) + i * 4 + j];
            }
            table->vector[i] = file_to_len(buffer);
        }
        free(buffer);
    }
}

void write_data_file(char *data, ssize_t len_data, char *path_file)
{
    int *DATA_FILE = open(path_file, O_WRONLY | O_TRUNC | O_CREATE, 0666);
    if (DATA_FILE == -1)
        errx(EXIT_FAILURE, "Fail to create temp data file");
    int r = write(DATA_FILE, data, len_data);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to write temp data file");
    close(DATA_FILE);
    free(data);
    return;
}

void write_dico_file(struct dico *table, char *path_dico)
{
    path_dico = strcat(path_dico, ".dic");
    int *DICO = open(path_dico, O_WRONLY | O_TRUNC |O_CREAT, 0666);
    if (DICO == -1)
        errx(EXIT_FAILURE, "Fail to open dico file");
    int r = 0;

    //Write len dico (0 -> 3 [4 B])
    r = write(DICO, len_to_file(table->taux), 4);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to write len dico in file");

    //Write letter list (4 -> table->taux + 4 [table->taux Bytes])
    r = write(DICO, (char *)(table->letter), table->taux);
    if (r == -1)
        errx(EXIT_FAILURE, "Fail to write letters in file");

    //Write vector of dico (4B per size_t number => (4 * taux)B)
    for (ssize_t i = 0; i < table->taux; ++i)
    {
        char *buf = len_to_file(table->vector[i]);
        r = write(DICO, buf, 4);
        if (r == -1)
            errx(EXIT_FAILURE, "Fail to write size_t vector in file");
        free(buf);
    }
    close(DICO);
}
