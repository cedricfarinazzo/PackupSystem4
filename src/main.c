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

#include "compression/huffman/huffman.h"
#include "compression/struct.h"
#include "compression/file.h"

#include "filesystem/build_metatree.h"
#include "filesystem/save_metatree.h"

#define VERSION "1.0"
#define DATE "2018-02"
#define TYPE "release"

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

void print_ascii(char *a)
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
}
