#include "aes_matrix.h"
#include "aes_subbytes.h"
#include "aes_keyexpansion.h"

const uint8_t rcon[40] = {
    //           0     1     2     3     4     5     6     7     8     9
    /* 1 */     0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
    /* 2 */     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 3 */     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 4 */     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void printTab(uint8_t *a)
{
    for (size_t i = 0; i < AES_MATRIX_DEFAULT_COLSLENGHT; ++i)
        printf("0x%02x ", a[i]);
    printf("\n");
}

uint8_t __GetRcon(int x, int y)
{
    return rcon[y * 10 + x];
}

void __copyColToTab(struct AES_matrix *m, size_t k, uint8_t *out)
{
    for (size_t i = 0; i < AES_MATRIX_DEFAULT_COLSLENGHT; ++i)
        out[i] = AES_matrix_get(m, k, i);
}

void __copyTabToCol(struct AES_matrix *m, size_t k, uint8_t *out)
{
    for (size_t i = 0; i < AES_MATRIX_DEFAULT_COLSLENGHT; ++i)
        AES_matrix_set(m, k, i, out[i]);
}

void __rotWord(uint8_t *in, uint8_t *out)
{
    for (size_t i = 0; i < AES_MATRIX_DEFAULT_COLSLENGHT; ++i)
        out[i] = in[(i+AES_MATRIX_DEFAULT_COLSLENGHT+1)%AES_MATRIX_DEFAULT_COLSLENGHT];
}

void __subBytes(uint8_t *in)
{
    for (size_t i = 0; i < AES_MATRIX_DEFAULT_COLSLENGHT; ++i)
        in[i] = AES_matrix_subBytesInt(in[i]); 
}

void __xorKeyS(uint8_t *in, uint8_t *prev4, size_t keyi)
{
    for (size_t i = 0; i < AES_MATRIX_DEFAULT_COLSLENGHT; ++i)
        in[i] = prev4[i] ^ in[i] ^ __GetRcon(keyi, i); 
}

void __xorKey(uint8_t *in, uint8_t *prev4)
{
    for (size_t i = 0; i < AES_MATRIX_DEFAULT_COLSLENGHT; ++i)
        in[i] = prev4[i] ^ in[i]; 
}

struct AES_matrix **AES_keyExpansion(struct AES_matrix *key)
{
    AES_matrix_printfhex(key);
    struct AES_matrix **roundKeys = malloc(sizeof(struct AES_matrix*) * (AES_NB_ROUND_KEY + 1));
    size_t index = 0;
    size_t col = 4;
    roundKeys[index] = AES_matrix_init();
    AES_matrix_copy(key, roundKeys[index]);
    ++index;

    for (; index < AES_NB_ROUND_KEY + 1; ++index)
    {
        roundKeys[index] = AES_matrix_init();
        for (size_t x = 0; x < AES_MATRIX_DEFAULT_ROWSLENGHT; ++x)
        {
            uint8_t in[AES_MATRIX_DEFAULT_COLSLENGHT];
            uint8_t srot[AES_MATRIX_DEFAULT_COLSLENGHT];
            uint8_t prev4[AES_MATRIX_DEFAULT_COLSLENGHT];
             
            __copyColToTab(roundKeys[index - 1], x, prev4);
            if (x == 0)
                __copyColToTab(roundKeys[index - 1], AES_MATRIX_DEFAULT_COLSLENGHT - 1, in);
            else
                __copyColToTab(roundKeys[index], x - 1, in);

    //printTab(in);
    //printTab(prev4);
   // printf("\n");
             

            if (x == 0) {
                __rotWord(in, srot);
                __subBytes(srot);
                __xorKeyS(srot, prev4, index - 1);
                __copyTabToCol(roundKeys[index], x, srot);
            } else {
                __xorKey(in, prev4);
                __copyTabToCol(roundKeys[index], x, in);
            }
           ++col;
        }
        printf("\n====================== \n   %ld\n", index);
        AES_matrix_printfhex(roundKeys[index]);
    }
    exit(1);
    return roundKeys;
}

void AES_keyExpansion_free(struct AES_matrix **roundKey)
{
    for (size_t i = 0; i < (AES_NB_ROUND_KEY + 1); ++i)
        AES_matrix_free(roundKey[i]);
    free(roundKey);
}
