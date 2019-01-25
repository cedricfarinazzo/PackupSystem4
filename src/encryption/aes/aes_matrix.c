#include <stdlib.h>

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

