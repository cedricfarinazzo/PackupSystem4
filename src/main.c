#include <stdio.h>
#include <stdlib.h>

#include "../src/encryption/aes/aes.h"
#include "../src/encryption/aes/aes_matrix.h"
#include "../src/encryption/aes/aes_addroundkey.h"
#include "../src/encryption/aes/aes_shiftrows.h"
#include "../src/encryption/aes/aes_subbytes.h"
#include "../src/encryption/aes/aes_mixcolumns.h"




int main()
{
    uint8_t data1[16] = 
    {
        0x00, 0x01, 0x02, 0x03, 
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F
    };
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_feed(mat, data1);
    AES_matrix_printfhex(mat);

    struct AES_matrix *subb = AES_matrix_subBytes(mat);
    AES_matrix_printfhex(subb);
    AES_matrix_free(subb);

    struct AES_matrix *shi = AES_matrix_shiftRows(mat);
    AES_matrix_printfhex(shi);
    AES_matrix_free(shi);
    
    struct AES_matrix *mi = AES_matrix_mixColumns(mat);
    AES_matrix_printfhex(mi);
    AES_matrix_free(mi);
    
    AES_matrix_free(mat);
    return EXIT_SUCCESS;
}
