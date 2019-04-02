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

#include "compression/huffman/huffman.h"
#include "compression/struct.h"
#include "compression/file.h"

#include "filesystem/build_metatree.h"
#include "filesystem/save_metatree.h"

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
            int len_a = strlen(argv[2]);
            unsigned char *text = (unsigned char *)(argv[2]);
            printf("Text input = %s\n", text);
            printf("Compressing...\n");
            struct huff_out *compressed = compression(text, len_a);
            printf("\nText ouput = %s | ", compressed->dataOUT);
            print_chare(compressed->dataOUT, compressed->len);
            printf("Ratio : %d%%\n", ((compressed->len * 100) / len_a));
            free_out(compressed);
        }

        if (strcmp("dehuffman", argv[1]) == 0)
        {
            unsigned char *text = (unsigned char *)(argv[2]);
            int len = strlen(argv[2]);
            struct huff_out *compressed = compression(text, len);

            struct huff_out *final = decompression(compressed->dataOUT,
                    compressed->len);
            int len_a = strlen((char *)final->dataOUT);
            if (len_a != len)
                printf("Longeur differente : %d -> %d\n", len, len_a);
            printf("Text input : %s\n", text);
            printf("Text output : ");
            print_chare(final->dataOUT, final->len);
            for (int i = 0; i < final->len; ++i)
                printf("%d", final->dataOUT[i]);
            printf("\n");
            free(compressed->dataOUT);
            free(compressed);
            free(final->dataOUT);
            free(final);
        }

        if (strcmp("filehuffman", argv[1]) == 0)
        {
            char *path_output = compression_function(argv[2]);
            long int ratio =(findSize(path_output)*100)/findSize(argv[2]);
            printf("Input_size = %ld\n", findSize(argv[2]));
            printf("Output_size = %ld\n", findSize(path_output));
            printf("Ratio = %ld %%\n", ratio);
            printf("Location compress file : %s\n", path_output);
            free(path_output);
        }

        if (strcmp("filedehuffman", argv[1]) == 0)
        {
            char *path_output = decompression_function(argv[2]);
            printf("Location decompress file : %s\n", path_output);
            free(path_output);
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


            mpz_t *encrypt = RSA_encode(pubk, (unsigned char*)text, lentext);
            printf("\nencode data: ");
            for (size_t i = 0; i < lentext; ++i)
            {
                size_t size;
                unsigned char *buf = get_str(128, encrypt[i], &size);//mpz_get_str(NULL, 62, encrypt[i]);
                //write(STDOUT_FILENO, buf, size); printf("\n");
                printf("%ld \n\n", size);
                free(buf);

                //gmp_printf("%#Zx ", encrypt[i]);
            }

            unsigned char *decode = RSA_decode(privk, encrypt, lentext);

            printf("\n\ndecode text: %s\n", decode);

            for (size_t i = 0; i < lentext; ++i)
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
