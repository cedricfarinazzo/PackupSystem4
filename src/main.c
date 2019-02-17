#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryption/rsa/rsa.h"

int main()
{
    ulong p = 1237;
    ulong q = 2003;
    ulong *public = RSA_gen_public_key(p, q);
    ulong *private = RSA_gen_private_key(p, q, public);

    char text[] = "Hello World!";
    size_t len = strlen(text);
    printf("text: %s\n", text);

    printf("public: (n: %ld)   (e: %ld)\n", public[1], public[0]);
    printf("private: (n: %ld)   (d: %ld)\n", private[1], private[0]);

    ulong *encode = RSA_encode(public, text, len);


    for (size_t i = 0; text[i] != '\0'; ++i)
        printf("%ld  ", encode[i]);

    char *decode = RSA_decode(private, encode, len);

    printf("\n\n");
    for (size_t i = 0; text[i] != '\0'; ++i)
        printf("%d  ", decode[i]);
    
    printf("\n%s\n\n", decode);
    
    free(decode);
    free(encode);

    free(public);
    free(private);

    printf("Hello World!");
    return EXIT_SUCCESS;
}
