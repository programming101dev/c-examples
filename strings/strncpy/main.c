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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void parse_arguments(int argc, char *argv[], char **n, char **string);
static void handle_arguments(const char *binary_name, const char *n_str, const char *string, size_t *n);
static size_t parse_size_t(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *n_str;
    char *string;
    size_t n;
    char *destination;

    n_str = NULL;
    string = NULL;
    parse_arguments(argc, argv, &n_str, &string);
    handle_arguments(argv[0], n_str, string, &n);
    destination = malloc(n + 1);

    if(destination == NULL)
    {
        perror("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    strncpy(destination, string, n);
    printf("Source string: %s\n", string);
    printf("Copied string: %s\n", destination);
    free(destination);

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **n, char **string)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hn:")) != -1)
    {
        switch(opt)
        {
            case 'n':
            {
                *n = optarg;
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

    *string = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *n_str, const char *string, size_t *n)
{
    if(n_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "n is required.");
    }

    if(string == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The string is required.");
    }

    *n = parse_size_t(binary_name, n_str);
}


static size_t parse_size_t(const char *binary_name, const char *str)
{
    char *endptr;
    unsigned long long parsed_value;

    errno = 0;
    parsed_value = strtoull(str, &endptr, 10);

    if (errno != 0)
    {
        perror("strtoull");
        exit(EXIT_FAILURE);
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for size_t
    if (parsed_value > SIZE_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Value out of range for size_t.");
    }

    return (size_t)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] -n <length> <string>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h           Display this help message\n", stderr);
    fputs("  -n <length>  The number of characters to copy\n", stderr);
    exit(exit_code);
}
