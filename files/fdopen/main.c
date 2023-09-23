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


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **file_path);
static void handle_arguments(const char *binary_name, const char *file_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *file_path;
    int  fd;
    FILE *file;
    char buffer[256];
    file_path = NULL;
    parse_arguments(argc, argv, &file_path);
    handle_arguments(argv[0], file_path);
    fd = open(file_path, O_RDONLY);
    if(fd == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    file = fdopen(fd, "r");
    if(file == NULL)
    {
        perror("Error creating FILE* stream");
        close(fd);
        return EXIT_FAILURE;
    }
    while(fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer); // Print the content to stdout
    }
    if(fclose(file) == EOF)
    {
        perror("Error closing FILE* stream");
        return EXIT_FAILURE;
    }

    // close was called by fclose

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **file_path)
{
    int opt;
    opterr     = 0;
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
