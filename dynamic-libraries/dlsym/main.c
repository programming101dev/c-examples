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

static void           parse_arguments(int argc, char *argv[], char **library_path, char **function_name);
static void           handle_arguments(const char *binary_name, const char *library_path, const char *function_name);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24

int main(int argc, char *argv[])
{
    char *library_path;
    char *function_name;
    void *handle;
    void (*func)(const char *);

    library_path  = NULL;
    function_name = NULL;
    parse_arguments(argc, argv, &library_path, &function_name);
    handle_arguments(argv[0], library_path, function_name);

    handle = dlopen(library_path, RTLD_LAZY);

    if(handle == NULL)
    {
        fprintf(stderr, "Error loading the shared library: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    // Get the symbol from the shared library
    /*
        The expression *(void **)(&func) might look complex, but it's used to correctly assign a function pointer
        obtained from dlsym to the func variable.

        Let's break down the expression step by step:

        &func:
            This takes the address of the func variable. It gives us a pointer to a function pointer,
            so it has a type of void ** (pointer to pointer to void).

        (void **)(&func):
            This explicitly casts the address of func to a void ** type. The void ** type
            indicates a pointer to a pointer to void. This is necessary because dlsym returns a void *
            (pointer to void), but we need to assign that to a function pointer variable func, which has a
            different type.

        *(void **)(&func):
            This dereferences the void ** pointer obtained from the previous step. It effectively
            treats the func variable as a void * pointer (since func is a function pointer), and then it performs
            a double indirection to get the value pointed to by the void ** pointer. This is done to properly assign
            the function pointer returned by dlsym to the func variable.
    */

    *(void **)(&func) = dlsym(handle, function_name);

    if(func == NULL)
    {
        fprintf(stderr, "Error getting the symbol '%s': %s\n", function_name, dlerror());
        dlclose(handle);
        return EXIT_FAILURE;
    }

    func("Hello, World!");

    if(dlclose(handle) != 0)
    {
        fprintf(stderr, "Error unloading the shared library: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **library_path, char **function_name)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hl:f:")) != -1)
    {
        switch(opt)
        {
            case 'l':
            {
                *library_path = optarg;
                break;
            }
            case 'f':
            {
                *function_name = optarg;
                break;
            }
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
}

static void handle_arguments(const char *binary_name, const char *library_path, const char *function_name)
{
    if(library_path == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The library path is required.");
    }

    if(function_name == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The function name is required.");
    }
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] -l <library path> -f <function name>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    fputs("  -l <library path>  Path to the shared library to load\n", stderr);
    fputs("  -f <function name> Function to call from the library (must be (void))\n", stderr);
    exit(exit_code);
}
