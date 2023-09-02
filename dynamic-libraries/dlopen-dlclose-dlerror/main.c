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
#include <dlfcn.h>


static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    int opt;
    char *library_name = NULL;

    while((opt = getopt(argc, argv, "h:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                usage(argv[0], EXIT_SUCCESS,  NULL);
                return 0;
            default:
                usage(argv[0], EXIT_SUCCESS, NULL);
                return 1;
        }
    }

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "");
        return 1;
    }

    library_name = argv[optind];

    // Load the shared library dynamically
    void *handle = dlopen(library_name, RTLD_LAZY);

    if(!handle)
    {
        fprintf(stderr, "Error loading the shared library: %s\n", dlerror());
        return 1;
    }

    // Close the shared library
    if(dlclose(handle) != 0)
    {
        fprintf(stderr, "Error unloading the shared library: %s\n", dlerror());
        return 1;
    }

    return EXIT_SUCCESS;
}


static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fputs(message, stderr);
    }

    fprintf(stderr, "Usage: %s -h <library_name>\n", program_name);
    exit(exit_code);
}
