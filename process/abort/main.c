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
