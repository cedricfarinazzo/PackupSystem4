#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <criterion/criterion.h>

#include "../src/encryption/aes/aes.h"
#include "../src/encryption/aes/aes_matrix.h"
#include "../src/encryption/aes/aes_addroundkey.h"
#include "../src/encryption/aes/aes_shiftrows.h"
#include "../src/encryption/aes/aes_subbytes.h"
#include "../src/encryption/aes/aes_mixcolumns.h"


void setup(void)
{
    srand(time(NULL));
}


// AES
Test(AES, printf_matrix)
{
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_set(mat, 1, 1, 1);
    AES_matrix_set(mat, 1, 2, 2);
    AES_matrix_printf(mat);
    AES_matrix_free(mat);
}

Test(AES, sprintf_matrix)
{

    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_set(mat, 1, 3, 1);
    AES_matrix_set(mat, 0, 2, 2);
    char *text;
    AES_matrix_sprintf(mat, &text);
    AES_matrix_free(mat);
    printf("%s", text);
    free(text);
}

Test(AES, addRoundKey)
{
    struct AES_matrix *mat = AES_matrix_init();
    struct AES_matrix *key = AES_matrix_init();
    for (size_t y = 0; y < mat->rowsLenght; ++y)
    {
        for (size_t x = 0; x < mat->colsLenght; ++x)
        {
            AES_matrix_set(mat, x, y, rand() % 255);
        }
        for (size_t x = 0; x < key->colsLenght; ++x)
        {
            AES_matrix_set(key, x, y, rand() % 255);
        }
    }

    struct AES_matrix *state = AES_matrix_addRoundKey(mat, key);

    for (size_t y = 0; y < mat->rowsLenght; ++y)
    {
        for (size_t x = 0; x < key->colsLenght; ++x)
        {
            cr_assert((AES_matrix_get(mat, x, y) ^ AES_matrix_get(key, x, y))
                      == AES_matrix_get(state, x, y));
        }
    }

    AES_matrix_free(state);
    AES_matrix_free(mat);
    AES_matrix_free(key);

}

Test(AES, shiftRows)
{
    struct AES_matrix *mat = AES_matrix_init();
    for (size_t y = 0; y < mat->rowsLenght; ++y)
    {
        for (size_t x = 0; x < mat->colsLenght; ++x)
        {
            AES_matrix_set(mat, x, y, rand() % 255);
        }
    }

    struct AES_matrix *state = AES_matrix_shiftRows(mat);

    for (size_t y = 0; y < mat->rowsLenght; ++y)
    {
        for (size_t x = 0; x < mat->colsLenght; ++x)
        {
            cr_assert(AES_matrix_get(state, x, y)
                      == AES_matrix_get(mat, (x + y)%4, y));
        }
    }

    AES_matrix_free(state);
    AES_matrix_free(mat);

}

Test(AES, subBytes)
{
    struct AES_matrix *mat = AES_matrix_init();
    for (size_t y = 0; y < mat->rowsLenght; ++y)
    {
        for (size_t x = 0; x < mat->colsLenght; ++x)
        {
            AES_matrix_set(mat, x, y, rand() % 255);
        }
    }

    struct AES_matrix *state = AES_matrix_subBytes(mat);

    if (state == NULL)
    {
        AES_matrix_free(mat);
        cr_assert_fail("state = null");
    }

    AES_matrix_free(state);
    AES_matrix_free(mat);
}

Test(AES, mixColumns)
{
    struct AES_matrix *mat = AES_matrix_init();
    for (size_t y = 0; y < mat->rowsLenght; ++y)
    {
        for (size_t x = 0; x < mat->colsLenght; ++x)
        {
            AES_matrix_set(mat, x, y, rand() % 255);
        }
    }

    struct AES_matrix *state = AES_matrix_mixColumns(mat);

    AES_matrix_free(state);
    AES_matrix_free(mat);
}

Test(AES, text2matrix)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

    struct AES_matrix **blocks;
    size_t count = 0;

    AES_matrix_text2matrix(text, &blocks, &count);

    for (size_t i = 0; i < count; ++i)
    {
        AES_matrix_free(blocks[i]);
    }
    free(blocks);
}


Test(AES, isEncrypted)
{
    srand(time(NULL));
    size_t key_size = 128;
    char key[key_size];
    size_t data_size = 256;
    char data[data_size];
    char *output = NULL;

    for (size_t i; i < key_size; ++i)
        key[i] = rand()%255;

    for (size_t i; i < data_size; ++i)
        data[i] = rand()%255;

    size_t count = AES_encrypt(data, data_size, key, key_size, &output);

    cr_expect_fail("Not implemented");
    //cr_assert_not_null(output);
    //cr_assert_str_not_empty(output);
    //cr_assert_str_neq(data, output);

    free(output);
}

Test(AES, EncryptDecrypt)
{
    srand(time(NULL));
    size_t key_size = 128;
    char key[key_size];
    size_t data_size = 256;
    char data[data_size];
    char *encrypt = NULL;
    char *decrypt = NULL;

    for (size_t i; i < key_size; ++i)
        key[i] = rand()%255;

    for (size_t i; i < data_size; ++i)
        data[i] = rand()%255;

    size_t encrypt_size = AES_encrypt(data, data_size, key, key_size, &encrypt);

    size_t decrypt_size = AES_decrypt(encrypt, encrypt_size, key, key_size, &decrypt);

    cr_expect_fail("Not implemented");
    //cr_assert_not_null(decrypt);
    //cr_assert_str_not_empty(decrypt);
    //cr_assert_str_eq(data, decrypt);

    free(encrypt);
    free(decrypt);
}

