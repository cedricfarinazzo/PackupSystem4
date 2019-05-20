#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <math.h>

#include "../struct.h"
#include "../lz78/dico.h"

size_t file_to_len(char *f_len)
{
    /*
     * The orders of input numbers is inversed
     * Converter from base 256 to base 10
     */
    size_t len = 0;
    char accu = -1;
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
    len -= flen[1] * 256;
    flen[0] = len % 256;
    return flen;
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
    unsigned char *buf;
    while (actuel != NULL)
    {
        buf = (unsigned char *)len_to_file(actuel->key);
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
        addpy(py, file_to_len((char *)buf));
    }
    free(buf);
    return py->len;
}


