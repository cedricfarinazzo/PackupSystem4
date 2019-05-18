#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <gmp.h>

#include "../src/tools/base64.h"
#include "../src/encryption/aes/sha1.h"

#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include <criterion/redirect.h>

#include "../src/encryption/aes/aes.h"
#include "../src/encryption/aes/hashpass.h"
#include "../src/encryption/aes/aes_file.h"
#include "../src/encryption/aes/aes_matrix.h"
#include "../src/encryption/aes/aes_addroundkey.h"
#include "../src/encryption/aes/aes_shiftrows.h"
#include "../src/encryption/aes/aes_subbytes.h"
#include "../src/encryption/aes/aes_mixcolumns.h"
#include "../src/encryption/aes/aes_keyexpansion.h"
#include "../src/encryption/rotn.h"

#include "../src/encryption/vigenere.h"

#include "../src/encryption/rsa/rsa.h"
#include "../src/encryption/rsa/rsa_file.h"
#include "../src/encryption/rsa/genkey.h"

#include "../src/encryption/elgamal/elgamal.h"
#include "../src/encryption/elgamal/genkey.h"

#include "../src/encryption/encryption.h"

unsigned char *decrypt = NULL;
unsigned char *output = NULL;
unsigned char *out;
char *vkey = NULL;

int ROTNkey = 13;

unsigned char *rand_string(size_t size)
{
    const unsigned char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.&é\"'(-è_ç^\\@)][{#},?;:/!§$%";
    if (size) {
        unsigned char *str = malloc(sizeof(unsigned char) * size);
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
    unsigned char *text;
    AES_matrix_sprintf(mat, &text);
    AES_matrix_free(mat);
    cr_assert_not_null(text);
    cr_assert_str_not_empty((char*)text);
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

Test(AES, addRoundKey2)
{

    uint8_t data[16] = 
    {
        0x04, 0xe0, 0x48, 0x28,
        0x66, 0xcb, 0xf8, 0x06,
        0x81, 0x19, 0xd3, 0x26,
        0xe5, 0x9a, 0x7a, 0x4c,
    };
    uint8_t key[16] = 
    {
        0xa0, 0x88, 0x23, 0x2a,
        0xfa, 0x54, 0xa3, 0x6c,
        0xfe, 0x2c, 0x39, 0x76,
        0x17, 0xb1, 0x39, 0x05,
    };
    uint8_t exp[16] = 
    {
        0xa4, 0x68, 0x6b, 0x02,
        0x9c, 0x9f, 0x5b, 0x6a,
        0x7f, 0x35, 0xea, 0x50,
        0xf2, 0x2b, 0x43, 0x49,
    };
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_feed(mat, data);

    struct AES_matrix *expm = AES_matrix_init();
    AES_matrix_feed(expm, exp);

    struct AES_matrix *keym = AES_matrix_init();
    AES_matrix_feed(keym, key);

    struct AES_matrix *r = AES_matrix_addRoundKey(mat, keym);

    cr_assert(AES_matrix_areEqual(r, expm));

    AES_matrix_free(r);

    AES_matrix_free(mat);
    AES_matrix_free(keym);
    AES_matrix_free(expm);
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

Test(AES, shiftRows2)
{
    uint8_t data[16] = 
    {
        0x63, 0xeb, 0x9f, 0xa0,
        0xc0, 0x2f, 0x93, 0x92,
        0xab, 0x30, 0xaf, 0xc7,
        0x20, 0xcb, 0x2b, 0xa2,
    };
    uint8_t exp[16] = 
    {
        0x63, 0xeb, 0x9f, 0xa0,
        0x2f, 0x93, 0x92, 0xc0,
        0xaf, 0xc7, 0xab, 0x30,
        0xa2, 0x20, 0xcb, 0x2b,
    };
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_feed(mat, data);

    struct AES_matrix *expm = AES_matrix_init();
    AES_matrix_feed(expm, exp);

    struct AES_matrix *r = AES_matrix_shiftRows(mat);

    cr_assert(AES_matrix_areEqual(r, expm));

    AES_matrix_free(r);
    AES_matrix_free(mat);
    AES_matrix_free(expm);
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

Test(AES, subBytes2)
{
    uint8_t data[16] = 
    {
        0xea, 0x04, 0x65, 0x85,
        0x83, 0x45, 0x5d, 0x96,
        0x5c, 0x33, 0x98, 0xb0,
        0xf0, 0x2d, 0xad, 0xc5,
    };
    uint8_t exp[16] = 
    {
        0x87, 0xf2, 0x4d, 0x97,
        0xec, 0x6e, 0x4c, 0x90,
        0x4a, 0xc3, 0x46, 0xe7,
        0x8c, 0xd8, 0x95, 0xA6,
    };
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_feed(mat, data);

    struct AES_matrix *expm = AES_matrix_init();
    AES_matrix_feed(expm, exp);

    struct AES_matrix *r = AES_matrix_subBytes(mat);

    cr_assert(AES_matrix_areEqual(r, expm));

    AES_matrix_free(mat);
    AES_matrix_free(r);
    AES_matrix_free(expm);
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

Test(AES, mixColumns2)
{
    uint8_t data[16] = 
    {
        0xdb, 0xf2, 0x01, 0x2d,
        0x13, 0x0a, 0x01, 0x26,
        0x53, 0x22, 0x01, 0x31,
        0x45, 0x5c, 0x01, 0x4c,

    };
    uint8_t exp[16] = 
    {
        0x8e, 0x9f, 0x01, 0x4d,
        0x4d, 0xdc, 0x01, 0x7e,
        0xa1, 0x58, 0x01, 0xbd,
        0xbc, 0x9d, 0x01, 0xf8,

    };
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_feed(mat, data);

    struct AES_matrix *expm = AES_matrix_init();
    AES_matrix_feed(expm, exp);

    struct AES_matrix *mi = AES_matrix_mixColumns(mat);

    cr_assert(AES_matrix_areEqual(mi, expm));

    AES_matrix_free(mi);

    AES_matrix_free(mat);
    AES_matrix_free(expm);

}


Test(AES, text2matrix)
{
    unsigned char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

    struct AES_matrix **blocks;
    size_t count = 0;

    size_t len = strlen((char*)text);
    size_t nbblock = (len / 16) + (len%16 != 0 ? 1 : 0);

    AES_matrix_text2matrix(text, &blocks, &count, strlen((char*)text));

    for (size_t i = 0; i < count; ++i)
    {
        AES_matrix_free(blocks[i]);
    }
    free(blocks);

    cr_assert_eq(nbblock, count);
}

Test(AES, matrix2text)
{
    unsigned char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

    struct AES_matrix **blocks;
    size_t count = 0;

    AES_matrix_text2matrix(text, &blocks, &count, strlen((char*)text));

    out = NULL;

    AES_matrix_matrix2text(blocks, count, &out);

    for (size_t i = 0; i < count; ++i)
    {
        AES_matrix_free(blocks[i]);
    }
    free(blocks);

    cr_assert_str_eq((char*)out, (char*)text);
}


Test(AES, Encryption)
{
    unsigned char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    output = NULL;
    unsigned char key[] = "01G345a.89sbhdef";

    //printf("\nkey: %s  | text: %s\n", key, text);
    struct AES_ctx *ctx = AES_init(key, strlen((char*)key));

    size_t outlen = AES_encrypt(ctx, text, strlen((char*)text), &output);

    if (output == NULL)
        cr_assert_fail("output = NULL");

    //printf("encryption: %s\n\n", output);
    AES_ctx_free(ctx);

    cr_assert_neq(outlen, 0);
    cr_assert_not_null(output);
    cr_assert_str_not_empty((char*)output);
    cr_assert_str_neq((char*)output, (char*)text);
}


Test(AES, Decrypt)
{

    unsigned char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    unsigned char *output = NULL;
    decrypt = NULL;
    unsigned char key[] = "01G345a.89sbhdef";

    struct AES_ctx *ctx = AES_init(key, strlen((char*)key));
    size_t outlen = AES_encrypt(ctx, text, strlen((char*)text), &output);

    if (output == NULL)
        cr_assert_fail("output = NULL");

    size_t delen = AES_decrypt(ctx, output, outlen, &decrypt);

    free(output);
    AES_ctx_free(ctx);

    cr_assert_neq(delen, 0);
    cr_assert_not_null(decrypt);;
    cr_assert_str_not_empty((char*)decrypt);
    cr_assert_str_eq((char*)decrypt, (char*)text);
}

Test(AES, encrypt_decrypt_file)
{
    int e = AES_encrypt_file("example/a/1", "example/a/2", "testai");
    cr_assert_eq(e, 0);

    e = AES_decrypt_file("example/a/2", "example/a/3", "testai");
    cr_assert_eq(e, 0);

    FILE *reff = fopen("example/a/1", "r");
    FILE *decf = fopen("example/a/3", "r");
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove("example/a/2");
    remove("example/a/3");
}


// RSA
/*
Test(RSA, encrypt)
{
    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    size_t lentext = strlen(text);

    unsigned long keysize = 2048;
    struct RSA_pubKey *pubk;
    struct RSA_privKey *privk;

    RSA_generateKey(keysize, &privk, &pubk);

    size_t elen;
    unsigned char *encrypt = RSA_encode(pubk, (unsigned char*)text, lentext, &elen);
    cr_expect_not_null(encrypt);
    cr_expect_str_not_empty((char*)encrypt);

    free(encrypt);
    RSA_free_public_key(pubk);
    RSA_free_private_key(privk);
}

Test(RSA, decrypt, .disabled=1)
{
    char *text = "Lorem Ipsum is simply dummy ";//text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    size_t lentext = strlen(text);

    unsigned long keysize = 1024;


    struct RSA_pubKey *pubk;
    struct RSA_privKey *privk;

    RSA_generateKey(keysize, &privk, &pubk);


    size_t elen;
    unsigned char *encrypt = RSA_encode(pubk, (unsigned char*)text, lentext, &elen);
    cr_expect_not_null(encrypt);
    cr_expect_str_not_empty((char*)encrypt);

    size_t dlen;
    unsigned char *decode = RSA_decode(privk, encrypt, elen, &dlen);
    cr_expect_not_null(decode);
    cr_expect_str_not_empty((char*)decode);
    cr_expect_str_eq((char*)decode, (char*)text);


    free(encrypt);
    free(decode);

    RSA_free_public_key(pubk);
    RSA_free_private_key(privk);
}*/

Test(RSA, GenKeyFile)
{
    char *pbkf = "key.pub";
    char *prkf = "key.priv";

    struct RSA_pubKey *pubk;
    struct RSA_privKey *privk;
    unsigned long keysize = 64;
    RSA_generateKey(keysize, &privk, &pubk);

    RSA_pubk_to_file(pubk, pbkf);
    RSA_privk_to_file(privk, prkf);
    struct RSA_pubKey *pub = RSA_pubKey_from_file(pbkf);
    struct RSA_privKey *priv = RSA_privKey_from_file(prkf);

    cr_expect_eq(mpz_cmp(*(pubk->n), *(pub->n)), 0);
    cr_expect_eq(mpz_cmp(*(pubk->e), *(pub->e)), 0);
    cr_expect_eq(mpz_cmp(*(privk->n), *(priv->n)), 0);
    cr_expect_eq(mpz_cmp(*(privk->d), *(priv->d)), 0);

    RSA_free_public_key(pubk);
    RSA_free_private_key(privk);
    RSA_free_public_key(pub);
    RSA_free_private_key(priv);

    remove(pbkf);
    remove(prkf);
}

Test(RSA, EncryptDecryptFile)
{
    struct RSA_pubKey *pubk;
    struct RSA_privKey *privk;
    unsigned long keysize = 64;
    RSA_generateKey(keysize, &privk, &pubk);
    
    RSA_encode_file("example/b/5", "example/b/6", pubk);
    RSA_decode_file("example/b/6", "example/b/7", privk);
    
    FILE *reff = fopen("example/b/5", "r");
    FILE *decf = fopen("example/b/7", "r");
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove("example/b/6");
    remove("example/b/7");
    
    RSA_free_public_key(pubk);
    RSA_free_private_key(privk);
}

Test(RSA, EncryptDecryptFileStream)
{
    struct RSA_pubKey *pubk;
    struct RSA_privKey *privk;
    unsigned long keysize = 64;
    RSA_generateKey(keysize, &privk, &pubk);
    
    char *in = "example/d/6";
    char *enc = "example/d/7";
    char *dec = "example/d/8";

    FILE *fin = fopen(in, "r");
    FILE *fenc = fopen(enc, "w+");
    FILE *fdec = fopen(dec, "w+");

    RSA_encode_stream(fin, fenc, pubk);
    fclose(fenc);
    fenc = fopen(enc, "r+");
    RSA_decode_stream(fenc, fdec, privk);

    fclose(fin);
    fclose(fenc);
    fclose(fdec);

    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
    
    RSA_free_public_key(pubk);
    RSA_free_private_key(privk);
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
    char *vkey = (char*)rand_string(vkeylen);

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
    char *vkey = (char*)rand_string(vkeylen);

    VIGENERE_encrypt(data, vkey);

    VIGENERE_decrypt(data, vkey);

    free(vkey); vkey = NULL;

    cr_assert_not_null(data);
    cr_assert_str_not_empty(data);
    cr_assert_str_eq(data, text);

}

// ELGAMAL
Test(ELGAMAL, PublicKey)
{
    FILE *f = tmpfile();
    struct ELGAMAL_pubkey *pubk;
    struct ELGAMAL_privkey *privk;

    ELGAMAL_generateKey(256, &privk, &pubk);
    ELGAMAL_privkey_free(privk);
    
    cr_expect_eq(ELGAMAL_pubk_to_stream(pubk, f), EL_OK);
    fseek(f, 0, SEEK_SET);

    struct ELGAMAL_pubkey *r = malloc(sizeof(struct ELGAMAL_pubkey));
    cr_expect_eq(ELGAMAL_pubk_from_stream(r,  f), 0);
    cr_expect_not_null(r);

    cr_expect_eq(r->p, pubk->p);
    cr_expect_eq(r->e1, pubk->e1);
    cr_expect_eq(r->e2, pubk->e2);

    ELGAMAL_pubkey_free(pubk);
    ELGAMAL_pubkey_free(r);
    fclose(f);
}

Test(ELGAMAL, PrivateKey)
{
    FILE *f = tmpfile();
    struct ELGAMAL_pubkey *pubk;
    struct ELGAMAL_privkey *privk;

    ELGAMAL_generateKey(256, &privk, &pubk);
    ELGAMAL_pubkey_free(pubk);
    
    cr_expect_eq(ELGAMAL_privk_to_stream(privk, f), EL_OK);
    fseek(f, 0, SEEK_SET);

    struct ELGAMAL_privkey *r = malloc(sizeof(struct ELGAMAL_privkey));
    cr_expect_eq(ELGAMAL_privk_from_stream(r,  f), 0);
    cr_expect_not_null(r);

    cr_expect_eq(r->p, privk->p);
    cr_expect_eq(r->d, privk->d);

    ELGAMAL_privkey_free(r);
    ELGAMAL_privkey_free(privk);
    fclose(f);
}

Test(ELGAMAL, EncryptionDecryption)
{
    char *in = "example/c/3";
    char *enc = "example/c/4";
    char *dec = "example/c/5";

    struct ELGAMAL_pubkey *pubk;
    struct ELGAMAL_privkey *privk;

    ELGAMAL_generateKey(256, &privk, &pubk);

    EL_encryption_file(in, enc, pubk);

    EL_decryption_file(enc, dec, privk);

    ELGAMAL_pubkey_free(pubk);
    ELGAMAL_privkey_free(privk);
    
    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
}

// PACKUP ENCRYPTION
Test(PackupEncryption, Rotn)
{
    char *in = "example/e/rotn.txt";
    char *enc = "example/e/rotn.txt.enc";
    char *dec = "example/e/rotn.txt.dec";

    FILE *fin = fopen(in, "r");
    FILE *fenc = fopen(enc, "w+");
    cr_expect_not_null(fin);
    cr_expect_not_null(fenc);
    PACKUP_encryption_stream(ROTN, fin, fenc, 3);
    fclose(fin);
    fclose(fenc);
    fenc = fopen(enc, "r");
    FILE *fdec = fopen(dec, "w+");
    cr_expect_not_null(fenc);
    cr_expect_not_null(fdec);
    PACKUP_decryption_stream(ROTN, fenc, fdec, 3);
    fclose(fenc);
    fclose(fdec);

    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_not_null(reff);
    cr_expect_not_null(decf);
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
}

Test(PackupEncryption, Vigenere)
{
    char *in = "example/e/vig.txt";
    char *enc = "example/e/vig.txt.enc";
    char *dec = "example/e/vig.txt.dec";
    char *key = "Votai test.";

    FILE *fin = fopen(in, "r");
    FILE *fenc = fopen(enc, "w+");
    cr_expect_not_null(fin);
    cr_expect_not_null(fenc);
    cr_expect_eq(PACKUP_encryption_stream(VIGENERE, fin, fenc, key), 0);
    fclose(fin);
    fclose(fenc);
    fenc = fopen(enc, "r");
    FILE *fdec = fopen(dec, "w+");
    cr_expect_not_null(fenc);
    cr_expect_not_null(fdec);
    cr_expect_eq(PACKUP_decryption_stream(VIGENERE, fenc, fdec, key), 0);
    fclose(fdec);
    fclose(fenc);

    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_not_null(reff);
    cr_expect_not_null(decf);
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
}

Test(PackupEncryption, Aes)
{
    char *in = "example/e/aes.txt";
    char *enc = "example/e/aes.txt.enc";
    char *dec = "example/e/aes.txt.dec";
    char *key = "Votai test.";

    FILE *fin = fopen(in, "r");
    FILE *fenc = fopen(enc, "w+");
    cr_expect_not_null(fin);
    cr_expect_not_null(fenc);
    cr_expect_eq(PACKUP_encryption_stream(AES, fin, fenc, key), 0);
    fclose(fin);
    fclose(fenc);
    fenc = fopen(enc, "r");
    FILE *fdec = fopen(dec, "w+");
    cr_expect_not_null(fenc);
    cr_expect_not_null(fdec);
    cr_expect_eq(PACKUP_decryption_stream(AES, fenc, fdec, key), 0);
    fclose(fdec);
    fclose(fenc);
    
    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_not_null(reff);
    cr_expect_not_null(decf);
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
}

Test(PackupEncryption, RsaGenKey)
{
    char *in = "example/e/rsagenkey.txt";
    char *enc = "example/e/rsagenkey.txt.enc";
    char *dec = "example/e/rsagenkey.txt.dec";
    unsigned long keysize = 1024;
    char *pub = "example/e/rsagenkey.pub";
    char *priv = "example/e/rsagenkey.priv";

    FILE *fin = fopen(in, "r");
    FILE *fenc = fopen(enc, "w+");
    cr_expect_not_null(fin);
    cr_expect_not_null(fenc);
    cr_expect_eq(PACKUP_encryption_stream(RSA, fin, fenc, pub, priv, keysize), 0);
    fclose(fin);
    fclose(fenc);
    fenc = fopen(enc, "r");
    FILE *fdec = fopen(dec, "w+");
    cr_expect_not_null(fenc);
    cr_expect_not_null(fdec);
    cr_assert_eq(PACKUP_decryption_stream(RSA, fenc, fdec, priv), 0);
    fclose(fdec);
    fclose(fenc);

    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_not_null(reff);
    cr_expect_not_null(decf);
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
    remove(pub);
    remove(priv);
}

Test(PackupEncryption, RsaUseKey)
{
    unsigned long keysize = 4096;
    char *pub = "example/e/rsausekey.pub";
    char *priv = "example/e/rsausekey.priv";
    struct RSA_pubKey *pubk;
    struct RSA_privKey *privk;
    RSA_generateKey(keysize, &privk, &pubk);
    RSA_pubk_to_file(pubk, pub);
    RSA_privk_to_file(privk, priv);
    RSA_free_public_key(pubk);
    RSA_free_private_key(privk);
    
    char *in = "example/e/rsausekey.txt";
    char *enc = "example/e/rsausekey.txt.enc";
    char *dec = "example/e/rsausekey.txt.dec";

    FILE *fin = fopen(in, "r");
    FILE *fenc = fopen(enc, "w+");
    cr_expect_not_null(fin);
    cr_expect_not_null(fenc);
    cr_expect_eq(PACKUP_encryption_stream(RSA, fin, fenc, pub), 0);
    fclose(fin);
    fclose(fenc);
    fenc = fopen(enc, "r");
    FILE *fdec = fopen(dec, "w+");
    cr_expect_not_null(fenc);
    cr_expect_not_null(fdec);
    cr_expect_eq(PACKUP_decryption_stream(RSA, fenc, fdec, priv), 0);
    fclose(fdec);
    fclose(fenc);

    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_not_null(reff);
    cr_expect_not_null(decf);
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
    remove(pub);
    remove(priv);
}

Test(PackupEncryption, ElgamalGenKey)
{
    char *in = "example/e/elgenkey.txt";
    char *enc = "example/e/elgenkey.txt.enc";
    char *dec = "example/e/elgenkey.txt.dec";
    unsigned long keysize = 1024;
    char *pub = "example/e/elgenkey.pub";
    char *priv = "example/e/elgenkey.priv";

    FILE *fin = fopen(in, "r");
    FILE *fenc = fopen(enc, "w+");
    cr_expect_not_null(fin);
    cr_expect_not_null(fenc);
    cr_expect_eq(PACKUP_encryption_stream(ELGAMAL, fin, fenc, pub, priv, keysize), 0);
    fclose(fin);
    fclose(fenc);
    fenc = fopen(enc, "r");
    FILE *fdec = fopen(dec, "w+");
    cr_expect_not_null(fenc);
    cr_expect_not_null(fdec);
    cr_expect_eq(PACKUP_decryption_stream(ELGAMAL, fenc, fdec, priv), 0);
    fclose(fdec);
    fclose(fenc);

    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_not_null(reff);
    cr_expect_not_null(decf);
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
    remove(pub);
    remove(priv);
}

Test(PackupEncryption, ElgamalUseKey)
{
    char *in = "example/e/elusekey.txt";
    char *enc = "example/e/elusekey.txt.enc";
    char *dec = "example/e/elusekey.txt.dec";
    unsigned long keysize = 1024;
    char *pub = "example/e/elusekey.pub";
    char *priv = "example/e/elusekey.priv";

    struct ELGAMAL_pubkey *pubk;
    struct ELGAMAL_privkey *privk;

    ELGAMAL_generateKey(keysize, &privk, &pubk);
    ELGAMAL_privk_to_file(privk, priv);
    ELGAMAL_pubk_to_file(pubk, pub);

    ELGAMAL_privkey_free(privk);
    ELGAMAL_pubkey_free(pubk);


    FILE *fin = fopen(in, "r");
    FILE *fenc = fopen(enc, "w+");
    cr_expect_not_null(fin);
    cr_expect_not_null(fenc);
    cr_expect_eq(PACKUP_encryption_stream(ELGAMAL, fin, fenc, pub), 0);
    fclose(fin);
    fclose(fenc);
    fenc = fopen(enc, "r");
    FILE *fdec = fopen(dec, "w+");
    cr_expect_not_null(fenc);
    cr_expect_not_null(fdec);
    cr_expect_eq(PACKUP_decryption_stream(ELGAMAL, fenc, fdec, priv), 0);
    fclose(fdec);
    fclose(fenc);

    FILE *reff = fopen(in, "r");
    FILE *decf = fopen(dec, "r");
    cr_expect_not_null(reff);
    cr_expect_not_null(decf);
    cr_expect_file_contents_eq(decf, reff); 
    fclose(reff);
    fclose(decf);

    remove(enc);
    remove(dec);
    remove(pub);
    remove(priv);
}

