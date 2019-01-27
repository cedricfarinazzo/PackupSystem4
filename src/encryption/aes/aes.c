#include <stdlib.h>

#include "aes_matrix.h"
#include "aes_addroundkey.h"
#include "aes_subbytes.h"
#include "aes_shiftrows.h"
#include "aes_mixcolumns.h"
#include "aes.h"

void AES_encrypt(char *data, char *key, char **encrypt)
{
    struct AES_matrix **blockskey;
    size_t countkey = 0;
    AES_matrix_text2matrix(key, &blockskey, &countkey);
    if (countkey != 1)
    {
        for (size_t i = 0; i < countkey; ++i)
        {
            AES_matrix_free(blockskey[i]);
        }
        free(blockskey);
        return;
    }
    struct AES_matrix *keyblock = blockskey[0];


    struct AES_matrix **blocksdata;
    size_t countdata = 0;
    AES_matrix_text2matrix(data, &blocksdata, &countdata);


    for (size_t i = 0; i < countdata; ++i)
    {
        struct AES_matrix *state = AES_matrix_addRoundKey(blocksdata[i], keyblock);

        for (size_t j = 0; j < 9; ++j)
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

            struct AES_matrix *tmp3 = AES_matrix_addRoundKey(state, keyblock);
            AES_matrix_copy(tmp3, state);
            AES_matrix_free(tmp3);
        }
        struct AES_matrix *tmp4 = AES_matrix_subBytes(state);
        AES_matrix_copy(tmp4, state);
        AES_matrix_free(tmp4);

        struct AES_matrix *tmp5 = AES_matrix_shiftRows(state);
        AES_matrix_copy(tmp5, state);
        AES_matrix_free(tmp5);

        struct AES_matrix *tmp6 = AES_matrix_addRoundKey(state, keyblock);
        AES_matrix_copy(tmp6, state);
        AES_matrix_free(tmp6);

        AES_matrix_copy(state, blocksdata[i]);
        AES_matrix_free(state);
    }


    AES_matrix_matrix2text(blocksdata, countdata, encrypt);

    for (size_t i = 0; i < countkey; ++i)
    {
        AES_matrix_free(blockskey[i]);
    }
    free(blockskey);
    for (size_t i = 0; i < countdata; ++i)
    {
        AES_matrix_free(blocksdata[i]);
    }
    free(blocksdata);
}

void AES_decrypt(char *encrypt, char *key, char **decrypt)
{


    struct AES_matrix **blocksencrypt;
    size_t countencrypt = 0;
    AES_matrix_text2matrix(encrypt, &blocksencrypt, &countencrypt);

    struct AES_matrix **blockskey;
    size_t countkey = 0;
    AES_matrix_text2matrix(key, &blockskey, &countkey);


    //do something


    AES_matrix_matrix2text(blocksencrypt, countencrypt, decrypt);

    for (size_t i = 0; i < countkey; ++i)
    {
        AES_matrix_free(blockskey[i]);
    }
    free(blockskey);
    for (size_t i = 0; i < countencrypt; ++i)
    {
        AES_matrix_free(blocksencrypt[i]);
    }
    free(blocksencrypt);




    struct AES_matrix **blocks;
    size_t count = 0;
    AES_matrix_text2matrix(encrypt, &blocks, &count);
    //do something


    AES_matrix_matrix2text(blocks, count, decrypt);

    for (size_t i = 0; i < count; ++i)
    {
        AES_matrix_free(blocks[i]);
    }
    free(blocks);
}

