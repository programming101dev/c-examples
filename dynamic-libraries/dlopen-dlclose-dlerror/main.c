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

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **library_path);
static void           handle_arguments(const char *binary_name, const char *library_path);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char *library_path;
    void *handle;

    library_path = NULL;
    parse_arguments(argc, argv, &library_path);
    handle_arguments(argv[0], library_path);
    handle = dlopen(library_path, RTLD_LAZY);

    if(handle == NULL)
    {
        fprintf(stderr, "Error loading the shared library: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    printf("Opened %s\n", library_path);

    if(dlclose(handle) != 0)
    {
        fprintf(stderr, "Error unloading the shared library: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    printf("Closed %s\n", library_path);

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **library_path)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h:")) != -1)
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
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
        }
    }

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The library name is required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *library_path = argv[optind];
}

static void handle_arguments(const char *binary_name, const char *library_path)
{
    if(library_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The library path is required.");
    }
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <library path>\n", program_name);
    exit(exit_code);
}
