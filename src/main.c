#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "encryption/rsa/rsa.h"
#include "encryption/rsa/genkey.h"

int main()
{
    mpz_t p; 
    mpz_init_set_ui(p, 1237);
    
    mpz_t q; 
    mpz_init_set_ui(q, 2003);

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
    
    //DO OTHER SHIT

    RSA_free_public_key(pub);
    RSA_free_private_key(pri);
    mpz_clear(p);
    mpz_clear(q);

    /*
    ulong p = 11;//1237;
    ulong q = 13;//2003;
    ulong *public = RSA_gen_public_key(p, q);
    ulong *private = RSA_gen_private_key(p, q, public);

    char text[] = "Hello World!";
    size_t len = strlen(text);
    printf("text: %s\n", text);

    printf("public: (n: %lld)   (e: %lld)\n", public[1], public[0]);
    printf("private: (n: %lld)   (d: %lld)\n", private[1], private[0]);

    ulong *encode = RSA_encode(public, text, len);


    for (size_t i = 0; text[i] != '\0'; ++i)
        printf("%lld  ", encode[i]);

    unsigned char *decode = RSA_decode(private, encode, len);

    printf("\n\n");
    for (size_t i = 0; text[i] != '\0'; ++i)
        printf("%d (%c) \n", decode[i], decode[i]);
    
    //printf("\n%s\n\n", decode);
    
    free(decode);
    free(encode);

    free(public);
    free(private);

    */
    printf("\nHello World!\n");
    return EXIT_SUCCESS;
}
