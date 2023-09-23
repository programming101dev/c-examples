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
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void parse_arguments(int argc, char *argv[], char **file_path, char **offset);
static void handle_arguments(const char *binary_name, const char *file_path, const char *offset_str, off_t *offset);
off_t get_off_t_min(void) __attribute__((const));
off_t get_off_t_max(void) __attribute__((const));
static off_t parse_off_t(const char *binary_name, const char *offset_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void display_file(FILE *file, const char *message, off_t offset);


#if defined(__APPLE__)
#define D_OFF_FORMAT "%lld"
#else
#define D_OFF_FORMAT "%ld"
#endif


int main(int argc, char *argv[])
{
    char *file_path;
    char *offset_str;
    off_t offset;
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
    fseeko(file, 0L, SEEK_SET);
    display_file(file, "\n\nFile contents after SEEK_SET", 0);
    fseeko(file, 0L, SEEK_SET);
    fseeko(file, offset, SEEK_CUR);
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


static void handle_arguments(const char *binary_name, const char *file_path, const char *offset_str, off_t *offset)
{
    if(file_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The file path is required.");
    }

    if(offset_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The offset is required.");
    }

    *offset = parse_off_t(binary_name, offset_str);
}


off_t get_off_t_min(void)
{
    if (sizeof(off_t) == sizeof(char))
    {
        return SCHAR_MIN;
    }
    else if (sizeof(off_t) == sizeof(short))
    {
        return SHRT_MIN;
    }
    else if (sizeof(off_t) == sizeof(int))
    {
        return INT_MIN;
    }
    else if (sizeof(off_t) == sizeof(long))
    {
        return LONG_MIN;
    }
    else if (sizeof(off_t) == sizeof(long long))
    {
        return LLONG_MIN;
    }
    else
    {
        // Handle other sizes or display an error message
        fprintf(stderr, "Unsupported size of off_t\n");
        exit(EXIT_FAILURE);
    }
}

// Function to get the maximum value for off_t based on its size
off_t get_off_t_max(void)
{
    if (sizeof(off_t) == sizeof(char))
    {
        return SCHAR_MAX;
    }
    else if (sizeof(off_t) == sizeof(short))
    {
        return SHRT_MAX;
    }
    else if (sizeof(off_t) == sizeof(int))
    {
        return INT_MAX;
    }
    else if (sizeof(off_t) == sizeof(long))
    {
        return LONG_MAX;
    }
    else if (sizeof(off_t) == sizeof(long long))
    {
        return LLONG_MAX;
    }
    else
    {
        fprintf(stderr, "Unsupported size of off_t\n");
        exit(EXIT_FAILURE);
    }
}

// Function to parse a string into an off_t value with bounds checking
off_t parse_off_t(const char *binary_name, const char *str)
{
    off_t min = get_off_t_min();
    off_t max = get_off_t_max();
    char *endptr;
    intmax_t parsed_value;

    errno = 0;
    parsed_value = strtoimax(str, &endptr, 10);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing off_t.");
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    if(parsed_value < min || parsed_value > max)
    {
        usage(binary_name, EXIT_FAILURE, "off_t value out of range.");
    }

    return (off_t)parsed_value;
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


static void display_file(FILE *file, const char *message, off_t offset)
{
    char ch;

    printf("%s " D_OFF_FORMAT ":\n\n", message, offset);

    while(fread(&ch, sizeof(ch), 1, file) > 0)
    {
        printf("%c", ch);
    }
}
