#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <criterion/criterion.h>

#include "../src/encryption/aes/aes.h"

// AES
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

    cr_assert_not_null(output);
    cr_assert_str_not_empty(output);
    cr_assert_str_neq(data, output);
    
    free(output);
}

// AES
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
    
    cr_assert_not_null(decrypt);
    cr_assert_str_not_empty(decrypt);
    cr_assert_str_eq(data, decrypt);
    
    free(encrypt);
    free(decrypt);
}
