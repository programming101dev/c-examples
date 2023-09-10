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


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **directory_path);
static void handle_arguments(const char *binary_name, const char *directrory_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static char *getCurrentWorkingDirectory(long path_max);
static void printCurrentWorkingDirectory(long path_max);


int main(int argc, char *argv[])
{
    char *directory_path;
    long path_max;

    directory_path = NULL;
    parse_arguments(argc, argv, &directory_path);
    handle_arguments(argv[0], directory_path);

    // Get the maximum path length
    path_max = pathconf(directory_path, _PC_PATH_MAX);

    if(path_max == -1)
    {
        path_max = 4096; // A common default value for the maximum path length
    }

    printCurrentWorkingDirectory(path_max);

    if(chdir(directory_path) == -1)
    {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    printCurrentWorkingDirectory(path_max);

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


char *getCurrentWorkingDirectory(long path_max)
{
    char *buffer = NULL;
    size_t size = (size_t) path_max;

    buffer = NULL;

    while(1)
    {
        buffer = realloc(buffer, size);

        if(buffer == NULL)
        {
            perror("Error allocating/reallocating memory for buffer");
            return NULL;
        }

        if(getcwd(buffer, size) != NULL)
        {
            return buffer; // Return the current working directory
        }
        else
        {
            // Check if the failure was due to insufficient buffer size
            if(errno == ERANGE)
            {
                // Retry with a larger buffer size
                size *= 2; // Double the buffer size
            }
            else
            {
                perror("Error getting current working directory");
                free(buffer);

                return NULL;
            }
        }
    }
}

void printCurrentWorkingDirectory(long path_max)
{
    char *cwd;

    cwd = getCurrentWorkingDirectory(path_max);

    if(cwd == NULL)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Current working directory: %s\n", cwd);
        free(cwd);
    }
}
