#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "struct.h"
#include "lz78/lz78.h"
#include "huffman/huffman.h"

void test_simple_huffman_compress(char *input_path, char *output_path)
{
    size_t len_data_input = 0;
    unsigned char *data_input = NULL;
    printf("Load input data file...\n");
    load_data_file(input_path, &data_input, &len_data_input);
    if (data_input == NULL)
        err(EXIT_FAILURE, "Fail to reassign");
    printf("Compress in progress...\n");
    struct huff_out *data_compress = NULL;
    data_compress = compression(data_input, (int)len_data_input);
    printf("Saving output file...\n");
    write_data_file((char *)data_compress->dataOUT,
            (ssize_t)data_compress->len, output_path);
    printf("Done !\n");
    printf("Weight input : %ld -> Weight output : %d\n", len_data_input,
            data_compress->len);
    printf("Ratio = %d%%\n",(data_compress->len * 100)/((int)len_data_input));
    free(data_compress);
    free(data_input);
}

void test_simple_huffman_decompress(char *input_path, char *output_path)
{
    size_t len_data_input = 0;
    unsigned char *data_input = NULL;
    printf("Load input data file...\n");
    load_data_file(input_path, &data_input, &len_data_input);
    printf("Decompress in progress...\n");
    struct huff_out *data_decompress = NULL;
    data_decompress = decompression(data_input, (int)len_data_input);
    printf("Saving output file...\n");
    write_data_file((char *)data_decompress->dataOUT,
            (ssize_t)data_decompress->len, output_path);
    printf("Done !\n\n\n");
    printf("Weight input : %ld -> Weight output : %d\n",
            len_data_input, data_decompress->len);
    printf("Ratio = %d%%\n",(data_decompress->len * 100)/((int)len_data_input));
    free(data_decompress);
    free(data_input);
}
