## RSA
---

An implementation of RSA in C

### How to use ?

Include headers
```c
#include "rsa.h"
#include "genkey.h"
```

First you have to generate a private key and a public key
```c
/* RSA_generateKey: Generate private and public key
 * keysize: unsigned long: the number of bit of the key. must be a power of 2 (ex: 1024, 2048 or 4096)
 * privk: struct RSA_privKey*: a pointer on a struct RSA_privKey
 * pubk: struct RSA_pubKey*: a pointer on a struct RSA_pubKey
 * return: void
 */
void RSA_generateKey(unsigned long keysize, struct RSA_privKey **privk, struct RSA_pubKey **pubk);
```

Then you can 
- encrypt some data
```c
/* RSA_encode: encrypt a char* with a public key
 * public: struct RSA_pubKey*: a public key generated RSA_generateKey
 * data: unsigned char*: message to encrypt
 * len: size_t: lenght of data
 * return: mpz_t*: an array of mpz_t (lenght = len). it's the encrypted message
 */
mpz_t *RSA_encode(struct RSA_pubKey *public, unsigned char *data, size_t len);
```

- decrypt 
```c
/* RSA_decode: decrypt a char* with a public key
 * private: struct RSA_privKey*: a private key generated RSA_generateKey
 * data: mpz_t*: message to decrypt
 * len: size_t: lenght of data
 * return: unsigned char*:(lenght = len). it's the decrypted message
 */
unsigned char *RSA_decode(struct RSA_privKey *private, mpz_t *data, size_t len);
```

Don't forget to free all key: 
```c
/* RSA_free_private_key: free the private key
 * privk: struct RSA_privKey*: a private key generated RSA_generateKey
 * return: void
 */
void RSA_free_private_key(struct RSA_privKey *privk);


/* RSA_free_public_key: free the public key
 * pubk: struct RSA_pubKey*: a public key generated RSA_generateKey
 * return: void
 */
void RSA_free_public_key(struct RSA_pubKey *pubk);
```


### Example

```c
#include "rsa.h"
#include "genkey.h"

#include "err.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
        errx(1, "rsa example: usage: <text> <keysize>");

    char *text = argv[2];
    size_t lentext = strlen(text);

    unsigned long keysize = (unsigned long)atol(argv[3]);
            
    printf("text: %s (size: %ld) | keysize: %ld\n\n", text, lentext, keysize);

    struct RSA_pubKey *pubk;
    struct RSA_privKey *privk;

    RSA_generateKey(keysize, &privk, &pubk);

    printf("public: n = "); mpz_out_str(stdout,10, *(pubk->n));
    printf("   e = "); mpz_out_str(stdout, 10, *(pubk->e));

    printf("\nprivate: n = "); mpz_out_str(stdout, 10, *(privk->n));
    printf("   d = "); mpz_out_str(stdout, 10, *(privk->d)); printf("\n");

    mpz_t *encrypt = RSA_encode(pubk, (unsigned char*)text, lentext);
    printf("\nencode data: ");
    for (size_t i = 0; i < lentext; ++i)
        gmp_printf("%#Zx ", encrypt[i]);

    unsigned char *decode = RSA_decode(privk, encrypt, lentext);

    printf("\n\ndecode text: %s\n", decode);

    for (size_t i = 0; i < lentext; ++i)
        mpz_clear(encrypt[i]);
    free(encrypt);
    free(decode);
    RSA_free_public_key(pubk);
    RSA_free_private_key(privk);

    return 0;
}
```

