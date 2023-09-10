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
#include <sys/stat.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **file_path);
static void handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *file_path;
    const char *symlink_name = "symlink_example.txt";
    struct stat link_stat;
    struct stat target_stat;

    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);

    if(symlink(file_path, symlink_name) == -1)
    {
        perror("Error creating symbolic link");
        return EXIT_FAILURE;
    }

    if(lstat(symlink_name, &link_stat) == -1)
    {
        perror("Error getting link information");
        return EXIT_FAILURE;
    }

    if(stat(symlink_name, &target_stat) == -1)
    {
        perror("Error getting target file information");
        return EXIT_FAILURE;
    }

    printf("Symbolic link information for: %s\n", symlink_name);
    printf("Link size: %lld bytes\n", (long long) link_stat.st_size);
    printf("Link permissions: %o\n", link_stat.st_mode & 0777);

    printf("\nTarget file information (follows the symbolic link):\n");
    printf("Target size: %lld bytes\n", (long long) target_stat.st_size);
    printf("Target permissions: %o\n", target_stat.st_mode & 0777);

    // Remove the symbolic link
    if(unlink(symlink_name) == -1)
    {
        perror("Error unlinking symbolic link");
        return EXIT_FAILURE;
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
