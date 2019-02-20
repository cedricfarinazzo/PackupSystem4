#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../src/tools/base64.h"

#include <criterion/criterion.h>
#include <criterion/hooks.h>

#include "../src/encryption/aes/aes.h"
#include "../src/encryption/aes/aes_matrix.h"
#include "../src/encryption/aes/aes_addroundkey.h"
#include "../src/encryption/aes/aes_shiftrows.h"
#include "../src/encryption/aes/aes_subbytes.h"
#include "../src/encryption/aes/aes_mixcolumns.h"

#include "../src/encryption/rotn.h"

#include "../src/encryption/vigenere.h"

#include "../src/encryption/rsa/rsa.h"

char *decrypt = NULL;
char *output = NULL;
char *out;
char *vkey = NULL;

int ROTNkey = 13;

char *rand_string(size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.&é\"'(-è_ç^\\@)][{#},?;:/!§$%";
    if (size) {
        char *str = malloc(sizeof(char) * size);
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
        return str;
    }
    return NULL;
}

ReportHook(PRE_ALL)(struct criterion_test_set *test __attribute__((unused)))
{
    srand(time(NULL));
    ROTNkey = rand() % 255;
}

void myfree(void *e)
{
    if (e != NULL)
        free(e);
}

void teardown(void)
{
    myfree(decrypt);
    myfree(output);
    myfree(out);
    myfree(vkey);
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
    cr_assert_not_null(text);
    cr_assert_str_not_empty(text);
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
            AES_matrix_set(mat, x, y, (rand() % (255 - 30)) + 30);
        }
        for (size_t x = 0; x < key->colsLenght; ++x)
        {
            AES_matrix_set(key, x, y, (rand() % (255 - 30)) + 30);
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
            AES_matrix_set(mat, x, y, (rand() % (255 - 30)) + 30);
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
        cr_assert_fail("state = NULL");
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

    size_t len = strlen(text);
    size_t nbblock = (len / 16) + (len%16 != 0 ? 1 : 0);

    AES_matrix_text2matrix(text, &blocks, &count);

    for (size_t i = 0; i < count; ++i)
    {
        AES_matrix_free(blocks[i]);
    }
    free(blocks);

    cr_assert_eq(nbblock, count);
}

Test(AES, matrix2text)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

    struct AES_matrix **blocks;
    size_t count = 0;

    AES_matrix_text2matrix(text, &blocks, &count);

    out = NULL;

    AES_matrix_matrix2text(blocks, count, &out);

    for (size_t i = 0; i < count; ++i)
    {
        AES_matrix_free(blocks[i]);
    }
    free(blocks);

    cr_assert_str_eq(out, text);
}


Test(AES, Encryption)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    output = NULL;
    char key[] = "01G345a.89sbhdef";

    //printf("\nkey: %s  | text: %s\n", key, text);
    AES_encrypt(text, key, &output);

    if (output == NULL)
        cr_assert_fail("output = NULL");
    
    //printf("encryption: %s\n\n", output);
 
    cr_assert_not_null(output);
    cr_assert_str_not_empty(output);
    cr_assert_str_neq(output, text);
}


Test(AES, Decrypt)
{

    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    char *output = NULL;
    decrypt = NULL;
    char key[] = "01G345a.89sbhdef";

    AES_encrypt(text, key, &output);

    if (output == NULL)
        cr_assert_fail("output = NULL");

    AES_decrypt(output, key, &decrypt);

    free(output);

    cr_assert_not_null(decrypt);;
    cr_assert_str_not_empty(decrypt);
    cr_assert_str_eq(decrypt, text);
}

// RSA
Test(RSA, encrypt)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    size_t lentext = strlen(text);
    unsigned char data[lentext + 1];
    for (size_t i = 0; i < lentext; ++i)
        data[i] = (unsigned char)text[i];
    data[lentext] = 0;

    mpz_t p; 
    mpz_init_set_ui(p, 1237);
    
    mpz_t q; 
    mpz_init_set_ui(q, 2003);

    struct RSA_publickey *pub = RSA_gen_public_key(p, q);
    struct RSA_privatekey *pri = RSA_gen_private_key(p, q, pub);
    
    mpz_t *encrypt = RSA_encode(pub, data, lentext);
    
    cr_assert_not_null(encrypt);
    
    for (size_t i = 0; i < lentext; ++i)
        mpz_clear(encrypt[i]);
    free(encrypt);

    RSA_free_public_key(pub);
    RSA_free_private_key(pri);
    mpz_clear(p);
    mpz_clear(q);
}

Test(RSA, decrypt)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum. é~è@¹~#{[-è_çà)^";
    size_t lentext = strlen(text);
    unsigned char data[lentext + 1];
    for (size_t i = 0; i < lentext; ++i)
        data[i] = (unsigned char)text[i];
    data[lentext] = 0;

    mpz_t p; 
    mpz_init_set_ui(p, 1237);
    
    mpz_t q; 
    mpz_init_set_ui(q, 2003);

    struct RSA_publickey *pub = RSA_gen_public_key(p, q);
    struct RSA_privatekey *pri = RSA_gen_private_key(p, q, pub);
    
    mpz_t *encrypt = RSA_encode(pub, data, lentext);
    
    cr_assert_not_null(encrypt);
    
    unsigned char *decode = RSA_decode(pri, encrypt, lentext);
    
    cr_assert_not_null(decode);
    cr_assert_str_not_empty((char*)decode);
    cr_assert_str_eq((char*)decode, text);

    for (size_t i = 0; i < lentext; ++i)
        mpz_clear(encrypt[i]);
    free(encrypt);

    RSA_free_public_key(pub);
    RSA_free_private_key(pri);
    mpz_clear(p);
    mpz_clear(q);
}





// ROTN
Test(ROTN, encrypt)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    size_t lentext = strlen(text);
    char data[lentext + 1];
    for (size_t i = 0; i < lentext; ++i)
        data[i] = text[i];
    data[lentext] = 0;

    ROTN_encrypt(data, ROTNkey);

    cr_assert_not_null(data);
    cr_assert_str_not_empty(data);
    cr_assert_str_neq(data, text);
}

Test(ROTN, decrypt)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum. é~è@¹~#{[-è_çà)^";
    size_t lentext = strlen(text);
    char data[lentext + 1];
    for (size_t i = 0; i < lentext; ++i)
        data[i] = text[i];
    data[lentext] = 0;

    ROTN_encrypt(data, ROTNkey);

    ROTN_decrypt(data, ROTNkey);

    cr_assert_not_null(data);
    cr_assert_str_not_empty(data);
    cr_assert_str_eq(data, text);
}



// Vigenere
Test(VIGENERE, encrypt)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    size_t lentext = strlen(text);
    char data[lentext + 1];
    for (size_t i = 0; i < lentext; ++i)
        data[i] = text[i];
    data[lentext] = 0;

    size_t vkeylen = rand() % 500;
    char *vkey = rand_string(vkeylen);

    VIGENERE_encrypt(data, vkey);

    free(vkey); vkey = NULL;

    cr_assert_not_null(data);
    cr_assert_str_not_empty(data);
    cr_assert_str_neq(data, text);
}

Test(VIGENERE, decrypt)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    size_t lentext = strlen(text);
    char data[lentext + 1];
    for (size_t i = 0; i < lentext; ++i)
        data[i] = text[i];
    data[lentext] = 0;

    size_t vkeylen = rand() % 500;
    char *vkey = rand_string(vkeylen);

    VIGENERE_encrypt(data, vkey);

    VIGENERE_decrypt(data, vkey);

    free(vkey); vkey = NULL;

    cr_assert_not_null(data);
    cr_assert_str_not_empty(data);
    cr_assert_str_eq(data, text);

}
