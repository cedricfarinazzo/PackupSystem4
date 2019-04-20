## AES-128
---

An implementation of AES 128 ECB in C

### How to use ?

Include the aes header
```c
#include "aes.h"
```

First initialize a context with a key by calling the following function
```c
/* AES_init: initialize aes with a key and return the context
 * key: unsigned char*: a 16 bit key
 * len: size_t: lenght of key
 * return: struct AES_ctx*: aes context
 */
struct AES_ctx *AES_init(unsigned char *key, size_t len);
```

Then you can 
- encrypt some data
```c
/* AES_encrypt: encrypt data with the context ctx and write the output on encrypt
 * ctx: struct AES_ctx*: an aes context initilize by AES_init
 * data: char*: data to encrypt
 * lendata: size_t: lenght of data
 * encrypt: char**: a pointer on char*. data will be malloc and write on *encrypt
 * return: size_t: lenght of encrypt
 */
size_t AES_encrypt(struct AES_ctx *ctx, unsigned char *data, size_t lendata, unsigned char **encrypt);
```

- decrypt 
```c
/* AES_decrypt: decrypt encrypted data with the context ctx and write the output on decrypt
 * ctx: struct AES_ctx*: an aes context initilize by AES_init
 * encrypt: char*: encrypt to decrypt
 * lendata: size_t: lenght of encrypt
 * decrypt: char**: a pointer on char*. data will be malloc and write on *decrypt
 * return: size_t: lenght of decrypt
 */
size_t AES_decrypt(struct AES_ctx *ctx, unsigned char *encrypt, size_t lendata, unsigned char **decrypt);
```

Don't forget to free the context by calling the following function
```c
/* AES_ctx_free: free an aes context
 * ctx: struct AES_ctx*: an aes context initilize by AES_init
 * return: void
 */
void AES_ctx_free(struct AES_ctx *ctx);
```

### The key

The key is only a 16 byte char*. So you can give a char*.

Or you can use include hashpass.h
```c
#include "hashpass.h"
```

and use the following function to generate a 16 byte char\* from a n-byte char (ex: a password)
```c
/* AES_keyFromPass: generate an aes key from password
 * pass: char*: a password
 * len: size_t: lenght of pass
 * return: unsigned char*: an aes key of 16 byte ended by null character
 */
unsigned char *AES_keyFromPass(char *pass, size_t len);
```

# AES on file

```c
#include "aes_file.h"
```

```c
/* AES_encrypt_fd: encrypt data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: char*: a password
 * return: int: 0 on success, -1 on failure
 */
int AES_encrypt_file(int fin, int fout, char *pass);
```

```c
/* AES_decrypt_fd: decrypt data from fin and write on fout
 * fin: int: file descriptor pointing to the input file
 * fout: int: file descriptor pointing to the output file
 * pass: char*: a password
 * return: int: 0 on success, -1 on failure
 */
int AES_decrypt_file(int fin, int fout, char *pass);
```

```c
/* AES_encrypt_file: encrypt data from in and write on out
 * in: char*: path to the input file
 * out: char*: path to the output file
 * pass: char*: a password
 * return: int: AES code (check aes_file.h)
 */
int AES_encrypt_file(char *in, char *out, char *pass);
```

```c
/* AES_decrypt_fd: decrypt data from in and write on out
 * fin: char*: path to the input file
 * fout: char*: path to the output file
 * pass: char*: a password
 * return: int: AES code (check aes_file.h)
 */
int AES_decrypt_file(char *in, char *out, char *pass);
```

### Example

```c
#include "aes.h"
#include "hashpass.h"

#include "err.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
        errx(1, "aes example: usage: <text> <password>");

    unsigned char *text = (unsigned char*)argv[1];
    char *pass = argv[2];
    
    unsigned char *key = AES_keyFromPass(pass, strlen(pass)); // Generate aes key
    
    
    struct AES_ctx *ctx = AES_init(key, strlen((char*)key)); // Initialize context
    
    unsigned char *encrypt = NULL;
    size_t outlen = AES_encrypt(ctx, text, strlen((char*)text), &encrypt); // encrypt data with ctx. write outlen byte on encrypt

    unsigned char *decrypt = NULL;
    size_t delen = AES_decrypt(ctx, encrypt, outlen, &decrypt); //decrypt encrypt with ctx. write delen byte on decrypt


    free(encrypt); //free encrypted data memory
    free(decrypt); //free decrypted data memory
    AES_ctx_free(ctx); //free aes ctx
    free(key);

    return 0;
}
```
