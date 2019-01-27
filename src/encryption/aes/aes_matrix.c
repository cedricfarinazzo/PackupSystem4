#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aes_matrix.h"


struct AES_matrix *AES_matrix_init()
{
    struct AES_matrix *m = malloc(sizeof(struct AES_matrix));
    m->rowsLenght = AES_MATRIX_DEFAULT_ROWSLENGHT;
    m->colsLenght = AES_MATRIX_DEFAULT_COLSLENGHT;
    m->data = malloc(sizeof(int*) * m->rowsLenght);
    for (size_t i = 0; i < m->rowsLenght; ++i)
        m->data[i] = calloc(m->colsLenght, sizeof(int));
    return m;
}

void AES_matrix_free(struct AES_matrix *m)
{
    for (size_t i = 0; i < m->rowsLenght; ++i)
        free(m->data[i]);
    free(m->data);
    free(m);
}

size_t AES_matrix_getRowsLenght(struct AES_matrix *m)
{
    return m->rowsLenght;
}

size_t AES_matrix_getColsLenght(struct AES_matrix *m)
{
    return m->colsLenght;
}

void AES_matrix_getShape(struct AES_matrix *m, int *rowsLenght, int *colsLenght)
{
    *rowsLenght = AES_matrix_getRowsLenght(m);
    *colsLenght = AES_matrix_getColsLenght(m);
}

int AES_matrix_get(struct AES_matrix *m, size_t x, size_t y)
{
    if (x < AES_matrix_getColsLenght(m)
        &&  y < AES_matrix_getRowsLenght(m))
        return m->data[y][x];
    return -1;
}

void AES_matrix_set(struct AES_matrix *m, size_t x, size_t y, int value)
{
    if (x < AES_matrix_getColsLenght(m)
        &&  y < AES_matrix_getRowsLenght(m))
        m->data[y][x] = value;
}

struct AES_matrix *AES_matrix_add(struct AES_matrix *a, struct AES_matrix *b)
{
    struct AES_matrix *c = AES_matrix_init();
    for (size_t y = 0; y < a->rowsLenght; ++y)
    {
        for (size_t x = 0; x < a->colsLenght; ++x)
        {
            AES_matrix_set(c, x, y, 
                           AES_matrix_get(a, x, y)
                           + AES_matrix_get(b, x, y));
        }
    }
    return c;
}

struct AES_matrix *AES_matrix_sub(struct AES_matrix *a, struct AES_matrix *b)
{
    struct AES_matrix *c = AES_matrix_init();
    for (size_t y = 0; y < a->rowsLenght; ++y)
    {
        for (size_t x = 0; x < a->colsLenght; ++x)
        {
            AES_matrix_set(c, x, y, 
                           AES_matrix_get(a, x, y)
                           - AES_matrix_get(b, x, y));
        }
    }
    return c;
}

struct AES_matrix *AES_matrix_mul(struct AES_matrix *a, struct AES_matrix *b)
{
    struct AES_matrix *c = AES_matrix_init();
    for (size_t y = 0; y < a->rowsLenght; ++y)
    {
        for (size_t x = 0; x < a->colsLenght; ++x)
        {
            int sum = 0;
            for (size_t k = 0; k < b->rowsLenght; ++k)
                sum += AES_matrix_get(a, x, k) 
                    * AES_matrix_get(b, k, y);
            AES_matrix_set(c, x, y, sum);
        }
    }
    return c;
}

void AES_matrix_text2matrix(char *text, struct AES_matrix ***blocks, size_t *count)
{
    size_t lentext = strlen(text);
    *count = (lentext / (AES_MATRIX_DEFAULT_ROWSLENGHT * AES_MATRIX_DEFAULT_COLSLENGHT))
        + (lentext % (AES_MATRIX_DEFAULT_ROWSLENGHT * AES_MATRIX_DEFAULT_COLSLENGHT) == 0 ? 0 : 1);
    *blocks = malloc(sizeof(struct AES_matrix*) * *count);
    struct AES_matrix **tmp = *blocks;
    size_t i = 0;
    size_t ib = 0;
    while (i < lentext)
    {
        struct AES_matrix *m = AES_matrix_init();
        if (m == NULL)
        {
            free(tmp);
            return;
        }
        tmp[ib] = m;
        for (size_t y = 0; y < (tmp[ib])->rowsLenght && i < lentext; ++y)
        {
            for (size_t x = 0; x < (tmp[ib])->colsLenght && i < lentext; ++x)
            {
                AES_matrix_set(tmp[ib], x, y,text[i]);
                ++i;
            }
        }
        ++ib;
    }
}

void AES_matrix_matrix2text(struct AES_matrix **blocks, size_t count, char **text)
{
    size_t lentext = count * (AES_MATRIX_DEFAULT_ROWSLENGHT * AES_MATRIX_DEFAULT_COLSLENGHT);
    *text = malloc(sizeof(char) * lentext);

    size_t i = 0;
    size_t ib = 0;

    while (i < lentext)
    {
        blocks[ib] = AES_matrix_init();
        for (size_t y = 0; y < blocks[ib]->rowsLenght && i < lentext; ++y)
        {
            for (size_t x = 0; x < blocks[ib]->colsLenght && i < lentext; ++x)
            {
                *text[i] = AES_matrix_get(blocks[ib], x, y);
                ++i;
            }
        }
        ++ib;
    }

}

void AES_matrix_printf(struct AES_matrix *block)
{
    printf("AES_matrix: rows: %ld, cols: %ld\n", block->rowsLenght, block->colsLenght);
    printf("{\n");

    for (size_t y = 0; y < block->rowsLenght; ++y)
    {
        printf(" | ");
        for (size_t x = 0; x < block->colsLenght; ++x)
        {
            printf("%d |", AES_matrix_get(block, x, y));
        }
        printf("\n");
    }

    printf("}\n");
}

void AES_matrix_sprintf(struct AES_matrix *block, char **text)
{
    size_t count = 4 + block->rowsLenght*((block->colsLenght * 2) + 1);
    *text = malloc(sizeof(char*) * count);

    char *tmp = *text;

    tmp += sprintf(tmp, "%d|%d\n", block->rowsLenght, block->colsLenght);
    for (size_t y = 0; y < block->rowsLenght; ++y)
    {
        for (size_t x = 0; x < block->colsLenght; ++x)
        {
            tmp += sprintf(tmp, "%d ", AES_matrix_get(block, x, y));
        }
        tmp += sprintf(tmp, "\n");
    }

}







