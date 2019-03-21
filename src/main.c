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
#include "encryption/aes/aes.h"
#include "encryption/aes/aes_matrix.h"
#include "encryption/aes/aes_addroundkey.h"
#include "encryption/aes/aes_shiftrows.h"
#include "encryption/aes/aes_subbytes.h"
#include "encryption/aes/aes_mixcolumns.h"

#include "compression/huffman.h"

#include "filesystem/build_metatree.h"
#include "filesystem/save_metatree.h"

#define VERSION "1.0"
#define DATE "2018-02"
#define TYPE "release"

// TO MOVE IN FILESYSTEM
/*
void print_tree(struct meta_tree *tree, int indent)
{
    char indents[indent + 1];
    for (int i = 0; i < indent; ++i)
        indents[i] = ' ';
    indents[indent] = 0;

    if (tree != NULL)
    {
        printf("%spath:%s | ", indents, tree->data->path);
        printf("size:%ld o | ",  tree->data->fs.st_size);
        printf("mode: %d | ", tree->data->fs.st_mode);
        printf("\n");

        struct meta_tree *c = tree->son;
        while (c)
        {
            print_tree(c, indent + 6);
            c = c->sibling;
        }
    }
}
*/

void print_ascii(unsigned char *a)
{
    for (size_t i = 0; a[i] != 0; ++i)
        printf("%d ", a[i]);
}

int main(int argc, char *argv[])
{
   

    unsigned char *text = (unsigned char*)argv[1];
    unsigned char *key = (unsigned char*)argv[2];
 
    printf("text: %s   | key: %s\n", text, key);

    unsigned char *output = NULL;
    //unsigned char *decrypt = NULL;

    AES_encrypt(text, key, &output);
    printf("encrypted text: %s  |   ", output);
    print_ascii(output); printf("\n");

    //AES_decrypt(output, key, &decrypt);
    //printf("encrypted text: %s  |   ", decrypt);
    //print_ascii(decrypt); printf("\n");

    free(output);
    //free(decrypt);

    return EXIT_SUCCESS;
    /*
    uint8_t data[16] = 
    {
        0x04, 0xe0, 0x48, 0x28,
        0x66, 0xcb, 0xf8, 0x06,
        0x81, 0x19, 0xd3, 0x26,
        0xe5, 0x9a, 0x7a, 0x4c,
    };
    uint8_t key[16] = 
    {
        0xa0, 0x88, 0x23, 0x2a,
        0xfa, 0x54, 0xa3, 0x6c,
        0xfe, 0x2c, 0x39, 0x76,
        0x17, 0xb1, 0x39, 0x05,
    };
    uint8_t exp[16] = 
    {
        0xa4, 0x68, 0x6b, 0x02,
        0x9c, 0x9f, 0x5b, 0x6a,
        0x7f, 0x35, 0xea, 0x50,
        0xf2, 0x2b, 0x43, 0x49,
    };
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_feed(mat, data);

    struct AES_matrix *expm = AES_matrix_init();
    AES_matrix_feed(expm, exp);
    
    struct AES_matrix *keym = AES_matrix_init();
    AES_matrix_feed(keym, key);
    
    struct AES_matrix *r = AES_matrix_addRoundKey(mat, keym);
    
    printf("%d", AES_matrix_areEqual(r, expm));

    AES_matrix_free(r);
   
    AES_matrix_free(mat);
    AES_matrix_free(keym);
    AES_matrix_free(expm);

    return EXIT_SUCCESS;
    uint8_t data[16] = 
    {
        0xdb, 0xf2, 0x01, 0x2d,
        0x13, 0x0a, 0x01, 0x26,
        0x53, 0x22, 0x01, 0x31,
        0x45, 0x5c, 0x01, 0x4c,

    };
    uint8_t exp[16] = 
    {
        0x8e, 0x9f, 0x01, 0x4d,
        0x4d, 0xdc, 0x01, 0x7e,
        0xa1, 0x58, 0x01, 0xbd,
        0xbc, 0x9d, 0x01, 0xf8,

    };
    struct AES_matrix *mat = AES_matrix_init();
    AES_matrix_feed(mat, data);
    printf("\n== Mat \n");
    AES_matrix_printfhex(mat);

    struct AES_matrix *expm = AES_matrix_init();
    AES_matrix_feed(expm, exp);
    
    
    struct AES_matrix *mi = AES_matrix_mixColumns(mat);
    printf("\n== MIX COLUMNS \n");
    AES_matrix_printfhex(mi);
    
    printf("%d", AES_matrix_areEqual(mi, expm));

    AES_matrix_free(mi);
   
    AES_matrix_free(mat);
    AES_matrix_free(expm);





    return EXIT_SUCCESS;
    
    srand(time(NULL));

    unsigned char *text = (unsigned char*)argv[1];
    unsigned char *key = (unsigned char*)argv[2];

    printf("text: %s   | key: %s\n", (char*)text, (char*)key);

    unsigned char *output = NULL;
    unsigned char *decrypt = NULL;

    AES_encrypt(text, key, &output);
    printf("\n encrypted text: ");
    printf("\n%s\n    \n", (char*)output);
    print_ascii(output); printf("\n");

    AES_decrypt(output, key, &decrypt);
    printf("\n decrypted text: \n%s\n    \n", (char*)decrypt);
    print_ascii(decrypt); printf("\n");

    free(output);
    free(decrypt);


    return EXIT_SUCCESS;
   
    if (argc == 1)
        return interface(argc, argv);

    if (argc == 2)
    {
        if (strcmp("--version", argv[1]) == 0)
            printf("Packup System 4 by PS4 %s v%s %s\n", TYPE, VERSION, DATE);
    }

    if (argc == 3)
    {
        if (strcmp("huffman", argv[1]) == 0)
        {
            char *text = argv[2];
            un_truc_explixcite(text);
        }

        if (strcmp("filesystem", argv[1]) == 0)
        {
            char *path = argv[2];

            struct meta_tree *tree = FILESYSTEM_build_metatree(path);
            printf("Scanning directory %s ...\n\n", path);
            printf("TREE:\n");
            print_tree(tree->son, 0);

            FILESYSTEM_save_metatree(tree, "test_tree_main.txt");
            printf("\n\nSaving tree from tree_test_main.txt\n");


            struct meta_tree *restored = FILESYSTEM_restore_metatree("test_tree_main.txt");
            printf("\n\nLoading tree to tree_test_main.txt\n");
            printf("RESTORED TREE: \n");
            print_tree(restored->son, 0);

            FILESYSTEM_free_metatree(restored);
            FILESYSTEM_free_metatree(tree);
        }
    }

    if (argc == 4)
    {
        if (strcmp("rotn", argv[1]) == 0)
        {
            char *text = argv[2];
            long key = atol(argv[3]);

            printf("text: %s   | key: %ld\n", text, key);

            ROTN_encrypt(text, key);
            printf("encrypted text: %s  |   ", text);
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
            printf("encrypted text: %s  |   ", text);
            print_ascii(text); printf("\n");

            VIGENERE_decrypt(text, key);
            printf("decrypted text: %s\n", text);
        }

        if (strcmp("aes", argv[1]) == 0)
        {
            char *text = argv[2];
            char *key = argv[3];

            printf("text: %s   | key: %s\n", text, key);

            char *output = NULL;
            char *decrypt = NULL;

            AES_encrypt(text, key, &output);
            printf("encrypted text: %s  |   ", output);
            print_ascii(output); printf("\n");

            AES_decrypt(output, key, &decrypt);
            printf("encrypted text: %s  |   ", decrypt);
            print_ascii(decrypt); printf("\n");

            free(output);
            free(decrypt);

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

            printf("text: %s (size: %ld)\np: %ld     | q: %ld\n", text, lentext, lp, lq);

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
                gmp_printf("%#Zx ", encrypt[i]);


            unsigned char *decode = RSA_decode(pri, encrypt, lentext);

            printf("\n\ndecode text: %s\n", decode);

            for (size_t i = 0; i < lentext; ++i)
                mpz_clear(encrypt[i]);
            free(encrypt);

            free(decode);

            RSA_free_public_key(pub);
            RSA_free_private_key(pri);
            mpz_clear(p);
            mpz_clear(q);

        }
    }

    return EXIT_SUCCESS;
    */
}
