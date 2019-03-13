#include "save_file_content.h"

void FILESYSTEM_save_file_content(FILE *src, FILE *dst)
{
    off_t beginning = ftell(dst);
    int re = fseek(dst, sizeof(size_t), SEEK_CUR);
    char buf[2048];
    int r;
    int w;
    int total;
    size_t len = 0;
    while ((r = fread(buf, 2048, 1, src)))
    {
        w = fwrite(buf, r, 1, dst);
        if (w == 0)
            err(41, "FILESYSTEM_save_file_content: failed to write in dst.");
        total = w;
        while (total < r)
        {
            w = fwrite(buf, 1, r - total, dst);
            total += w;
        }
        len += total;
    }
    fseek(dst, 0, beginning);
    fwrite(&len, 1, sizeof(size_t), dst);
}

char *FILESYSTEM_load_file_content(FILE *file, off_t offset)
{
    char *content;
    size_t len;
    fseek(file, offset, SEEK_SET);
    fread(&len, sizeof(size_t), 1, file);
    content = malloc(len);
    *content = 0;
    char buf[2048];
    size_t total = 0;
    int r;
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
