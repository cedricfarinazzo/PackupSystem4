#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_mixcolumns.h"

struct AES_matrix *AES_matrix_mixColumns(struct AES_matrix *block)
{
    struct AES_matrix *state = AES_matrix_init();
    for (size_t y = 0; y < block->rowsLenght; ++y)
    {

        int e0 = AES_matrix_get(state, 0, y);
        int e1 = AES_matrix_get(state, 1, y);
        int e2 = AES_matrix_get(state, 2, y);
        int e3 = AES_matrix_get(state, 3, y);

        int s0 = (2 * e0) ^ (3 * e1) ^ e2 ^ e3;
        int s1 = e0 ^ (2 * e1) ^ (3 * e2) ^ e3;
        int s2 = e0 ^ e1 ^ (2 * e2) ^ (3 * e3); 
        int s3 = (3 * e0) ^ e1 ^ e2 ^ (2 * e3);

        AES_matrix_set(state, 0, y, s0);
        AES_matrix_set(state, 1, y, s1);
        AES_matrix_set(state, 2, y, s2);
        AES_matrix_set(state, 3, y, s3);
    }
    return state;
}

struct AES_matrix *AES_matrix_InvMixColumns(struct AES_matrix *block)
{
    struct AES_matrix *state = AES_matrix_init();
    for (size_t y = 0; y < block->rowsLenght; ++y)
    {

        int e0 = AES_matrix_get(state, 0, y);
        int e1 = AES_matrix_get(state, 1, y);
        int e2 = AES_matrix_get(state, 2, y);
        int e3 = AES_matrix_get(state, 3, y);

        int s0 = (14 * e0) ^ (11 * e1) ^ (13 * e2) ^ (9 * e3);
        int s1 = (9 * e0) ^ (14 * e1) ^ (11 * e2) ^ (13 * e3);
        int s2 = (13 * e0) ^ (9 * e1) ^ (14 * e2) ^ (11 * e3); 
        int s3 = (11 * e0) ^ (13 * e1) ^ (9 * e2) ^ (14 * e3);

        AES_matrix_set(state, 0, y, s0);
        AES_matrix_set(state, 1, y, s1);
        AES_matrix_set(state, 2, y, s2);
        AES_matrix_set(state, 3, y, s3);
    }
    return state;
}
