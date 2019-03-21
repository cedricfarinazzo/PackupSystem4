#ifndef _SRC_ENCRYPTION_AES_AES_MATRIX_H_
#define _SRC_ENCRYPTION_AES_AES_MATRIX_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct AES_matrix {
    size_t rowsLenght;
    size_t colsLenght;
    uint8_t **data;
} AES_matrix;

#define AES_MATRIX_DEFAULT_ROWSLENGHT 4
#define AES_MATRIX_DEFAULT_COLSLENGHT 4

struct AES_matrix *AES_matrix_init();

void AES_matrix_free(struct AES_matrix *m);

size_t AES_matrix_getRowsLenght(struct AES_matrix *m);

size_t AES_matrix_getColsLenght(struct AES_matrix *m);

void AES_matrix_getShape(struct AES_matrix *m, size_t *rowsLenght, size_t *colsLenght);

uint8_t AES_matrix_get(struct AES_matrix *m, size_t x, size_t y);

void AES_matrix_set(struct AES_matrix *m, size_t x, size_t y, uint8_t value);

struct AES_matrix *AES_matrix_add(struct AES_matrix *a, struct AES_matrix *b);

struct AES_matrix *AES_matrix_sub(struct AES_matrix *a, struct AES_matrix *b);

struct AES_matrix *AES_matrix_mul(struct AES_matrix *a, struct AES_matrix *b);

void AES_matrix_copy(struct AES_matrix *in, struct AES_matrix *out);

void AES_matrix_text2matrix(unsigned char *text, struct AES_matrix ***blocks, size_t *count, size_t lentext);

void AES_matrix_matrix2text(struct AES_matrix **blocks, size_t count, unsigned char **text);

void AES_matrix_printf(struct AES_matrix *block);

void AES_matrix_printfchar(struct AES_matrix *block);

void AES_matrix_printfhex(struct AES_matrix *block);

void AES_matrix_printfhexline(struct AES_matrix *block);

void AES_matrix_sprintf(struct AES_matrix *block, unsigned char **text);

void AES_matrix_feed(struct AES_matrix *block, uint8_t data[AES_MATRIX_DEFAULT_ROWSLENGHT * AES_MATRIX_DEFAULT_COLSLENGHT]);

int AES_matrix_areEqual(struct AES_matrix *a, struct AES_matrix *b);

#endif /* _SRC_ENCRYPTION_AES_AES_MATRIX_H_ */
