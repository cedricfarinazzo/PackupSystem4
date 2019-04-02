#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/stat.h>
#include "huffman/huffman.h"
#include "struct.h"
#include "file.h"

long int findSize(const char *file_name)
{
    struct stat st; /*declare stat variable*/
    if(stat(file_name,&st)==0)
        return (st.st_size);
    else
        return -1;
}

char *compression_function(char *path_input)
{
    if (path_input == NULL)
        errx(EXIT_FAILURE, "No input file");
    char *path_output2 = calloc(sizeof(char), strlen(path_input) + 5);
    int len_inp = strlen(path_input);
    for (int i = 0; i < len_inp; ++i)
    {
        path_output2[i] = path_input[i];
    }
    char extension[4] = ".huf";
    for (int i = len_inp; i < len_inp + 4; ++i)
        path_output2[i] = extension[i - len_inp];


    FILE *input = fopen(path_input, "r");
    printf("path_input = %s\n", path_input);
    printf("path_output = %s\n", path_output2);
    if (input == NULL)
        errx(EXIT_FAILURE, "Fail to open the input file");

    int len_in = (int)findSize(path_input);
    printf("len_in = %d\n", len_in);
    if (len_in == -1)
        errx(EXIT_FAILURE, "Fail to get the file size");

    char *input_data = malloc(sizeof(char) * len_in);
    fgets(input_data, (int)len_in, input);
    fclose(input);

    printf("input_data = %*s\n", len_in, input_data);
    struct huff_out *out = compression((unsigned char *)input_data, len_in - 1);
    free(input_data);
    FILE *output = fopen(path_output2, "w+");
    if (output == NULL)
        errx(EXIT_FAILURE, "Fail to open the output file");

    for (int i = 0; i < out->len; ++i)
    {
        fputc((char)out->dataOUT[i], output);
    }
    free(out->dataOUT);
    free(out);
    fclose(output);
    return path_output2;
}

char *decompression_function(char *path_input)
{
    if (path_input == NULL)
        errx(EXIT_FAILURE, "No input file");

    FILE *input = fopen(path_input, "r");
    if (input == NULL)
        errx(EXIT_FAILURE, "Fail to open the input file");

    int len_in = (int)findSize(path_input);
    if (len_in == -1)
        errx(EXIT_FAILURE, "Fail to get the file size");

    char *input_data = malloc(sizeof(char) * len_in + 1);
    fgets(input_data, (int)len_in, input);
    fclose(input);
    printf("Len_in = %d;\nData = ", len_in);
    for (int i = 0; i < len_in; ++i)
    {
        printf("%d ", (unsigned char)input_data[i]);
    }
    printf("\n\n");

    struct huff_out *out = decompression((unsigned char *)input_data, len_in);

    char *path_output = calloc(sizeof(char), (strlen(path_input) + 5));
    int len_inp = strlen(path_input);
    for (int i = 0; i < len_inp; ++i)
    {
        path_output[i] = path_input[i];
    }
    char extension[4] = ".txt";
    for (int i = len_inp; i < len_inp + 4; ++i)
        path_output[i] = extension[i - len_inp];

    FILE *output = fopen(path_output, "w+");
    if (output == NULL)
        errx(EXIT_FAILURE, "Fail to open the output file");

    for (int i = 0; i < out->len; ++i)
    {
        fputc(out->dataOUT[i], output);
    }
    fclose(output);
    free(out->dataOUT);
    free(out);
    free(input_data);
    return path_output;
}
