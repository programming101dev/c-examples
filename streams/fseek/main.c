/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */


#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void parse_arguments(int argc, char *argv[], char **file_path, char **offset);
static void handle_arguments(const char *binary_name, const char *file_path, const char *offset_str, long *offset);
static long parse_long(const char *binary_name, const char *offset_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void display_file(FILE *file, const char *message, long offset);


int main(int argc, char *argv[])
{
    char *file_path;
    char *offset_str;
    long offset;
    FILE *file;

    file_path = NULL;
    offset_str = NULL;
    parse_arguments(argc, argv, &file_path, &offset_str);
    handle_arguments(argv[0], file_path, offset_str, &offset);
    file = fopen(file_path, "r");

    if(file == NULL)
    {
        perror("Error opening the file");
        return EXIT_FAILURE;
    }

    display_file(file, "File contents", 0);
    fseek(file, 0L, SEEK_SET);
    display_file(file, "\n\nFile contents after SEEK_SET", 0);
    fseek(file, 0L, SEEK_SET);
    fseek(file, offset, SEEK_CUR);
    display_file(file, "\n\nFile contents after SEEK_CUR", offset);

    fclose(file);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **file_path, char **offset)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "ho:")) != -1)
    {
        switch(opt)
        {
            case 'o':
            {
                *offset = optarg;
                break;
            }
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[24];

                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *file_path = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *file_path, const char *offset_str, long *offset)
{
    if(file_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The file path is required.");
    }

    if(offset_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The offset is required.");
    }

    *offset = parse_long(binary_name, offset_str);
}


static long parse_long(const char *binary_name, const char *str)
{
    char *endptr;
    intmax_t parsed_value;

    errno = 0;
    parsed_value = strtoimax(str, &endptr, 10);

    if (errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing unsigned integer.");
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for signed long
    if (parsed_value < LONG_MIN || parsed_value > LONG_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Unsigned integer out of range for signed long.");
    }

    return (long)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <file path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h           Display this help message\n", stderr);
    fputs("  -o <offset>  The offset to move from the start of the file\n", stderr);
    exit(exit_code);
}


static void display_file(FILE *file, const char *message, long offset)
{
    char ch;

    printf("%s %ld:\n\n", message, offset);

    while(fread(&ch, sizeof(ch), 1, file) > 0)
    {
        printf("%c", ch);
    }
}
