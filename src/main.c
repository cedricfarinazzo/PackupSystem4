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
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>

#include "gui/interface.h"

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

#define VERSION "2.0"
#define DATE "2018-04"
#define TYPE "dev"


void print_ps4logo(void)
{
#define OFFSET_LOGO "      "
    printf("\n%s", OFFSET_LOGO"[0;31;40m:[0;1;30;43m;;;;;;;;;;;;;;;[0;31;40m8[0;32;40m:[0;34;40m                           [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS:.[0;1;33;43m.[0;5;1;31;43mX[0;5;33;43m  [0;1;33;43m.[0;5;1;31;43mX[0;5;33;43m  [0;1;33;43m.[0;5;1;31;43mX[0;5;33;43m .[0;1;33;43m.[0;1;30;41m8[0;32;40m      [0;34;40m                     [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS[0;1;31;43m8[0;5;37;43mXX8[0;5;37;41m8[0;5;37;43m88[0;1;33;47m8[0;5;37;41m8[0;5;37;43m88[0;1;33;47m8[0;5;37;41m8[0;5;37;43m88[0;1;33;43mt[0;5;33;40mt[0;5;31;40mS[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;5;30;40m8[0;5;33;40m8[0;34;40m:[0;31;40m.[0;32;40m   [0;34;40m [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43m% [0;5;1;33;41m8[0;1;33;47m8[0;1;37;47m [0;1;33;47mS[0;1;37;47m [0;1;33;47mS[0;1;37;47m [0;1;33;47mX[0;1;37;47m [0;1;33;47mS[0;1;37;47m [0;1;33;47mX[0;1;37;47m  %[0;1;33;47m@%[0;1;30;47m [0;1;33;47mX[0;1;37;47m.[0;1;33;47mX[0;1;37;47m [0;1;33;47mX[0;1;37;47m.[0;1;33;47mX[0;1;37;47m [0;1;33;47mX[0;1;37;47m.[0;1;33;47mX[0;1;37;47m [0;1;33;47mX[0;1;37;47m.[0;1;33;47mX[0;1;37;47m [0;1;33;47mX[0;1;37;47m.[0;1;30;41m8[0;32;40m.    [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS[0;1;33;43m:[0;5;37;43mX8[0;1;33;47mS[0;1;37;47m t[0;5;37;47m8[0;5;1;33;47m8[0;5;37;47m8[0;1;37;47mt[0;5;37;47m8[0;5;1;33;47m8[0;5;37;47m8[0;1;37;47mS[0;5;1;37;43m8[0;5;37;47m8[0;1;37;47m%8888888888888888888[0;1;33;47mS[0;1;30;47m;[0;5;33;40mt[0;32;40m    [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43m; [0;5;1;33;41m8[0;1;33;47m8[0;1;37;47m %[0;5;1;33;47m8[0;5;37;47m88@%@8XS8Sttt[0;5;1;33;47m8[0;5;37;47m%tt[0;5;1;33;47m8[0;5;37;47m%tt[0;5;1;33;47m8[0;5;37;47m%tt[0;5;1;33;47m8[0;5;37;47m%tt;;t[0;1;37;47m%[0;31;40m.[0;32;40m   [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;1;33;43m%[0;5;37;43mX@[0;1;37;47m ;%[0;5;37;47m8@%t[0;5;1;33;47m8[0;5;37;47m8%ttS@[0;5;1;33;47m8[0;5;37;47mS%S@S%%t[0;1;33;47mS[0;5;37;47mt%ttX8tt[0;5;1;33;47m8[0;5;37;47mStt[0;1;33;47mS[0;31;40m. [0;32;40m  [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43m:[0;5;33;41m [0;5;37;47m:@S[0;5;1;33;47m@[0;5;37;47m8%[0;5;1;37;43m8[0;5;37;47m8[0;5;37;43m8[0;1;37;47mt[0;5;1;37;43m8[0;5;1;33;47m88[0;5;37;43m8[0;1;37;47m:[0;5;1;33;47m8[0;5;37;47m8[0;5;1;37;43m8[0;1;37;47m8[0;5;37;43mX8[0;5;37;47m8[0;5;37;43m@[0;5;37;47m%[0;5;37;43m8[0;1;37;47mX[0;5;1;37;43m8[0;1;37;47m8[0;5;37;43mX[0;1;33;47mS[0;5;37;43m8[0;5;37;47m8[0;5;37;43m8[0;5;37;47m@[0;5;1;37;43m8[0;1;37;47m8[0;5;37;43mX[0;1;37;47mS[0;1;30;43m;[0;5;31;40mS[0;1;30;43m:[0;34;40m [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS;[0;5;37;47m@t%S%[0;5;37;43m@@X8@8888@X[0;5;1;31;43mX[0;5;37;43m8X88@8X@XXX888X8X@X88@@[0;1;33;47m8[0;1;30;41m8[0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS;[0;5;37;47m8Stt[0;5;1;37;43m8[0;5;37;43m88[0;5;1;31;43m@[0;5;37;43m88[0;5;1;31;43m@[0;5;37;43m888[0;5;1;31;43m@[0;5;37;43m888[0;1;37;47m [0;5;37;43m8[0;1;37;47m [0;5;37;43m8888888888888888888[0;5;33;43m:[0;34;40m;[0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS[0;1;33;43m:[0;5;37;47mXt%S[0;5;37;43m@88888[0;5;1;37;43m8[0;5;37;43m8[0;5;1;31;43m8[0;5;1;33;47m8[0;5;37;43m88[0;5;37;47m888[0;1;37;47m    [0;5;1;31;43mX[0;5;37;43m88[0;5;1;31;43m8[0;5;37;43m88[0;5;1;31;43m@[0;5;37;43m88[0;5;1;31;43m@[0;5;37;43m88[0;5;1;31;43m8[0;5;37;43m8888[0;35;41m@[0;34;40m [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43m;[0;5;37;43mS[0;5;37;47m8St[0;5;37;43m888888[0;5;1;31;43m8[0;5;37;43m888888[0;5;37;47m8[0;1;37;47m [0;5;1;31;43mX[0;5;37;43m88[0;1;37;47m  [0;5;37;47m8[0;5;1;31;43m@[0;5;1;37;43m8[0;5;37;43m888[0;5;1;37;43m8[0;5;37;43m88[0;5;1;37;43m8[0;5;37;43m88[0;5;1;37;43m8[0;5;37;43m8[0;5;1;31;43m8[0;5;37;43m8[0;5;33;41m;[0;34;40m%[0;32;40m:[0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43m%[0;5;1;31;43m8[0;5;37;47m@t[0;5;1;33;47m8[0;5;37;43m88[0;5;1;31;43m@[0;5;1;37;43m8[0;5;1;31;43m8[0;5;37;47m8[0;5;37;43m8888[0;5;1;31;43m8[0;5;37;47m8[0;5;1;31;43m8[0;5;37;47m%[0;1;37;47m.[0;5;37;43m8888[0;1;37;47m;[0;5;37;43m88888[0;5;1;31;43m8[0;1;37;47m [0;5;1;31;43mX[0;5;1;37;43m8[0;5;1;33;41m8[0;5;37;43m88[0;5;1;31;43m8[0;5;37;47m8[0;5;37;43m88[0;5;31;40m8[0;32;40m.[0;34;40m [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS.[0;5;37;47m8@[0;5;37;43m888888[0;5;1;31;43mX[0;5;1;37;43m8[0;5;1;31;43m8[0;5;1;33;47m8[0;5;37;43m88[0;5;1;31;43mX[0;5;37;47m8[0;1;37;47m;[0;5;37;47m8[0;5;37;43m88[0;1;33;47mX[0;1;37;47m . [0;5;37;43m8[0;5;1;31;43m8[0;5;1;37;43m8[0;5;37;43m8[0;5;1;37;43m8[0;5;1;31;43mX[0;5;1;33;47m8[0;5;37;43m88888[0;5;1;31;43m@[0;5;37;43m8[0;31;43m8[0;34;40mt[0;31;40m [0;34;40m [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS[0;1;33;43m.[0;5;37;47m@8[0;5;37;43mS88888888[0;5;1;31;43m8[0;5;1;37;43m8[0;5;37;43m888[0;1;37;47mS [0;1;35;47m%S[0;35;47mXX[0;1;37;47m [0;5;37;43m8888[0;5;1;31;43m8[0;5;37;47m8[0;5;1;31;43mX[0;5;37;43m888888[0;5;1;37;43m8[0;1;31;47m8[0;31;40m8.[0;34;40m  [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43m;[0;5;37;43mS[0;5;37;47m8[0;5;1;31;43mS[0;5;37;43m88[0;5;1;31;43m8[0;5;1;33;47m8[0;5;1;31;43m8[0;5;1;37;43m8[0;5;37;43m888[0;1;33;47mS[0;5;1;31;43m8[0;5;1;37;43m8[0;1;33;47mX[0;1;35;47mS[0;1;37;47m.: [0;1;30;47m888[0;35;47m@@[0;5;37;43m88888888[0;5;1;31;43m8[0;5;37;47m8[0;5;1;31;43m8[0;5;1;37;43m8[0;5;1;31;43m8[0;5;31;40m8[0;32;40m.[0;34;40m.[0;31;40m  [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43m%[0;5;1;31;43m8[0;1;33;47mS[0;5;37;43m8888888[0;5;1;31;43m8[0;5;37;47m8[0;5;1;31;43m@[0;5;1;37;43m8[0;5;37;43m88[0;1;37;47m  ::[0;1;30;47mX[0;5;35;40mS [0;1;30;47mX@%[0;35;47mX[0;5;37;43m888[0;5;1;31;43m8[0;5;1;37;43m8[0;5;1;31;43m8[0;5;37;47m8[0;5;1;31;43mS[0;5;37;43m888[0;37;43m8[0;31;40mS[0;34;40m [0;31;40m   [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43mS.[0;1;33;47m8[0;5;37;43m88888[0;5;1;31;43m8[0;5;37;47m8[0;5;1;31;43mX[0;5;37;43m8888[0;5;1;31;43m8[0;5;37;47m8[0;1;37;47m  :[0;1;30;47m%[0;5;35;40m:[0;5;37;40mS[0;1;30;47mSSX@[0;5;37;43m8888888888[0;5;1;31;43m8[0;30;41m@[0;34;40mt[0;31;40m    [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;1;33;43mS[0;1;33;47m8[0;5;37;43m88[0;5;1;31;43m8[0;5;1;37;43m8[0;5;1;31;43m8[0;5;37;47m8[0;5;1;31;43mS[0;5;37;43m8888[0;5;1;31;43m8[0;5;37;47m8[0;5;37;43mX[0;5;1;31;43m@[0;1;37;47m;::: [0;1;30;47mSSS8[0;5;37;43m88[0;5;1;31;43m8[0;5;1;33;47m8[0;5;1;31;43m8[0;5;37;47m8[0;5;1;31;43m@[0;5;37;43m8888[0;1;30;45m8[0;31;40m%[0;34;40m [0;31;40m    [0m\n");
    printf("%s", OFFSET_LOGO"[0;1;30;41m8[0;5;33;43m:[0;1;33;47m8[0;5;37;43m8888888888[0;5;1;37;43m8[0;5;37;43m8[0;5;1;31;43m@[0;5;37;43m888[0;1;33;47mS[0;1;37;47m  [0;1;30;47m;@8[0;1;33;47m@[0;5;37;43m888888@888[0;5;1;31;43m8[0;1;33;47m8[0;31;40m8      [0m\n");
    printf("%s", OFFSET_LOGO"[0;5;30;40m@[0;5;37;43m888888[0;5;1;31;43m8[0;5;1;37;43m8[0;5;37;43m8[0;5;1;31;43m8[0;5;1;37;43m8[0;5;1;31;43m8[0;5;37;43m88888888@888888888[0;5;1;31;43m8[0;5;1;37;43m8[0;5;37;43m8[0;5;1;31;43m8[0;5;1;33;47m8[0;5;37;43m8[0;35;41m;[0;34;40m [0;31;40m      [0m\n");
    printf("%s", OFFSET_LOGO"[0;31;40m.[0;1;30;43mX[0;33;41m@[0;31;43m8[0;5;35;41m@[0;1;30;43m8[0;5;35;41mX[0;5;33;40m.[0;5;33;41m@[0;5;33;40m:[0;1;31;43m8[0;5;35;40m.[0;1;31;43m8[0;5;35;40m:[0;5;33;41m%[0;1;30;43m8[0;5;33;41mX[0;5;33;40m:[0;5;33;41mX[0;1;30;43m8[0;1;31;43m8[0;1;30;43m8[0;31;43m8[0;1;30;43m8[0;31;43m8[0;1;30;43m8[0;5;31;41m8[0;1;30;43m8[0;5;31;41mX[0;5;33;40m;[0;5;33;41mS[0;5;37;40m%[0;5;33;41m8[0;5;33;40m;[0;1;31;43m8[0;5;33;40m%[0;1;30;41m%[0;34;40m.[0;31;40m       [0m\n\n");
}

char line[4096];


int cp(const char *to, const char *from)
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
        printf("%spath:%s | ", indents, tree->data->path);
        printf("size:%ld o | ",  tree->data->fs.st_size);
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

/*
   struct keys
   {
//fill with whatever you need for every compression system or encryption system
//when initializing, please only fill what you need
//and don't forget to free
}keys;

char *restore_file(char *src, char *dst, int enc, int comp, struct keys *k)
{
if (dst == NULL)
{
dst = malloc(4096);
strcpy(dst, src);
char *start = dst + strlen(dst);
strcpy(dst, "_restored");
}

char tempfile[4096];
//set tempfile to whatever you wantand decrypt into it
switch (enc)
{
case 1:
//rotn
break;
case 2:
//vigenere
break;
case 3:
//aes
break;
case 4:
//rsa
break;
case 5:
//elgamal
break;
default:
break;
}

//from tempfile to dst
switch (comp)
{
case 1:
//huffman
break;
case 2:
//LZ
break;
default:
break;
}
}

struct keys *keys_init(int enc, int comp)
{
struct keys *key = malloc(sizeof(struct keys));
switch (enc)
{
case 0:
break;
case 1:
printf("Please give the number for rotn encryption:\n");
//Get whatever is needed for rotn
break;
case 2:
printf("Please give the key for vigenere encryption:\n");
//Get the key for vigenere
break;
case 3:
printf("Please give the password for aes encryption:\n");
//get whatever for aes
break;
case 4:
printf("Please give the link to the key for rsa encryption:\n");
//get whatever for rsa
break;
case 5:
printf("Please give whatever for elgamal encryption:\n");
//get whatever for elgamal encryption
break;
default:
break;
}

switch (comp)
{
    case 0:
        break;
    case 1:
        break;
    case 2:
        printf("Please give the dictionnary path for LZ compression:\n");
        char line[4096];
        getline(&line, 4096, stdin);
        //save it in keys struct
        break;
    default:
        break;
}
return key;
}

char *restore_clear_saves(char *save_dir, int enc, int comp)
{
    struct keys *k = keys_init(enc, comp);
    DIR *directory = opendir(save_dir);
    struct dirent *next;
    char *tempdir = malloc(4096);
    strcpy(tempdir, save_dir);
    char *start = tempdir + strlen(save_dir);
    strcpy(start, "/tempsaves");
    mkdir(tempdir, 0666);
    char tempname[4096];
    strcpy(tempname, save_dir);
    start = tempname + strlen(save_dir);
    char tempsave[4096];
    strcpy(tempsave, tempdir);
    char *savestart = tempsave + strlen(tempdir);
    while ((next = readdir(directory)))
    {
        if (strcmp(".", next->d_name) == 0 || strcmp("..", next->d_name) == 0)
        {
            continue;
        }
        strcpy(start, next->d_name);
        strcpy(savestart, next->d_name);
        switch (next->d_type)
        {
            case DT_REG:
                restore_file(tempname, tempsave, enc, comp, k);
                break;
            default:
                break;
        }
    }
    closedir(directory);
    free(k);
    return tempdir; //tempdir is a local variable. do some cancer here pls
}
*/

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


#define PASS_SIZE 256
char passbuff[PASS_SIZE];
void getPassword(char password[])
{
    static struct termios oldt, newt;
    int i = 0;
    int c;

    /*saving the old settings of STDIN_FILENO and copy settings for resetting*/
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;

    /*setting the approriate bit in the termios struct*/
    newt.c_lflag &= ~(ECHO);

    /*setting the new bits*/
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*reading the password from the console*/
    while ((c = getchar())!= '\n' && c != EOF && i < PASS_SIZE){
        password[i++] = c;
    }
    password[i] = '\0';

    /*resetting our old STDIN_FILENO*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

void ask_string_with_text(char *text, char *out)
{
    int awnser = -1;
    while (awnser < 0)
    {
        for (size_t i = 0; i < PATH_MAX; ++i)
            out[i] = 0;
        printf("%s", text);
        fgets(out, PATH_MAX, stdin);

        awnser = strlen(out) > 1 && out[0] != '\n' ? 1 : -1;
        printf("\n\n");
    }
    for (size_t i = strlen(out) - 1; i < PATH_MAX; ++i)
        out[i] = 0;
}

void ask_predef_with_text(char *text, char **allowed, size_t len, char *line)
{
    int awnser = -1;
    while (awnser < 0)
    {
        printf("%s", text);
        fgets(line, sizeof(line), stdin);

        int ok = 0;
        for(size_t i = 0; i < len && !ok; ++i)
        {
            size_t expect_len = strlen(allowed[i]);
            ok = strncmp(allowed[i], line, expect_len) == 0;
            if (ok)
                line[expect_len] = 0;
        }
        awnser = ok ? 1 : -1;
        printf("\n\n");
    }
}

void comp_chiff_file(char *save_file, enum ENCRYPTION_TYPE enc_type, int comp_type)
{
    if (enc_type == NONE && comp_type == 0)
        return;
    char save_file_comp[PATH_MAX];
    char save_file_enc[PATH_MAX];
    sprintf(save_file_comp, "%s.comp", save_file);
    sprintf(save_file_enc, "%s.enc", save_file);

    unsigned long rot_pass;
    char backup_pass[32768];
    size_t key_size = 0;
    int need_to_create_key = 0;
    char public_key_file[PATH_MAX];
    char private_key_file[PATH_MAX];
    char lz_dico_file[PATH_MAX];
    if (enc_type == RSA || enc_type == ELGAMAL)
    {
        ask_string_with_text("Please give the path of the public file key:\n", public_key_file);
        if (!file_exist(public_key_file))
        {
            need_to_create_key = 1;
            printf("Public key not found. new keys will be created\n");
            ask_string_with_text("Please give the path of the private file key:\n", private_key_file);

            ask_string_with_text("key size:\n", line);
            key_size = atol(line);
        }
    }

    if (enc_type == ROTN || enc_type == VIGENERE || enc_type == AES)
    {
        printf("Please give your backup password: \n");
        getPassword(passbuff);
        printf("\n\n");
        if (enc_type == ROTN)
            rot_pass = atol(passbuff);
        else
            strcpy(backup_pass, passbuff);
    }

    switch (comp_type) {
        case 0: // NONE
            rename(save_file, save_file_comp);
            break;

        case 1: // Huffman
            test_simple_huffman_compress(save_file, save_file_comp);
            break;

        case 2: //Lz78
            ask_string_with_text("Please give the path of the lz78 dictionnary file:\n", lz_dico_file);
            compress_lz78(save_file, lz_dico_file, save_file_comp);
            break;

        default:
            break;
    }

    if (enc_type == NONE) {
        rename(save_file_comp, save_file_enc);
    } else if (enc_type == VIGENERE) {
        FILE *in = fopen(save_file_comp, "r");
        FILE *out = fopen(save_file_enc, "w+");
        PACKUP_encryption_stream(VIGENERE, in, out, backup_pass);
        fclose(out);
        fclose(in);
    } else if (enc_type == ROTN) {
        FILE *in = fopen(save_file_comp, "r");
        FILE *out = fopen(save_file_enc, "w+");
        PACKUP_encryption_stream(ROTN, in, out, rot_pass);
        fclose(out);
        fclose(in);
    } else if (enc_type == AES) {
        FILE *in = fopen(save_file_comp, "r");
        FILE *out = fopen(save_file_enc, "w+");
        PACKUP_encryption_stream(AES, in, out, backup_pass);
        fclose(out);
        fclose(in);
    } else if (enc_type == RSA) {
        FILE *in = fopen(save_file_comp, "r");
        FILE *out = fopen(save_file_enc, "w+");
        if (need_to_create_key)
            PACKUP_encryption_stream(RSA, in, out, public_key_file, private_key_file, key_size);
        else
            PACKUP_encryption_stream(RSA, in, out, public_key_file);
        fclose(out);
        fclose(in);
    } else if (enc_type == ELGAMAL) {
        FILE *in = fopen(save_file_comp, "r");
        FILE *out = fopen(save_file_enc, "w+");
        if (need_to_create_key)
            PACKUP_encryption_stream(ELGAMAL, in, out, public_key_file, private_key_file, key_size);
        else
            PACKUP_encryption_stream(ELGAMAL, in, out, public_key_file);
        fclose(out);
        fclose(in);
    }

    remove(save_file);
    rename(save_file_enc, save_file);
    remove(save_file_comp);
    remove(save_file_enc);
}

void decomp_dechiff_backup(char *backup, enum ENCRYPTION_TYPE enc_type, int comp_type, char *tmp_dir, char *out)
{
    char *base = basename(backup);
    sprintf(out, "%s/%s", tmp_dir, base);
    if (enc_type == NONE && comp_type == 0)
        return;

    unsigned long rot_pass;
    char backup_pass[32768];
    char private_key_file[PATH_MAX];
    char lz_dico_file[PATH_MAX];
    if (enc_type == RSA || enc_type == ELGAMAL)
        ask_string_with_text("Please give the path of the private file key:\n", private_key_file);
    if (enc_type == ROTN || enc_type == VIGENERE || enc_type == AES)
    {
        printf("Please give your backup password: \n");
        getPassword(passbuff);
        printf("\n\n");
        if (enc_type == ROTN)
            rot_pass = atol(passbuff);
        else
            strcpy(backup_pass, passbuff);
    }

    if (comp_type == 2)
        ask_string_with_text("Please give the path of the lz78 dictionary file:\n", lz_dico_file);

    char save_file_comp[PATH_MAX];
    char save_file_enc[PATH_MAX];
    sprintf(save_file_enc, "%s.dec", out);
    sprintf(save_file_comp, "%s", out);

    if (enc_type == NONE) {
        cp(backup, save_file_enc);
    } else if (enc_type == VIGENERE) {
        FILE *in = fopen(backup, "r");
        FILE *out = fopen(save_file_enc, "w+");
        PACKUP_encryption_stream(VIGENERE, in, out, backup_pass);
        fclose(out);
        fclose(in);
    } else if (enc_type == ROTN) {
        FILE *in = fopen(backup, "r");
        FILE *out = fopen(save_file_enc, "w+");
        PACKUP_encryption_stream(ROTN, in, out, rot_pass);
        fclose(out);
        fclose(in);
    } else if (enc_type == AES) {
        FILE *in = fopen(backup, "r");
        FILE *out = fopen(save_file_enc, "w+");
        PACKUP_encryption_stream(AES, in, out, backup_pass);
        fclose(out);
        fclose(in);
    } else if (enc_type == RSA) {
        FILE *in = fopen(backup, "r");
        FILE *out = fopen(save_file_enc, "w+");
        PACKUP_encryption_stream(RSA, in, out, private_key_file);
        fclose(out);
        fclose(in);
    } else if (enc_type == ELGAMAL) {
        FILE *in = fopen(backup, "r");
        FILE *out = fopen(save_file_enc, "w+");
        PACKUP_encryption_stream(ELGAMAL, in, out, private_key_file);
        fclose(out);
        fclose(in);
    }

    switch (comp_type) {
        case 0: // NONE
            rename(save_file_enc, save_file_comp);
            break;

        case 1: // Huffman
            test_simple_huffman_compress(save_file_enc, save_file_comp);
            break;

        case 2: //Lz78
            ask_string_with_text("Please give the path of the lz78 dictionnary file:\n", lz_dico_file);
            compress_lz78(save_file_enc, lz_dico_file, save_file_comp);
            break;

        default:
            break;
    }

    remove(save_file_enc);
}

void decomp_dechiff_all_backup(char *save_dir, char *out_dir, enum ENCRYPTION_TYPE enc_type, int comp_type)
{
    //decomp_dechiff_backup(, enc_type, comp_type, tmp_dir, );
    DIR *d;
    struct dirent *f;
    d = opendir(save_dir);
    if (d)
    {
        while ((f = readdir(d)) != NULL)
        {
            if (strcmp(".", f->d_name) == 0 || strcmp("..", f->d_name) == 0)
                continue;
            char path[PATH_MAX];
            char out[PATH_MAX];
            sprintf(path, "%s/%s", save_dir, f->d_name);
            printf(" -> %s\n", path);
            decomp_dechiff_backup(path, enc_type, comp_type, out_dir, out);
        }
        closedir(d);
    }
}

int main_cli(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    printf("Hello, welcome to PackupSystem4 command line interface!\n\n");

    char *main_menu_0 = "0";
    char *main_menu_1 = "1";
    char *main_menu_q = "q";
    char *main_menu_args[] = {main_menu_0, main_menu_1, main_menu_q};
    ask_predef_with_text("Do you want to save a directory or restore it ?\n[0]: save\n[1]: restore\n[q]: Exit!\n", main_menu_args, 3, line);
    if (strcmp(line, main_menu_0) == 0) {
        printf("You have chosen to save a directory.\n\n");
        char *save_menu_0 = "0";
        char *save_menu_1 = "1";
        char *save_menu_args[] = {save_menu_0, save_menu_1};
        ask_predef_with_text("Do you already have a save or is it the first one ?\n"
                             "[0]: no prior save\n[1]: a save already exists\n", save_menu_args, 2, line);
        int have_save;
        if (strcmp(line, save_menu_0) == 0)
            have_save = 0;
        else
            have_save = 1;
        char save_dir[PATH_MAX];
        ask_string_with_text("Please give the path of the directory you want to save:\n", save_dir);

        char save_file[PATH_MAX];
        ask_string_with_text("Please give the path of the file in which you want to save it.\nPlease note that it will"
                             " be lost if it already exists:\n", save_file);
        if (have_save) {
            char previous_save[PATH_MAX];
            ask_string_with_text("Please give the name of the previous save:\n", previous_save);

            char *enc_menu_0 = "0";
            char *enc_menu_1 = "1";
            char *enc_menu_2 = "2";
            char *enc_menu_3 = "3";
            char *enc_menu_4 = "4";
            char *enc_menu_5 = "5";
            char *enc_menu_args[] = {enc_menu_0, enc_menu_1, enc_menu_2, enc_menu_3, enc_menu_4, enc_menu_5};
            ask_predef_with_text("Which encrypton was used ?\n"
                                 "[0]: no encryption\n[1]: Rotn\n[2]: Vigenere\n[3]: AES\n[4]: RSA\n[5]: Elgamal)\n", enc_menu_args, 6, line);
            enum ENCRYPTION_TYPE enc_type;
            switch (line[0]) {
                case '0':
                    enc_type = NONE; break;
                case '1':
                    enc_type = ROTN; break;
                case '2':
                    enc_type = VIGENERE; break;
                case '3':
                    enc_type = AES; break;
                case '4':
                    enc_type = RSA; break;
                case '5':
                    enc_type = ELGAMAL; break;
                default:
                    enc_type = NONE; break;
            }
            char *comp_menu_0 = "0";
            char *comp_menu_1 = "1";
            char *comp_menu_2 = "2";
            char *comp_menu_args[] = {comp_menu_0, comp_menu_1, comp_menu_2};
            ask_predef_with_text("Which compression was used ?\n"
                                 "[0]: no compression\n[1]: Huffman\n[2]: Lz78\n", comp_menu_args, 3, line);
            int comp_type = atoi(line);
            char tmp_dir[] = "./.packuptemp/";
            mkdir(tmp_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            char clear_backup_prev[PATH_MAX];
            decomp_dechiff_backup(previous_save, enc_type, comp_type, tmp_dir, clear_backup_prev);
            FILESYSTEM_create_new_save(save_dir, save_file, clear_backup_prev);
            remove_dir(tmp_dir);

        } else {
            FILESYSTEM_create_save(save_dir, save_file);
        }
        char *enc_menu_0 = "0";
        char *enc_menu_1 = "1";
        char *enc_menu_2 = "2";
        char *enc_menu_3 = "3";
        char *enc_menu_4 = "4";
        char *enc_menu_5 = "5";
        char *enc_menu_args[] = {enc_menu_0, enc_menu_1, enc_menu_2, enc_menu_3, enc_menu_4, enc_menu_5};
        ask_predef_with_text("Which encrypton do you want to used ?\n"
                             "[0]: no encryption\n[1]: Rotn\n[2]: Vigenere\n[3]: AES\n[4]: RSA\n[5]: Elgamal)\n", enc_menu_args, 6, line);
        enum ENCRYPTION_TYPE new_enc_type;
        switch (line[0]) {
            case '0':
                new_enc_type = NONE; break;
            case '1':
                new_enc_type = ROTN; break;
            case '2':
                new_enc_type = VIGENERE; break;
            case '3':
                new_enc_type = AES; break;
            case '4':
                new_enc_type = RSA; break;
            case '5':
                new_enc_type = ELGAMAL; break;
            default:
                new_enc_type = NONE; break;
        }
        char *comp_menu_0 = "0";
        char *comp_menu_1 = "1";
        char *comp_menu_2 = "2";
        char *comp_menu_args[] = {comp_menu_0, comp_menu_1, comp_menu_2};
        ask_predef_with_text("Which compression do you want to used ?\n"
                             "[0]: no compression\n[1]: Huffman\n[2]: Lz78\n", comp_menu_args, 3, line);
        int new_comp_type = atoi(line);
        comp_chiff_file(save_file, new_enc_type, new_comp_type);
        printf("\n Your new backup is here : %s", save_file);
    } else if (strcmp(line, main_menu_1) == 0) {
        printf("You have chosen to restore a dir.\n");
        char save_dir[PATH_MAX];
        ask_string_with_text("Please give the directory in which the saves are:\n", save_dir);
        char *enc_menu_0 = "0";
        char *enc_menu_1 = "1";
        char *enc_menu_2 = "2";
        char *enc_menu_3 = "3";
        char *enc_menu_4 = "4";
        char *enc_menu_5 = "5";
        char *enc_menu_args[] = {enc_menu_0, enc_menu_1, enc_menu_2, enc_menu_3, enc_menu_4, enc_menu_5};
        ask_predef_with_text("Which encrypton was used ?\n"
                             "[0]: no encryption\n[1]: Rotn\n[2]: Vigenere\n[3]: AES\n[4]: RSA\n[5]: Elgamal)\n", enc_menu_args, 6, line);
        enum ENCRYPTION_TYPE enc_type;
        switch (line[0]) {
            case '0':
                enc_type = NONE; break;
            case '1':
                enc_type = ROTN; break;
            case '2':
                enc_type = VIGENERE; break;
            case '3':
                enc_type = AES; break;
            case '4':
                enc_type = RSA; break;
            case '5':
                enc_type = ELGAMAL; break;
            default:
                enc_type = NONE; break;
        }
        char *comp_menu_0 = "0";
        char *comp_menu_1 = "1";
        char *comp_menu_2 = "2";
        char *comp_menu_args[] = {comp_menu_0, comp_menu_1, comp_menu_2};
        ask_predef_with_text("Which compression was used ?\n"
                             "[0]: no compression\n[1]: Huffman\n[2]: Lz78\n", comp_menu_args, 3, line);
        int comp_type = atoi(line);
        
        char tmp_dir[] = "./.packuptemp/";
        mkdir(tmp_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        decomp_dechiff_all_backup(save_dir, tmp_dir, enc_type, comp_type);
        FILESYSTEM_restore_save(tmp_dir);
        remove_dir(tmp_dir);
        printf("Restoration done\n");
    } else
        return EXIT_SUCCESS;
    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
    print_ps4logo();
    srand(time(NULL));

    if (argc == 1)
        return interface(argc, argv);

    if (argc == 2)
    {
        if (strcmp("--version", argv[1]) == 0)
            printf("Packup System 4 by PS4 %s v%s %s\n", TYPE, VERSION, DATE);

        if (strcmp("cli", argv[1]) == 0)
            return main_cli(argc, argv);
    }

    return EXIT_SUCCESS;
}

/*        if (strcmp("backup", argv[1]) == 0)
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
