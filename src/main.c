#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <sys/types.h>
#include <utime.h>

#include "encryption/encryption.h"

#include "compression/wrap.h"
#include "compression/huffman/huffman.h"
#include "compression/struct.h"
#include "compression/file.h"
#include "compression/lz78/lz78.h"

#include "filesystem/build_metatree.h"
#include "filesystem/save_metatree.h"
#include "filesystem/build_restore_tree.h"
#include "filesystem/save_file_content.h"
#include "filesystem/restore_save.h"
#include "filesystem/create_save.h"

#define VERSION "3.0"
#define DATE "2019-05"
#define TYPE "webbin"


int cp(const char *from, const char *to)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;
    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;
    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;
    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;
        do {
            nwritten = write(fd_to, out_ptr, nread);
            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
                goto out_error;
        } while (nread > 0);
    }
    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);
        /* Success! */
        return 0;
    }
  out_error:
    saved_errno = errno;
    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);
    errno = saved_errno;
    return -1;
}


// TO MOVE IN FILESYSTEM
void print_tree(struct meta_tree *tree, int indent)
{
    char indents[indent + 1];
    for (int i = 0; i < indent; ++i)
        indents[i] = ' ';
    indents[indent] = 0;

    if (tree != NULL)
    {
        printf("%s%s | size: %ld o\n", indents, tree->data->path, tree->data->fs.st_size);

        struct meta_tree *c = tree->son;
        while (c)
        {
            print_tree(c, indent + 6);
            c = c->sibling;
        }
    }
}

int remove_dir(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d)
    {
        struct dirent *p;
        r = 0;
        while (!r && (p = readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
                continue;
            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(sizeof(char) * len);
            if (buf)
            {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);
                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                        r2 = remove_dir(buf);
                    else
                        r2 = unlink(buf);
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    if (!r)
        r = rmdir(path);

    return r;
}

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc == 2)
    {
        if (strcmp("--version", argv[1]) == 0)
            printf("Packup System 4 by PS4 %s v%s %s\n", TYPE, VERSION, DATE);
    }
    
    if (argc == 6)
    {
        enum ENCRYPTION_TYPE enc_type;
        int comp_type;
        int rotn_pass = 0;
        char *backup_path = argv[1];
        char *backup_name = basename(backup_path);
        char *lz_dico = NULL;
        char *priv_key = NULL;
        char *backup_pass = NULL;
        if (strcmp(argv[2], "HUFF") == 0) {
            comp_type = 1;
        } else if (strcmp(argv[2], "LZ") == 0) {
            comp_type = 2;
            lz_dico = argv[3];
        } else {
            comp_type = 0;
        }

        if (strcmp(argv[4], "ROTN") == 0) {
            enc_type = ROTN;
            rotn_pass = atoi(argv[5]);
        } else if (strcmp(argv[4], "VIGENERE") == 0) {
            enc_type = VIGENERE;
            backup_pass = argv[5];
        } else if (strcmp(argv[4], "AES") == 0) {
            enc_type = AES;
            backup_pass = argv[5];
        } else if (strcmp(argv[4], "RSA") == 0) {
            enc_type = RSA;
            priv_key = argv[5];
        } else if (strcmp(argv[4], "ELGAMAL") == 0) {
            enc_type = ELGAMAL;
            priv_key = argv[5];
        } else {
            enc_type = NONE;
        }

        char exe_path[PATH_MAX];
        if (realpath("/proc/self/exe", exe_path) == NULL)
            return EXIT_FAILURE;
        char *exe_dir = dirname(exe_path);

        char tmp_dir[PATH_MAX];
        char buff_rand[9];
        sprintf(tmp_dir, "%s/.packuptemp.%s/", exe_dir, rand_string(buff_rand, 9));
        mkdir(tmp_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        
        char save_file_dec[PATH_MAX];
        char save_file_out[PATH_MAX];
        sprintf(save_file_dec, "%s/%s.dec", tmp_dir, backup_name);
        sprintf(save_file_out, "%s/%s.out", tmp_dir, backup_name);

        if (enc_type == NONE) {
            cp(backup_path, save_file_dec);
        } else if (enc_type == VIGENERE) {
            FILE *in = fopen(backup_path, "r");
            FILE *out = fopen(save_file_dec, "w+");
            PACKUP_decryption_stream(VIGENERE, in, out, backup_pass);
            fclose(out);
            fclose(in);
        } else if (enc_type == ROTN) {
            FILE *in = fopen(backup_pass, "r");
            FILE *out = fopen(save_file_dec, "w+");
            PACKUP_decryption_stream(ROTN, in, out, rotn_pass);
            fclose(out);
            fclose(in);
        } else if (enc_type == AES) {
            FILE *in = fopen(backup_path, "r");
            FILE *out = fopen(save_file_dec, "w+");
            PACKUP_decryption_stream(AES, in, out, backup_pass);
            fclose(out);
            fclose(in);
        } else if (enc_type == RSA) {
            FILE *in = fopen(backup_path, "r");
            FILE *out = fopen(save_file_dec, "w+");
            PACKUP_decryption_stream(RSA, in, out, priv_key);
            fclose(out);
            fclose(in);
        } else if (enc_type == ELGAMAL) {
            FILE *in = fopen(backup_path, "r");
            FILE *out = fopen(save_file_dec, "w+");
            PACKUP_decryption_stream(ELGAMAL, in, out, priv_key);
            fclose(out);
            fclose(in);
        }

        switch (comp_type) {
            case 0: // NONE
                rename(save_file_dec, save_file_out);
                break;

            case 1: // Huffman
                test_simple_huffman_decompress(save_file_dec, save_file_out);
                break;

            case 2: //Lz78
                decompress_lz78(save_file_dec, lz_dico, save_file_out);
                break;

            default:
                break;
        }
    
        remove(save_file_dec);

        struct meta_tree *tree = FILESYSTEM_SAVE_restore_metatree_from_save(save_file_out);

        printf("\n");
        print_tree(tree->son, 0);
        FILESYSTEM_free_metatree(tree);
        printf("\n");
        remove(save_file_out);
        remove_dir(tmp_dir);
    }

    return EXIT_SUCCESS;
}
