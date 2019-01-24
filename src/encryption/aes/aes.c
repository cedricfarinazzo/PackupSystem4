#include <stdlib.h>

#include "aes.h"

size_t AES_encrypt(char *data, size_t data_size, char *key, size_t key_size, char **encrypt)
{
    *encrypt = malloc(sizeof(char)*data_size);

    //do something

    return data_size; 
}

size_t AES_decrypt(char *encrypt, size_t encrypt_size, char *key, size_t key_size, char **decrypt)
{
    *decrypt = malloc(sizeof(char)*encrypt_size);

    //do something
    
    return encrypt_size;
}

