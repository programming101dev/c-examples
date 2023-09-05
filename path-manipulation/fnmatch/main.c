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


#include <stdio.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <getopt.h>


static void parse_arguments(int argc, char *argv[], char **pattern);

static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *pattern = NULL;

    parse_arguments(argc, argv, &pattern);

    for(int i = optind + 1; i < argc; i++)
    {
        const char *filename = argv[i];

        if(fnmatch(pattern, filename, 0) != 0)
        {
            printf("Filename '%s' doesn't match the pattern.\n", filename);
        }
        else
        {
            printf("Filename '%s' matches the pattern.\n", filename);
        }
    }

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **pattern)
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
                break;
            }
            case '?':
            {
                char message[24];

                snprintf(message, sizeof(message), "Unknown option '-%c'.\n", optopt);
                usage(argv[0], EXIT_FAILURE, message);
                break;
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if (optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "Pattern is required.\n");
    }

    *pattern = argv[optind];
}


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <pattern> <filename1> [filename2 ...]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}
