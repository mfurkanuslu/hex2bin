#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void upcase_char(char* c);

#define check_file_name(file_name) strnlen(file_name, FILENAME_MAX + 1) > FILENAME_MAX

int main(int argc, char* argv[])
{
    FILE* input_file = NULL;
    FILE* output_file = NULL;
    char input_file_name[FILENAME_MAX];
    char output_file_name[FILENAME_MAX];
    int nibble_index = 1;
    unsigned int byte = 0;
    unsigned int invalid_char_count = 0;

    switch(argc)
    {
    case 1:
        fputs("Usage: hex2bin inputfile.hex outputfile.bin\n", stderr);
        return 1;
    case 2:
        fputs("Missing output file operand!\n", stderr);
        return 1;
    case 3:

        if(check_file_name(argv[1]))
        {
            fputs("Input file name is too long!\n", stderr);
            return 1;
        }

        if(check_file_name(argv[2]))
        {
            fputs("Output file name is too long!\n", stderr);
            return 1;
        }
        strncpy(input_file_name, argv[1], FILENAME_MAX);
        strncpy(output_file_name, argv[2], FILENAME_MAX);
        break;
    default:
        fputs("Operand count mismatch!\n", stderr);
        return 1;
    }

    if(!strncmp(input_file_name, output_file_name, FILENAME_MAX))
    {
        fputs("Files must be different!\n", stderr);
        return 1;
    }

    input_file = fopen(input_file_name, "rb");
    if(input_file == NULL)
    {
        perror(NULL);
        return 1;
    }

    output_file = fopen(output_file_name, "wb");
    if(output_file == NULL)
    {
        free(input_file);
        perror(NULL);
        return 1;
    }
    while(!feof(input_file))
    {
        int c = fgetc(input_file);
        upcase_char(&c);
        if((c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))
        {
            byte += (c < '=' ? c - '0' : c - '7') << (4 * nibble_index);
            if(nibble_index == 0)
            {
                fputc(byte, output_file);
                byte = 0;
            }
            nibble_index = (nibble_index + 1) % 2;
        }
        else
        {
            invalid_char_count++;
        }
    }
    if(invalid_char_count)
    {
        printf("Warning: %u invalid chars found in hex file! Nevertheless output file is created (by ignoring them).\n",
               invalid_char_count);
    }
    free(output_file);
    free(input_file);
    return 0;
}

void upcase_char(char* c)
{
    if(*c >= 'a' && *c <= 'f')
        *c -= 32;
}
