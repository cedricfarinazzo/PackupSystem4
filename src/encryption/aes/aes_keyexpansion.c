#include "aes_matrix.h"
#include "aes_subbytes.h"
#include "aes_keyexpansion.h"

struct AES_matrix **AES_keyExpansion(struct AES_matrix *key)
{
    struct AES_matrix **roundKeys = malloc(sizeof(struct AES_matrix*) * (AES_NB_ROUND_KEY + 1));
    size_t index = 0;
    roundKeys[index] = AES_matrix_init();
    AES_matrix_copy(key, roundKeys[index]);
    ++index;

    for (index; index < AES_NB_ROUND_KEY + 1; ++index)
    {
        roundKeys[index] = AES_matrix_init();
        for (size_t x = 0; x < AES_MATRIX_DEFAULT_ROWSLENGHT; ++x)
        {
            int previous_tmp[AES_MATRIX_DEFAULT_COLSLENGHT];
            size_t previous_collum_bloc = x == 0 ? index - 1 : index;
            size_t previous_collum_x = x == 0 ? AES_MATRIX_DEFAULT_ROWSLENGHT - 1 : x - 1;

            for (size_t y = 0; y < AES_MATRIX_DEFAULT_COLSLENGHT; ++y)
                previous_tmp[y] = AES_matrix_get(roundKeys[previous_collum_x], previous_collum_x, (y - 1 + 4) % 4);

            for (size_t y = 0; y < AES_MATRIX_DEFAULT_COLSLENGHT; ++y)
                previous_tmp[y] = AES_matrix_subBytesInt(previous_tmp[y]); 
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
