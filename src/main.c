#include <stdio.h>
#include <stdlib.h>

#include "../src/encryption/aes/aes.h"
#include "../src/encryption/aes/aes_matrix.h"
#include "../src/encryption/aes/aes_addroundkey.h"
#include "../src/encryption/aes/aes_shiftrows.h"
#include "../src/encryption/aes/aes_subbytes.h"
#include "../src/encryption/aes/aes_mixcolumns.h"
#include "../src/encryption/aes/aes_keyexpansion.h"

int main()
{
    /*
    uint8_t data1[16] = 
    {
        0x04, 0xe0, 0x48, 0x28, 
        0x66, 0xcb, 0xf8, 0x06,
        0x81, 0x19, 0xd3, 0x26,
        0xe5, 0x9a, 0x7a, 0x4c
    };
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_feed(mat, data1);
    printf("\n== Mat \n");
    AES_matrix_printfhex(mat);

    uint8_t key[16] = 
    {
        0xa0, 0x88, 0x23, 0x2a, 
        0xfa, 0x54, 0xa3, 0x6c,
        0xfe, 0x2c, 0x39, 0x76,
        0x17, 0xb1, 0x39, 0x05
    };
    struct AES_matrix *mkey = AES_matrix_init();
    AES_matrix_feed(mkey, key);
    printf("\n== Key \n");
    AES_matrix_printfhex(mkey);


    struct AES_matrix *subb = AES_matrix_subBytes(mat);
    printf("\n== SUBBYTES \n");
    AES_matrix_printfhex(subb);
    AES_matrix_free(subb);

    struct AES_matrix *shi = AES_matrix_shiftRows(mat);
    printf("\n== SHIFT ROWS \n");
    AES_matrix_printfhex(shi);
    AES_matrix_free(shi);
    
    struct AES_matrix *mi = AES_matrix_mixColumns(mat);
    printf("\n== MIX COLUMNS \n");
    AES_matrix_printfhex(mi);
    AES_matrix_free(mi);
   
    struct AES_matrix *ro = AES_matrix_addRoundKey(mat, mkey);
    printf("\n== ADD ROUND KEY \n");
    AES_matrix_printfhex(ro);
    AES_matrix_free(ro);

    AES_matrix_free(mat);
    AES_matrix_free(mkey);
  */  
    printf("\n\n\n============= key expansion \n\n");

    uint8_t key1[16] = 
    {
        0x2b, 0x28, 0xab, 0x09, 
        0x7e, 0xae, 0xf7, 0xcf,
        0x15, 0xd2, 0x15, 0x4f,
        0x16, 0xa6, 0x88, 0x3c
    };
    struct AES_matrix *mkey1 = AES_matrix_init();
    AES_matrix_feed(mkey1, key1);
    printf("\n== Key \n");
    AES_matrix_printfhex(mkey1);
    printf("\n======================\n");

    struct AES_matrix **rb = AES_keyExpansion(mkey1);
     
    for (size_t i = 0; i < (AES_NB_ROUND_KEY + 1); ++i)
        AES_matrix_printfhex(rb[i]);

    AES_keyExpansion_free(rb);
    AES_matrix_free(mkey1);
  
  /* 
    printf("====== MIX COLUMS ONCE\n");

    uint8_t data2[16] = 
    {
        0xd4, 0x00, 0x00, 0x00, 
        0xbf, 0x00, 0x00, 0x00,
        0x81, 0x00, 0x00, 0x00,
        0xe5, 0x00, 0x00, 0x00
    };
    struct AES_matrix *mat1 = AES_matrix_init();
    AES_matrix_feed(mat1, data2);
    printf("\n== Mat \n");
    AES_matrix_printfhex(mat1);

    struct AES_matrix *mi1 = AES_matrix_mixColumns(mat1);
    AES_matrix_printfhex(mi1);
    struct AES_matrix *mi2 = AES_matrix_InvMixColumns(mi1);
    AES_matrix_printfhex(mi2);
    AES_matrix_free(mi1);
    AES_matrix_free(mi2);

    AES_matrix_free(mat1);
*/
    return EXIT_SUCCESS;
}
