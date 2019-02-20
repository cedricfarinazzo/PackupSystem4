#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gui/interface.h"

#include "encryption/rotn.h"
#include "encryption/vigenere.h"

#include "encryption/rsa/rsa.h"
#include "encryption/rsa/genkey.h"

#include "encryption/aes/aes.h"

void print_ascii(char *a)
{
    for (size_t i = 0; a[i] != 0; ++i)
        printf("%d ", a[i]);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (argc == 1)
        return interface(argc, argv);

    if (argc == 2)
    {
    }

    if (argc == 3)
    {}

    if (argc == 4)
    {
        if (strcmp("rotn", argv[1]) == 0)
        {
            char *text = argv[2];
            long key = atol(argv[3]);

            printf("text: %s   | key: %ld\n", text, key);

            ROTN_encrypt(text, key);
            printf("encrypted text: %s | ", text);
            print_ascii(text); printf("\n");

            ROTN_decrypt(text, key);
            printf("decrypted text: %s\n", text);
        }

        if (strcmp("vigenere", argv[1]) == 0)
        {
            char *text = argv[2];
            char *key = argv[3];

            printf("text: %s   | key: %s\n", text, key);

            VIGENERE_encrypt(text, key);
            printf("encrypted text: %s | ", text);
            print_ascii(text); printf("\n");

            VIGENERE_decrypt(text, key);
            printf("decrypted text: %s\n", text);
        }
    }

    if (argc == 5)
    {
        if (strcmp("rsa", argv[1]) == 0)
        {
            char *text = argv[2];
            size_t lentext = strlen(text);
            long lp = atol(argv[3]);
            long lq = atol(argv[4]);

            mpz_t p, q;
            mpz_init(p); mpz_init(q);
            mpz_set_ui(p, lp);
            mpz_set_ui(q, lq);

            printf("text: %s (size: %ld\np: %ld     | q: %ld\n", text, lentext, lp, lq);

            struct RSA_publickey *pub = RSA_gen_public_key(p, q);
            struct RSA_privatekey *pri = RSA_gen_private_key(p, q, pub);

            printf("public: n = ");
            mpz_out_str(stdout,10, *(pub->n));
            printf("   e = ");
            mpz_out_str(stdout, 10, *(pub->e));

            printf("\nprivate: n = ");
            mpz_out_str(stdout, 10, *(pri->n));
            printf("   d = ");
            mpz_out_str(stdout, 10, *(pri->d));
            printf("\n");


            mpz_t *encrypt = RSA_encode(pub, (unsigned char*)text, lentext);
            printf("\nencode data: ");
            for (size_t i = 0; i < lentext; ++i)
                gmp_printf("%A ", encrypt[i]);


            unsigned char *decode = RSA_decode(pri, encrypt, lentext);
            
            printf("\n\ntext: decode: %s", decode);

            for (size_t i = 0; i < lentext; ++i)
                mpz_clear(encrypt[i]);
            free(encrypt);

            RSA_free_public_key(pub);
            RSA_free_private_key(pri);
            mpz_clear(p);
            mpz_clear(q);
            free(decode);

        }
    }


    return EXIT_SUCCESS;
}
