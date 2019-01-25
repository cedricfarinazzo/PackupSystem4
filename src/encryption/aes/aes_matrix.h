#ifndef _SRC_ENCRYPTION_AES_AES_MATRIX_H_
#define _SRC_ENCRYPTION_AES_AES_MATRIX_H_

#include <stdlib.h>
#include <string.h>

struct AES_matrix {
    size_t rowsLenght;
    size_t colsLenght;
    int **data;
} AES_matrix;

#define AES_MATRIX_DEFAULT_ROWSLENGHT 4
#define AES_MATRIX_DEFAULT_COLSLENGHT 4

struct AES_matrix *AES_matrix_init();

void AES_matrix_free(struct AES_matrix *m);

size_t AES_matrix_getRowsLenght(struct AES_matrix *m);

size_t AES_matrix_getColsLenght(struct AES_matrix *m);

void AES_matrix_getShape(struct AES_matrix *m, int *rowsLenght, int *colsLenght);

int AES_matrix_get(struct AES_matrix *m, size_t x, size_t y);

void AES_matrix_set(struct AES_matrix *m, size_t x, size_t y, int value);

struct AES_matrix *AES_matrix_add(struct AES_matrix *a, struct AES_matrix *b);

struct AES_matrix *AES_matrix_sub(struct AES_matrix *a, struct AES_matrix *b);

struct AES_matrix *AES_matrix_mul(struct AES_matrix *a, struct AES_matrix *b);

void AES_matrix_text2matrix(char *text, struct AES_matrix **blocks, size_t *count);

void AES_matrix_matrix2text(struct AES_matrix **blocks, size_t count, char **text);

#endif /* _SRC_ENCRYPTION_AES_AES_MATRIX_H_ */
