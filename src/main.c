#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "gui/interface.h"

#include "encryption/rotn.h"
#include "encryption/vigenere.h"

#include "encryption/rsa/rsa.h"
#include "encryption/rsa/genkey.h"
#include "encryption/rsa/tools.h"

#include "encryption/aes/aes.h"
#include "encryption/aes/aes_file.h"
#include "encryption/aes/aes_matrix.h"
#include "encryption/aes/aes_addroundkey.h"
#include "encryption/aes/aes_shiftrows.h"
#include "encryption/aes/aes_subbytes.h"
#include "encryption/aes/aes_mixcolumns.h"
#include "encryption/aes/hashpass.h"

#include "compression/huffman.h"

#include "filesystem/build_metatree.h"
#include "filesystem/save_metatree.h"

#include "tools/base62.h"

#define VERSION "1.0"
#define DATE "2018-03"
#define TYPE "dev"

// TO MOVE IN FILESYSTEM
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

void print_ascii(unsigned char *a)
{
    for (size_t i = 0; a[i] != 0; ++i)
        printf("%d ", a[i]);
}

int main(int argc, char *argv[])
{

    //char *text = "Mdr lol 42 is best.";
    char *text = "Mana";
    
    size_t len = strlen(text);
    printf("text (%ld): %s\n", len, text);
    
    size_t elen;
    char *encode = base62_encode(text, len, &elen);
    
    printf("encode (%ld): %s\n", elen, encode);
        
    size_t dlen;
    char *decode = base62_decode(encode, elen, &dlen);

    printf("decode (%ld): %s\n", dlen, decode);
    
    if (encode != NULL)
        free(encode);
    if (decode != NULL)
        free(decode);

    return 0;


    srand(time(NULL));
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
            print_ascii((unsigned char*)text); printf("\n");

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
            print_ascii((unsigned char*)text); printf("\n");

            VIGENERE_decrypt(text, key);
            printf("decrypted text: %s\n", text);
        }

        if (strcmp("aes", argv[1]) == 0)
        {
            unsigned char *text = (unsigned char*)argv[2];
            char *pass = argv[3];

            unsigned char *key = AES_keyFromPass(pass, strlen(pass));
            printf("password (%ld): %s\n", strlen(pass), pass);
            printf("text(%ld): %s   | key: %s\n\n", strlen((char*)text), text, key);

            unsigned char *output = NULL;
            unsigned char *decrypt = NULL;

            struct AES_ctx *ctx = AES_init(key, strlen((char*)key));

            size_t outlen = AES_encrypt(ctx, text, strlen((char*)text), &output);
            printf("\nencrypted text (%ld): %s  |   ", outlen, output);
            print_ascii(output); printf("\n");

            size_t delen = AES_decrypt(ctx, output, outlen, &decrypt);
            printf("\n\ndecrypted text (%ld): %s  |   ", delen, decrypt);
            print_ascii(decrypt); printf("\n");

            free(output);
            free(decrypt);
            AES_ctx_free(ctx);
            free(key);
        }

        if (strcmp("rsa", argv[1]) == 0)
        {
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
            mpz_t *encrypt = RSA_encode(pubk, (unsigned char*)text, lentext, &elen);
            printf("\nencode (%ld): \n", elen);
            for (size_t i = 0; i < elen; ++i)
            {
                char *b = mpz_get_str(NULL, 62, encrypt[i]);
                printf("(%ld):  %s\n", strlen(b), b);
                free(b);
            }
            
            size_t dlen;
            unsigned char *decode = RSA_decode(privk, encrypt, elen, &dlen);

            printf("\n\ndecode text (%ld): %s\n", strlen((char*)decode), decode);

            for (size_t i = 0; i < elen; ++i)
                mpz_clear(encrypt[i]);
            free(encrypt);
            free(decode);
            
            RSA_free_public_key(pubk);
            RSA_free_private_key(privk);
        }
    }

    if (argc == 5)
    {
        if (strcmp("aes-file-enc", argv[1]) == 0)
        {
            int fin = open(argv[2],O_RDONLY);
            int fout = open(argv[3], O_WRONLY | O_CREAT);
            int e = AES_encrypt_file(fin, fout, argv[4]);
            if (e == -1)
                err(e, "aes file enc: error ");
            close(fout);
            close(fin);
        }
        
        if (strcmp("aes-file-dec", argv[1]) == 0)
        {
            int fin = open(argv[2],O_RDONLY);
            int fout = open(argv[3], O_WRONLY | O_CREAT);
            int e = AES_decrypt_file(fin, fout, argv[4]);
            if (e == -1)
                err(e, "aes file dec: error ");
            close(fout);
            close(fin);
        }
    }
    return EXIT_SUCCESS;
}
