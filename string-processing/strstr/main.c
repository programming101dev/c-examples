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


static void parse_arguments(int argc, char *argv[], char **needle, char **haystack);
static void handle_arguments(const char *binary_name, const char *needle, char *haystack);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void search_for(const char *needle, const char *haystack);


int main(int argc, char *argv[])
{
    char *needle;
    char *haystack;

    needle = NULL;
    haystack = NULL;
    parse_arguments(argc, argv, &needle, &haystack);
    handle_arguments(argv[0], needle, haystack);
    search_for(needle, haystack);

    return EXIT_SUCCESS;
}


static void search_for(const char *needle, const char *haystack)
{
    char *found_substring;

    found_substring = strstr(haystack, needle);

    if(found_substring != NULL)
    {
        printf("Found substring '%s' at position %ld\n", needle, found_substring - haystack);
    }
    else
    {
        printf("Substring '%s' not found.\n", needle);
    }
}


static void parse_arguments(int argc, char *argv[], char **needle, char **haystack)
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

    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too few arguments.");
    }
    else if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *needle = argv[optind];
    *haystack = argv[optind + 1];
}


static void handle_arguments(const char *binary_name, const char *needle, char *haystack)
{
    if(needle == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "");
    }

    if(haystack == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <file path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


static void display_file(FILE *file, const char *message)
{
    char ch;

    fputs(message, stdout);

    while((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }
}
