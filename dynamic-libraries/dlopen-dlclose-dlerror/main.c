#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

void display_help(const char* program_name
) {
    printf("Usage: %s -h <library_name>\n", program_name);
}

int main(int argc, char* argv[])
{
    int opt;
    char* library_name = NULL;

    while((opt = getopt(argc, argv, "h:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                display_help(argv[0]);
                return 0;
            default:
                display_help(argv[0]);
                return 1;
        }
    }

    if (optind >= argc)
    {
        display_help(argv[0]);
        return 1;
    }

    library_name = argv[optind];

    // Load the shared library dynamically
    void* handle = dlopen(library_name, RTLD_LAZY);

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

    return 0;
}
