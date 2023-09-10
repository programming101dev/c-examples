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
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>


static void parse_arguments(int argc, char *argv[], char **pattern);
static void handle_arguments(const char *binary_name, const char *pattern);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


// TODO: what are the parameters for the glob function?


int main(int argc, char *argv[])
{
    char *pattern;
    glob_t glob_result;

    pattern = NULL;
    parse_arguments(argc, argv, &pattern);
    handle_arguments(argv[0], pattern);

    if(glob(pattern, 0, NULL, &glob_result) != 0)
    {
        fprintf(stderr, "Error matching files.\n");
        return EXIT_FAILURE;
    }

    for(size_t i = 0; i < glob_result.gl_pathc; ++i)
    {
        printf("Matched file: %s\n", glob_result.gl_pathv[i]);
    }

    globfree(&glob_result);

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

    *pattern = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *pattern)
{
    if(pattern == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The pattern is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <pattern>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}
