#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

void display_help(const char* program_name) {
    printf("Usage: %s -h <library_name> -f <function_name>\n", program_name);
}

int main(int argc, char* argv[]) {
    int opt;
    char* library_name = NULL;
    char* function_name = NULL;

    while ((opt = getopt(argc, argv, "h:f:")) != -1) {
        switch (opt) {
            case 'h':
                display_help(argv[0]);
                return 0;
            case 'f':
                function_name = optarg;
                break;
            default:
                display_help(argv[0]);
                return 1;
        }
    }

    if (optind >= argc || function_name == NULL) {
        display_help(argv[0]);
        return 1;
    }

    library_name = argv[optind];

    // Load the shared library dynamically
    void* handle = dlopen(library_name, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error loading the shared library: %s\n", dlerror());
        return 1;
    }

    // Get the symbol from the shared library
    void (*func)(const char*) = dlsym(handle, function_name);
    if (!func) {
        fprintf(stderr, "Error getting the symbol '%s': %s\n", function_name, dlerror());
        dlclose(handle);
        return 1;
    }

    // Call the function with "Hello, World!"
    func("Hello, World!");

    // Close the shared library
    if (dlclose(handle) != 0) {
        fprintf(stderr, "Error unloading the shared library: %s\n", dlerror());
        return 1;
    }

    return 0;
}
