#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_mixcolumns.h"

uint8_t xtime(uint8_t x)
{
    return ((x<<1) ^ ((x>>7) & 1) * 0x1b);
}

uint8_t gf_mult(uint8_t y, uint8_t x)
{
    return (((y & 1) * x) ^
            ((y>>1 & 1) * xtime(x)) ^
            ((y>>2 & 1) * xtime(xtime(x))) ^
            ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
            ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

struct AES_matrix *AES_matrix_mixColumns(struct AES_matrix *block)
{
    struct AES_matrix *state = AES_matrix_init();
    for (size_t x = 0; x < block->rowsLenght; ++x)
    {
        uint8_t e0 = AES_matrix_get(block, x, 0);
        uint8_t e1 = AES_matrix_get(block, x, 1);
        uint8_t e2 = AES_matrix_get(block, x, 2);
        uint8_t e3 = AES_matrix_get(block, x, 3);

        uint8_t s0 = gf_mult(0x02, e0) ^ gf_mult(0x03, e1) ^ e2 ^ e3;
        uint8_t s1 = e0 ^ gf_mult(0x02, e1) ^ gf_mult(0x03, e2) ^ e3;
        uint8_t s2 = e0 ^ e1 ^ gf_mult(0x02, e2) ^ gf_mult(0x03, e3); 
        uint8_t s3 = gf_mult(0x03, e0) ^ e1 ^ e2 ^ gf_mult(0x02, e3);

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

        uint8_t s0 = gf_mult(0x0e, e0) ^ gf_mult(0x0b, e1) ^ gf_mult(0x0d, e2) ^ gf_mult(0x09, e3);
        uint8_t s1 = gf_mult(0x09, e0) ^ gf_mult(0x0e, e1) ^ gf_mult(0x0b, e2) ^ gf_mult(0x0d, e3);
        uint8_t s2 = gf_mult(0x0d, e0) ^ gf_mult(0x09, e1) ^ gf_mult(0x0e, e2) ^ gf_mult(0x0b, e3); 
        uint8_t s3 = gf_mult(0x0b, e0) ^ gf_mult(0x0d, e1) ^ gf_mult(0x09, e2) ^ gf_mult(0x0e, e3);

        AES_matrix_set(state, 0, y, s0);
        AES_matrix_set(state, 1, y, s1);
        AES_matrix_set(state, 2, y, s2);
        AES_matrix_set(state, 3, y, s3);
    }
    return state;
}
