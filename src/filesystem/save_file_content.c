#include "save_file_content.h"

void FILESYSTEM_save_file_content(FILE *src, FILE *dst)
{
    /*
    getting current offset
    */
    off_t beginning = ftell(dst);

    /*
    leaving space for length
    */
    int re = fseek(dst, sizeof(size_t), SEEK_CUR);

    /*
    declaring buffer, and all necessary variables
    */
    char buf[2048];
    int r;
    int w;
    int total;
    size_t len = 0;

    /*
    while there is content in the original file, copy it into the dst file
    also counts the number of chars written
    */
    while ((r = fread(buf, 2048, 1, src)))
    {
        w = fwrite(buf, r, 1, dst);
        if (w == 0)
            err(41, "FILESYSTEM_save_file_content: failed to write in dst.");
        total = w;
        while (total < r)
        {
            w = fwrite(buf + total, 1, r - total, dst);
            total += w;
        }
        len += total;
    }

    /*
    going back to saved space for len
    */
    fseek(dst, 0, beginning);

    /*
    writing said len
    */
    fwrite(&len, 1, sizeof(size_t), dst);

    /*
    going back to the end of the written content
    */
    fseek(dst, len, SEEK_CUR);
}

char *FILESYSTEM_load_file_content(FILE *file, off_t offset)
{
    /*
    declaring necessary variables
    */
    char *content;
    size_t len;

    /*
    going to content offset
    */
    fseek(file, offset, SEEK_SET);

    /*
    getting length of saved content
    */
    fread(&len, sizeof(size_t), 1, file);

    /*
    allocating memory for it
    */
    content = malloc(len);

    /*
    initializing end of string for strncat
    */
    *content = 0;

    /*
    declaring other necessary variables
    */
    char buf[2048];
    size_t total = 0;
    int r;

    /*
    copying file content in allocated memory
    */
    while (total < len)
    {
        if (total + 2048 > len)
        {
            fread(buf, len - total, 1, file);
            strncat(buf, content, (len - total));
            break;
        }
        r = fread(buf, 2048, 1, file);
        strncat(buf, content, r);
        total += r;
    }

    return content;
}
