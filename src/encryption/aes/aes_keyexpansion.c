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

uint8_t __GetRcon(int x, int y)
{
    return rcon[y * 10 + x];
}

struct AES_matrix **AES_keyExpansion(struct AES_matrix *key)
{
    struct AES_matrix **roundKeys = malloc(sizeof(struct AES_matrix*) * (AES_NB_ROUND_KEY + 1));
    size_t index = 0;
    roundKeys[index] = AES_matrix_init();
    AES_matrix_copy(key, roundKeys[index]);
    ++index;

    for (; index < AES_NB_ROUND_KEY + 1; ++index)
    {
        roundKeys[index] = AES_matrix_init();
        for (size_t x = 0; x < AES_MATRIX_DEFAULT_ROWSLENGHT; ++x)
        {
            int tmp[AES_MATRIX_DEFAULT_COLSLENGHT];
            size_t previous_collum_bloc = (x == 0 ? index - 1 : index);
            size_t previous_collum_x = x == 0 ? AES_MATRIX_DEFAULT_ROWSLENGHT - 1 : x - 1;

            for (size_t y = 0; y < AES_MATRIX_DEFAULT_COLSLENGHT; ++y)
                tmp[y] = AES_matrix_get(roundKeys[previous_collum_bloc], previous_collum_x, (y - 1 + 4) % 4);
            for (size_t y = 0; y < AES_MATRIX_DEFAULT_COLSLENGHT; ++y)
                tmp[y] = AES_matrix_subBytesInt(tmp[y]); 
            for (size_t y = 0; y < AES_MATRIX_DEFAULT_COLSLENGHT; ++y)
                tmp[y] = tmp[y] ^ AES_matrix_get(roundKeys[index - 1], x, y) ^ __GetRcon(index - 1, y);
            for (size_t y = 0; y < AES_MATRIX_DEFAULT_COLSLENGHT; ++y)
                AES_matrix_set(roundKeys[index], x, y, tmp[y]);
        }
    }
    return roundKeys;
}

void AES_keyExpansion_free(struct AES_matrix **roundKey)
{
    for (size_t i = 0; i < (AES_NB_ROUND_KEY + 1); ++i)
        AES_matrix_free(roundKey[i]);
    free(roundKey);
}
