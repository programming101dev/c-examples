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


#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void parse_arguments(int argc, char *argv[], char **string1, char **string2);
static void handle_arguments(const char *binary_name, const char *string1, const char *string2);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *string1;
    char *string2;
    int result;

    string1 = NULL;
    string2 = NULL;
    parse_arguments(argc, argv, &string1, &string2);
    handle_arguments(argv[0], string1, string2);
    result = strcmp(string1, string2);

    if(result == 0)
    {
        printf("\"%s\" and \"%s\" are identical.\n", string1, string2);
    }
    else
    {
        printf("\"%s\" and \"%s\" are different (%d).\n", string1, string2, result);
    }

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **string1, char **string2)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
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

    if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *string1 = argv[optind];
    *string2 = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *string1, const char *string2)
{
    if(string1 == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "string1 is required.");
    }

    if(string2 == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "string2 is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <string1> <string2>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}
