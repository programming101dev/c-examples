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
static void handle_arguments(const char *binary_name, const char *pattern);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *pattern;
    regex_t regex;
    int ret;
    char error_buffer[100];

    pattern = NULL;
    parse_arguments(argc, argv, &pattern);
    handle_arguments(argv[0], pattern);
    ret = regcomp(&regex, pattern, 0);

    if(ret != 0)
    {
        regerror(ret, &regex, error_buffer, sizeof(error_buffer));
        printf("Error compiling regex: %s\n", error_buffer);
    }

    printf("Regular expression compiled successfully\n");


    for(int i = optind + 1; i < argc; i++)
    {
        ret = regexec(&regex, argv[i], 0, NULL, 0);

        if(ret == 0)
        {
            printf("The string \"%s\" matches the regex: \"%s\"\n", argv[i], pattern);
        }
        else if(ret == REG_NOMATCH)
        {
            printf("The string \"%s\" does not match the regex: \"%s\"\n", argv[i], pattern);
        }
        else
        {
            regerror(ret, &regex, error_buffer, sizeof(error_buffer));
            printf("Error executing regex: %s on the string \"%s\"\n", error_buffer, argv[i]);
        }
    }

    regfree(&regex);

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
                usage(argv[0], EXIT_FAILURE, "Z");
            }
        }
    }

    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too few arguments.");
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

    fprintf(stderr, "Usage: %s [-h] pattern <test string>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h            Display this help message\n", stderr);
    exit(exit_code);
}

