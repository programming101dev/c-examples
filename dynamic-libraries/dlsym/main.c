#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>


static void display_help(const char *program_name);


int main(int argc, char* argv[])
{
    int opt;
    char* library_name = NULL;
    char* function_name = NULL;
    void *handle;
    void (*func)(const char *);

    while((opt = getopt(argc, argv, "h:f:")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                display_help(argv[0]);
                return EXIT_FAILURE;
            }
            case 'f':
            {
                function_name = optarg;
                break;
            }
            default:
            {
                display_help(argv[0]);
                return EXIT_FAILURE;
            }
        }
    }

    if(optind >= argc || function_name == NULL)
    {
        display_help(argv[0]);
        return EXIT_FAILURE;
    }

    library_name = argv[optind];

    // Load the shared library dynamically
    handle = dlopen(library_name, RTLD_LAZY);

    if(!handle)
    {
        fprintf(stderr, "Error loading the shared library: %s\n", dlerror());
        return EXIT_FAILURE;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    // Get the symbol from the shared library
    func = (void (*)(const char*))dlsym(handle, function_name);
#pragma GCC diagnostic pop
    if(!func)
    {
        fprintf(stderr, "Error getting the symbol '%s': %s\n", function_name, dlerror());
        dlclose(handle);
        return EXIT_FAILURE;
    }

    // Call the function with "Hello, World!"
    func("Hello, World!");

    // Close the shared library
    if(dlclose(handle) != 0)
    {
        fprintf(stderr, "Error unloading the shared library: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


static void display_help(const char *program_name)
{
    printf("Usage: %s -h <library_name> -f <function_name>\n", program_name);
}
