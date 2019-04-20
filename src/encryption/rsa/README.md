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
 * rlen: size_t: lenght of output (mpz_t *)
 * return: mpz_t*: an array of mpz_t (lenght = rlen). it's the encrypted message
 */
unsigned char *RSA_encode(struct RSA_pubKey *public, unsigned char *data, size_t len, size_t *rlen);
```

- decrypt 
```c
/* RSA_decode: decrypt a char* with a public key
 * private: struct RSA_privKey*: a private key generated RSA_generateKey
 * data: mpz_t*: message to decrypt
 * len: size_t: lenght of data
 * return: unsigned char*:(lenght = len). it's the decrypted message
 */
unsigned char *RSA_decode(struct RSA_privKey *private, unsigned char *data, size_t len, size_t *rlen);
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

### RSA on file

```c
#include "rsa_file.h"
#include "genkey.h"
```

```c
/* RSA_pubk_to_file: write the RSA public to file
 * pub: struct RSA_pubKey*: RSA public key
 * path: int: path to the output file
 * return: int: RSA code (check tools.h)
 */
int RSA_pubk_to_file(struct RSA_pubKey *pub, char *path);
```

```c
/* RSA_privk_to_file: write the RSA private to file
 * pub: struct RSA_privKey*: RSA private key
 * path: int: path to the output file
 * return: int: RSA code (check tools.h)
 */
int RSA_privk_to_file(struct RSA_privKey *priv, char *path);
```

```c
/* RSA_pubk_from_file: load a RSA public from file
 * path: int: path to file
 * return: struct RSA_pubKey*: RSA public key
 */
struct RSA_pubKey *RSA_pubKey_from_file(char *path);
```

```c
/* RSA_privKey_from_file: load a RSA private from file
 * path: int: path to file
 * return: struct RSA_privKey*: RSA private key
 */
struct RSA_privKey *RSA_privKey_from_file(char *path);
```

```c
/* RSA_encode_fd: encode data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: struct RSA_pubKey*: RSA public key
 * return: int: RSA code (check tools.h)
 */
int RSA_encode_fd(int fin, int fout, struct RSA_pubKey *pubk);
```

```c
/* RSA_decode_fd: decode data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: struct RSA_privKey*: RSA private key
 * return: int: RSA code (check tools.h)
 */
int RSA_decode_fd(int fin, int fout, struct RSA_privKey *privk);
```

```c
/* RSA_encode_file: encode data from in file and write on out file
 * fin: char*: path to the input file
 * fout: int: path to the output file
 * pass: struct RSA_pubKey*: RSA public key
 * return: int: RSA code (check tools.h)
 */
int RSA_encode_file(char *in, char *out, struct RSA_pubKey *pubk);
```

```c
/* RSA_decode_file: decode data from in file and write on out file
 * fin: char*: path to the input file
 * fout: int: path to the output file
 * pass: struct RSA_privKey*: RSA private key
 * return: int: RSA code (check tools.h)
 */
int RSA_decode_file(char *in, char *out, struct RSA_privKey *privk);
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

    size_t elen;
    unsigned char *encrypt = RSA_encode(pubk, (unsigned char*)text, lentext, &elen);
    printf("\nencode (%ld): %s\n", elen, encrypt);

    size_t dlen;
    unsigned char *decode = RSA_decode(privk, encrypt, elen, &dlen);
   
    printf("\n\ndecode text (%ld): %s\n", dlen, decode);

    free(encrypt);
    free(decode);
    RSA_free_public_key(pubk);
    RSA_free_private_key(privk);

    return 0;
}
```

