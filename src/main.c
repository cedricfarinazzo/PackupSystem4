#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "gui/interface.h"

#include "encryption/encryption.h"

#include "compression/huffman/huffman.h"
#include "compression/struct.h"
#include "compression/file.h"

#include "filesystem/build_metatree.h"
#include "filesystem/save_metatree.h"
#include "filesystem/build_restore_tree.h"
#include "filesystem/save_file_content.h"
#include "filesystem/restore_save.h"
#include "filesystem/create_save.h"

#define VERSION "2.0"
#define DATE "2018-04"
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

char *restore_clear_saves(char *save_dir, int enc, int comp)
{
    enc = enc;
    comp = comp;
    //TODO
    DIR *directory = opendir(save_dir);
    struct dirent *next;
    char tempdir[4096];
    strcpy(tempdir, save_dir);
    char *start = tempdir + strlen(save_dir);
    strcpy(start, "/tempsaves");
    mkdir(tempdir, 0666);
    char tempname[4096];
    strcpy(tempname, tempdir);
    start = tempname + strlen(tempdir);
    while ((next = readdir(directory)))
    {
    }
    return tempdir; //tempdir is a local variable. do some cancer here pls
}

void remove_dir(char *dir)
{
    dir = dir;
    //TODO
}

int main(int argc, char *argv[])
{
    argc = argc;
    argv = argv;
    printf("Hello, welcome to PackupSystem4.\n");
    printf("Do you want to save a directory or restore it ?\n"
           "(0 => save, 1 => restore)");
    if (/*save a dir*/0)
    {
        printf("You have chosen to save a directory.\n"
               "Do you already have a save or is it the first one ?\n"
               "(0 => no prior save, 1 => a save already exists)\n");
        printf("Please give the name of the directory you want to save:\n");
        char *dirpath;
        printf("Please give the name of the file in which you want to save it. Please note that it will\n"
               "be lost if it already exists:\n");
        char *savepath;
        char tempfile[4096];
        strcpy(tempfile, savepath);
        char *start = tempfile + strlen(savepath);
        strcpy(start, ".rdtgs");
        if (/*a save already exists*/0)
        {
            printf("Please give the name of the previous save:\n");
            char *previous_save;
            printf("Which encrypton was used ?\n"
                   "(0 => no encryption, 1 => Rotn, 2 => Vigenere, 3 => AES, 4 => RSA, 5 => Elgamal)\n");
            int enc;
            printf("Which compression was used ?\n"
                   "(0 => no compression, 1 => Huffman, 2 => LZ)\n");
            int comp;
            //decompresser et dechiffrer l'ancienne save
            char *prev;
            FILESYSTEM_create_new_save(dirpath, tempfile, prev);
        }
        else
        {
            FILESYSTEM_create_save(dirpath, tempfile);
        }
        printf("Please say which compression you want to use: (0 => no compression, 1 => Huffman, 2 => LZ)\n");
        int comp;
        char secondtempfile[4096];
        switch (comp)
        {
            case 0:
                break;
            case 1:
                //compression huffman
                break;
            case 2:
                //compression LZ
                break;
            default:
                break;
        }
        remove(tempfile);
        printf("Please say which encryption you want to use:\n"
               "(0 => no encryption, 1 => Rotn, 2 => Vigenere, 3 => AES, 4 => RSA, 5 => Elgamal)\n");
        int enc;
        switch (enc)
        {
            case 0:
                break;
            case 1:
                //encryption rotn
                break;
            case 2:
                //encryption vigenere
                break;
            case 3:
                //encryption aes
                break;
            case 4:
                //encryption rsa
                break;
            case 5:
                //encryption elgamal
                break;
            default:
                break;
        }
        remove(secondtempfile);
        printf("Save created\n");
    }
    if (/*restore a dir*/0)
    {
        printf("You have chosen to restore a dir.\n");
        printf("Please give the directory in which the saves are:\n");
        char *save_dir;
        printf("Please give the way it was encrypted:\n"
               "(0 => no encryption, 1 => Rotn, 2 => Vigenere, 3 => AES, 4 => RSA, 5 => Elgamal)\n");
        int enc;
        printf("Please give the way it was compressed:\n"
               "(0 => no compression, 1 => Huffman, 2 => LZ)\n");
        int comp;
        char *temp_saves = restore_clear_saves(save_dir, enc, comp);
        FILESYSTEM_restore_save(temp_saves);
        printf("Restoration done\n");
    }
    /*
       srand(time(NULL));

       if (argc == 1)
       return interface(argc, argv);

       if (argc == 2)
       {
       if (strcmp("--version", argv[1]) == 0)
       printf("Packup System 4 by PS4 %s v%s %s\n", TYPE, VERSION, DATE);

       if (strcmp("backup", argv[1]) == 0)
       {

       struct meta_tree *tree = FILESYSTEM_build_metatree("./testfiles/content");
       print_tree(tree->son, 0);
       FILESYSTEM_create_save("./testfiles/content", "./testfiles/saves/save.rdtgs");
       FILESYSTEM_free_metatree(tree);
       }

       if (strcmp("restore", argv[1]) == 0)
       {
       FILESYSTEM_restore_original_save("./testfiles/saves/save.rdtgs");

       struct meta_tree *tree = FILESYSTEM_build_metatree("./testfiles/content");
       print_tree(tree->son, 0);
       FILESYSTEM_free_metatree(tree);
       }

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
}

if (argc == 5)
{
    if (strcmp("aes-file-enc", argv[1]) == 0)
    {
        int e = AES_encrypt_file(argv[2], argv[3], argv[4]);
        if (e < 0)
            errx(e, "aes file enc: error ");
    }

    if (strcmp("aes-file-dec", argv[1]) == 0)
    {
        int e = AES_decrypt_file(argv[2], argv[3], argv[4]);
        if (e < 0)
            errx(e, "aes file dec: error ");
    }

    if (strcmp("gen-rsa-key", argv[1]) == 0)
    {
        struct RSA_pubKey *pubk;
        struct RSA_privKey *privk;
        unsigned long keysize = (unsigned long)atol(argv[2]);
        RSA_generateKey(keysize, &privk, &pubk);

        RSA_pubk_to_file(pubk, argv[3]);
        RSA_privk_to_file(privk, argv[4]);

        RSA_free_public_key(pubk);
        RSA_free_private_key(privk);

    }

    if (strcmp("rsa-file-enc", argv[1]) == 0)
    {
        struct RSA_pubKey *pub = RSA_pubKey_from_file(argv[4]);
        int e = RSA_encode_file(argv[2], argv[3], pub);
        RSA_free_public_key(pub);
        if (e < 0)
            errx(e, "rsa file enc: error ");
    }

    if (strcmp("rsa-file-dec", argv[1]) == 0)
    {
        struct RSA_privKey *priv = RSA_privKey_from_file(argv[4]);
        int e = RSA_decode_file(argv[2], argv[3], priv);
        RSA_free_private_key(priv);
        if (e < 0)
            errx(e, "aes file dec: error ");
    }

}*/
return EXIT_SUCCESS;
}
