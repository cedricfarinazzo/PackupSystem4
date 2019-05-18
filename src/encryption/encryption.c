#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>

#include "rotn.h"
#include "tea.h"
#include "vigenere.h"
#include "rsa/rsa.h"
#include "rsa/rsa_file.h"
#include "rsa/genkey.h"
#include "aes/aes.h"
#include "aes/aes_file.h"
#include "aes/hashpass.h"
#include "elgamal/elgamal.h"

#include "encryption.h"


int file_exist(char *fname)
{
    return access(fname, F_OK) != -1;
}

int PACKUP_encryption_stream(enum ENCRYPTION_TYPE type, FILE *in, FILE *out, ...)
{
    if (type == NONE)
        return 0;
    if (in == NULL || out == NULL) return -1;
    va_list ap;
    va_start(ap, out);

    int r = 0;
    int rotn_key = 0;

    char *vig_key = NULL;

    struct RSA_pubKey *RSA_pubk = NULL;
    char *RSA_pubk_file = NULL;

    char *AES_pass = NULL;

    struct ELGAMAL_pubkey *EL_pubk = NULL;
    char *EL_pubk_file = NULL;

    switch (type)
    {
        case ROTN:
            rotn_key = va_arg(ap, int);
            break;

        case VIGENERE:
            vig_key = va_arg(ap, char*);
            if (vig_key == NULL) return -1;
            break;

        case RSA:
            RSA_pubk_file = va_arg(ap, char*);
            if (RSA_pubk_file == NULL) return -1;
            if (!file_exist(RSA_pubk_file)) {
                struct RSA_privKey *RSA_privk = NULL;
                char *RSA_privk_file = NULL;
                RSA_privk_file = va_arg(ap, char*);
                if (RSA_privk_file == NULL) return -1;
                unsigned long keysize = va_arg(ap, unsigned long);
                if (keysize == 0) return -1;
                RSA_generateKey(keysize, &RSA_privk, &RSA_pubk);
                RSA_pubk_to_file(RSA_pubk, RSA_pubk_file);
                RSA_privk_to_file(RSA_privk, RSA_privk_file);
                RSA_free_private_key(RSA_privk);
            } else {
                RSA_pubk = RSA_pubKey_from_file(RSA_pubk_file);
                if (RSA_pubk == NULL) return -1;
            }
            break;

        case AES:
            AES_pass = va_arg(ap, char*);
            if (AES_pass == NULL) return -1;
            break;

        case ELGAMAL:
            EL_pubk_file = va_arg(ap, char*);
            if (EL_pubk_file == NULL) return -1;
            if (!file_exist(EL_pubk_file)) {
                struct ELGAMAL_privkey *EL_privk = NULL;
                char *EL_privk_file = NULL;
                EL_privk_file = va_arg(ap, char*);
                if (EL_privk_file == NULL) return -1;
                unsigned long keysize = va_arg(ap, unsigned long);
                if (keysize == 0) return -1;
                ELGAMAL_generateKey(keysize, &EL_privk, &EL_pubk);
                ELGAMAL_pubk_to_file(EL_pubk, EL_pubk_file);
                ELGAMAL_privk_to_file(EL_privk, EL_privk_file);
                ELGAMAL_privkey_free(EL_privk);
            } else {
                EL_pubk = ELGAMAL_pubKey_from_file(EL_pubk_file);
                if (EL_pubk == NULL) return -1;
            }
            break;

        default:
            return r;
    }
    va_end(ap);

    switch (type)
    {
        case ROTN:
            r = ROTN_encrypt_stream(in, out, rotn_key);
            break;

        case VIGENERE:
            r = VIGENERE_encrypt_stream(in, out, vig_key);
            break;

        case RSA:
            r = RSA_encode_stream(in, out, RSA_pubk);
            break;

        case AES:
            r = AES_encrypt_stream(in, out, AES_pass);
            break;

        case ELGAMAL:
            r = EL_encryption_stream(in, out, EL_pubk);
            break;

        default:
            return r;
    }

    switch (type)
    {
        case RSA:
            RSA_free_public_key(RSA_pubk);
            break;

        case ELGAMAL:
            ELGAMAL_pubkey_free(EL_pubk);
            break;

        default:
            break;
    }

    return r;
}



int PACKUP_decryption_stream(enum ENCRYPTION_TYPE type, FILE *in, FILE *out, ...)
{
    if (type == NONE)
        return 0;
    if (in == NULL || out == NULL) return -1;
    va_list ap;
    va_start(ap, out);

    int r = 0;
    int rotn_key = 0;

    char *vig_key = NULL;

    struct RSA_privKey *RSA_privk = NULL;
    char *RSA_privk_file = NULL;

    char *AES_pass = NULL;

    struct ELGAMAL_privkey *EL_privk = NULL;
    char *EL_privk_file = NULL;

    switch (type)
    {
        case ROTN:
            rotn_key = va_arg(ap, int);
            break;

        case VIGENERE:
            vig_key = va_arg(ap, char*);
            if (vig_key == NULL) return -1;
            break;

        case RSA:
            RSA_privk_file = va_arg(ap, char*);
            if (RSA_privk_file == NULL) return -1;
            if (!file_exist(RSA_privk_file)) {
                return -1;
            } else {
                RSA_privk = RSA_privKey_from_file(RSA_privk_file);
                if (RSA_privk == NULL) return -1;
            }
            break;

        case AES:
            AES_pass = va_arg(ap, char*);
            if (AES_pass == NULL) return -1;
            break;

        case ELGAMAL:
            EL_privk_file = va_arg(ap, char*);
            if (EL_privk_file == NULL) return -1;
            if (!file_exist(EL_privk_file)) {
                return -1;
            } else {
                EL_privk = ELGAMAL_privKey_from_file(EL_privk_file);
                if (EL_privk == NULL) return -1;
            }
            break;

        default:
            return r;
    }
    va_end(ap);

    switch (type)
    {
        case ROTN:
            r = ROTN_decrypt_stream(in, out, rotn_key);
            break;

        case VIGENERE:
            r = VIGENERE_decrypt_stream(in, out, vig_key);
            break;

        case RSA:
            r = RSA_decode_stream(in, out, RSA_privk);
            break;

        case AES:
            r = AES_decrypt_stream(in, out, AES_pass);
            break;

        case ELGAMAL:
            r = EL_decryption_stream(in, out, EL_privk);
            break;

        default:
            return r;
    }

    switch (type)
    {
        case RSA:
            RSA_free_private_key(RSA_privk);
            break;

        case ELGAMAL:
            ELGAMAL_privkey_free(EL_privk);
            break;

        default:
            break;
    }

    return r;
}
