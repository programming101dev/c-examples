#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/resource.h>


static void print_help(void);
static void abort_handler(void);
static void set_core_dump_limit(long long size);


int main(int argc, char *argv[])
{
    int opt;
    bool set_core_limit;

    set_core_limit = false;

    // Parse command-line options
    while((opt = getopt(argc, argv, "lh")) != -1)
    {
        switch(opt)
        {
            case 'l':
                set_core_limit = true;
                break;
            case 'h':
                print_help();
                return EXIT_SUCCESS;
            case '?':
                fprintf(stderr, "Unknown option: '%c'\n", optopt);
                return EXIT_FAILURE;
            default:
                break;
        }
    }

    if(atexit(abort_handler) != 0)
    {
        perror("Error setting abort handler");
        return EXIT_FAILURE;
    }

    if(set_core_limit)
    {
        set_core_dump_limit(RLIM_INFINITY);
    }

    printf("Main function executing.\n");
    abort();

    // This line will not be reached due to the abort call
    printf("Main function finished.\n");
    return EXIT_SUCCESS;
}

static void print_help(void)
{
    printf("Usage: core_dump [-l] [-h]\n");
    printf("Options:\n");
    printf("  -l  Set the core dump limit to unlimited\n");
    printf("  -h  Display this help message\n");
}

static void abort_handler(void)
{
    printf("Abort handler called.\n");
}

static void set_core_dump_limit(long long size)
{
    struct rlimit rlim;

    rlim.rlim_cur = size;
    rlim.rlim_max = size;

    if(setrlimit(RLIMIT_CORE, &rlim) == -1)
    {
        perror("Error setting core dump limit");
        exit(EXIT_FAILURE);
    }

    printf("Core dump limit set to: \"unlimited\"\n");
}
