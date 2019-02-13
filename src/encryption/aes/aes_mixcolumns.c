#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_mixcolumns.h"

struct AES_matrix *AES_matrix_mixColumns(struct AES_matrix *block)
{
    struct AES_matrix *state = AES_matrix_init();
    for (size_t x = 0; x < block->rowsLenght; ++x)
    {

        uint8_t e0 = AES_matrix_get(state, x, 0);
        uint8_t e1 = AES_matrix_get(state, x, 1);
        uint8_t e2 = AES_matrix_get(state, x, 2);
        uint8_t e3 = AES_matrix_get(state, x, 3);

        uint8_t s0 = (0x02 * e0) ^ (0x03 * e1) ^ e2 ^ e3;
        uint8_t s1 = e0 ^ (0x02 * e1) ^ (0x03 * e2) ^ e3;
        uint8_t s2 = e0 ^ e1 ^ (0x02 * e2) ^ (0x03 * e3); 
        uint8_t s3 = (0x03 * e0) ^ e1 ^ e2 ^ (0x02 * e3);

        AES_matrix_set(state, x, 0, s0);
        AES_matrix_set(state, x, 1, s1);
        AES_matrix_set(state, x, 2, s2);
        AES_matrix_set(state, x, 3, s3);
    }
    return state;
}

struct AES_matrix *AES_matrix_InvMixColumns(struct AES_matrix *block)
{
    struct AES_matrix *state = AES_matrix_init();
    for (size_t y = 0; y < block->rowsLenght; ++y)
    {

        uint8_t e0 = AES_matrix_get(state, 0, y);
        uint8_t e1 = AES_matrix_get(state, 1, y);
        uint8_t e2 = AES_matrix_get(state, 2, y);
        uint8_t e3 = AES_matrix_get(state, 3, y);

        uint8_t s0 = (14 * e0) ^ (11 * e1) ^ (13 * e2) ^ (9 * e3);
        uint8_t s1 = (9 * e0) ^ (14 * e1) ^ (11 * e2) ^ (13 * e3);
        uint8_t s2 = (13 * e0) ^ (9 * e1) ^ (14 * e2) ^ (11 * e3); 
        uint8_t s3 = (11 * e0) ^ (13 * e1) ^ (9 * e2) ^ (14 * e3);

        AES_matrix_set(state, 0, y, s0);
        AES_matrix_set(state, 1, y, s1);
        AES_matrix_set(state, 2, y, s2);
        AES_matrix_set(state, 3, y, s3);
    }
    return state;
}
