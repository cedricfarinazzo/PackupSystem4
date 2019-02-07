#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_addroundkey.h"

struct AES_matrix *AES_matrix_addRoundKey(struct AES_matrix *block, struct AES_matrix *key)
{
    struct AES_matrix *state = AES_matrix_init();
    for (size_t y = 0; y < block->rowsLenght; ++y)
    {
        for (size_t x = 0; x < block->colsLenght; ++x)
        {
            AES_matrix_set(state, x, y, 
                           AES_matrix_get(block, x, y)^AES_matrix_get(key, x, y));
        }
    }
    return state;
}
