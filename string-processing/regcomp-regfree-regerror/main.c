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
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void parse_arguments(int argc, char *argv[], char **pattern);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


// TODO: what are some valid vs invalid regexs?


int main(int argc, char *argv[])
{
    regex_t regex;
    int ret;
    char error_buffer[100];
    char *pattern =  NULL;

    parse_arguments(argc, argv, &pattern);

    if(pattern == NULL)
    {
        pattern = strdup("invalid[");
    }

    ret = regcomp(&regex, pattern, 0);

    if(ret != 0)
    {
        regerror(ret, &regex, error_buffer, sizeof(error_buffer));
        printf("Error compiling regex: %s\n", error_buffer);
        free(pattern);

        return EXIT_FAILURE;
    }

    printf("Regular expression compiled successfully\n");
    regfree(&regex);
    free(pattern);

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

    *pattern = strdup(argv[optind]);
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] [pattern]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h            Display this help message\n", stderr);
    exit(exit_code);
}
