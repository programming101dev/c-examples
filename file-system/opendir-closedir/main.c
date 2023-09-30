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

#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void           parse_arguments(int argc, char *argv[], char **directory_path);
static void           handle_arguments(const char *binary_name, const char *directory_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char *directory_path;
    DIR  *dir;

    directory_path = NULL;
    parse_arguments(argc, argv, &directory_path);
    handle_arguments(argv[0], directory_path);
    dir = opendir(directory_path);

    if(dir == NULL)
    {
        perror("opendir");
        return EXIT_FAILURE;
    }

    printf("Opened directory: %s\n", directory_path);

    if(closedir(dir) == -1)
    {
        perror("closedir");
        return EXIT_FAILURE;
    }

    printf("Closed directory: %s\n", directory_path);

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **directory_path)
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
                char message[UNKNOWN_OPTION_MESSAGE_LEN];

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
        usage(argv[0], EXIT_FAILURE, "The directory path is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *directory_path = argv[optind];
}

static void handle_arguments(const char *binary_name, const char *directory_path)
{
    if(directory_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The directory path is required.");
    }
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <directory path>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}
