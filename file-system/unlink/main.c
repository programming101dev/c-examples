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
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **file_path);
static void           handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char *file_path;
    FILE *file;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);

    if(access(file_path, F_OK) == 0)
    {
        fprintf(stderr, "%s already exists, please us a file that does not exist\n", file_path);
        return EXIT_FAILURE;
    }

    file = fopen(file_path, "we");

    if(file == NULL)
    {
        perror("Error creating file");
        return EXIT_FAILURE;
    }

    printf("Created file %s\n", file_path);
    fprintf(file, "This is a sample file.\n");
    fclose(file);

    if(access(file_path, F_OK) == -1)
    {
        printf("File '%s' does not exist.\n", file_path);
        return EXIT_FAILURE;
    }

    if(unlink(file_path) == -1)
    {
        perror("Error deleting file");
        return EXIT_FAILURE;
    }

    if(access(file_path, F_OK) == -1)
    {
        printf("File '%s' successfully deleted.\n", file_path);
    }
    else
    {
        printf("File '%s' was not deleted.\n", file_path);
    }

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **file_path)
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
        usage(argv[0], EXIT_FAILURE, "The group id is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *file_path = argv[optind];
}

static void handle_arguments(const char *binary_name, const char *file_path)
{
    if(file_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The file path is required.");
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
