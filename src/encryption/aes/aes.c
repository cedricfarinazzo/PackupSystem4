#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_addroundkey.h"
#include "aes_subbytes.h"
#include "aes_shiftrows.h"
#include "aes_mixcolumns.h"
#include "aes_keyexpansion.h"
#include "aes.h"

struct AES_ctx *AES_init(unsigned char *key, size_t len)
{
    struct AES_ctx *ctx = malloc(sizeof(struct AES_ctx));
    struct AES_matrix **blockskey;
    size_t countkey = 0;
    AES_matrix_text2matrix(key, &blockskey, &countkey, len);
    if (countkey != 1)
    {
        for (size_t i = 0; i < countkey; ++i)
        {
            AES_matrix_free(blockskey[i]);
        }
        free(blockskey);
        return NULL;
    }
    ctx->key = AES_matrix_init();
    AES_matrix_copy(blockskey[0], ctx->key);
    
    for (size_t i = 0; i < countkey; ++i)
    {
        AES_matrix_free(blockskey[i]);
    }
    free(blockskey);

    ctx->roundKeys = AES_keyExpansion(ctx->key);
    return ctx;
}

void AES_ctx_free(struct AES_ctx *ctx)
{
    AES_matrix_free(ctx->key);
    AES_keyExpansion_free(ctx->roundKeys);
    free(ctx);
}

size_t AES_encrypt(struct AES_ctx *ctx, unsigned char *data, size_t lendata, unsigned char **encrypt)
{
    struct AES_matrix **blocksdata;
    size_t countdata = 0;
    AES_matrix_text2matrix(data, &blocksdata, &countdata, lendata);

    for (size_t i = 0; i < countdata; ++i)
    {
        struct AES_matrix *state = AES_matrix_addRoundKey(blocksdata[i], (ctx->roundKeys)[0]);

        for (size_t j = 1; j < 10; ++j)
        {
            struct AES_matrix *tmp = AES_matrix_subBytes(state);
            AES_matrix_copy(tmp, state);
            AES_matrix_free(tmp);

            struct AES_matrix *tmp1 = AES_matrix_shiftRows(state);
            AES_matrix_copy(tmp1, state);
            AES_matrix_free(tmp1);

            struct AES_matrix *tmp2 = AES_matrix_mixColumns(state);
            AES_matrix_copy(tmp2, state);
            AES_matrix_free(tmp2);

            struct AES_matrix *tmp3 = AES_matrix_addRoundKey(state, (ctx->roundKeys)[j]);
            AES_matrix_copy(tmp3, state);
            AES_matrix_free(tmp3);
        }
        struct AES_matrix *tmp4 = AES_matrix_subBytes(state);
        AES_matrix_copy(tmp4, state);
        AES_matrix_free(tmp4);

        struct AES_matrix *tmp5 = AES_matrix_shiftRows(state);
        AES_matrix_copy(tmp5, state);
        AES_matrix_free(tmp5);

        struct AES_matrix *tmp6 = AES_matrix_addRoundKey(state, (ctx->roundKeys)[10]);
        AES_matrix_copy(tmp6, state);
        AES_matrix_free(tmp6);

        AES_matrix_copy(state, blocksdata[i]);
        AES_matrix_free(state);
    }

    AES_matrix_matrix2text(blocksdata, countdata, encrypt);
    
    for (size_t i = 0; i < countdata; ++i)
    {
        AES_matrix_free(blocksdata[i]);
    }
    free(blocksdata);
    return 16 * countdata;
}

size_t AES_decrypt(struct AES_ctx *ctx, unsigned char *encrypt, size_t lendata, unsigned char **decrypt)
{
    struct AES_matrix **blocksen;
    size_t counten = 0;
    AES_matrix_text2matrix(encrypt, &blocksen, &counten, lendata);

    for (size_t i = 0; i < counten; ++i)
    {
        struct AES_matrix *state = AES_matrix_addRoundKey(blocksen[i], (ctx->roundKeys)[10]);

        for (size_t j = 9; j > 0; --j)
        {
            struct AES_matrix *tmp = AES_matrix_InvShiftRows(state);
            AES_matrix_copy(tmp, state);
            AES_matrix_free(tmp);
    
            struct AES_matrix *tmp1 = AES_matrix_InvSubBytes(state);
            AES_matrix_copy(tmp1, state);
            AES_matrix_free(tmp1);

            struct AES_matrix *tmp2 = AES_matrix_addRoundKey(state, (ctx->roundKeys)[j]);
            AES_matrix_copy(tmp2, state);
            AES_matrix_free(tmp2);

            struct AES_matrix *tmp3 = AES_matrix_InvMixColumns(state);
            AES_matrix_copy(tmp3, state);
            AES_matrix_free(tmp3);
        }
        struct AES_matrix *tmp4 = AES_matrix_InvShiftRows(state);
        AES_matrix_copy(tmp4, state);
        AES_matrix_free(tmp4);

        struct AES_matrix *tmp5 = AES_matrix_InvSubBytes(state);
        AES_matrix_copy(tmp5, state);
        AES_matrix_free(tmp5);

        struct AES_matrix *tmp6 = AES_matrix_addRoundKey(state, (ctx->roundKeys)[0]);
        AES_matrix_copy(tmp6, state);
        AES_matrix_free(tmp6);

        AES_matrix_copy(state, blocksen[i]);
        AES_matrix_free(state);
    }

    AES_matrix_matrix2text(blocksen, counten, decrypt);

    for (size_t i = 0; i < counten; ++i)
    {
        AES_matrix_free(blocksen[i]);
    }
    free(blocksen);
    return 16 * counten;
}
