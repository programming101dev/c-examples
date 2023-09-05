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


static void parse_arguments(int argc, char *argv[], bool *set_core_limit);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void abort_handler(void);
static void set_core_dump_limit(long long size);


int main(int argc, char *argv[])
{
    bool set_core_limit = false;

    parse_arguments(argc, argv, &set_core_limit);

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


static void parse_arguments(int argc, char *argv[], bool *set_core_limit)
{
    int opt;

    opterr = 0;

    // Parse command-line options
    while((opt = getopt(argc, argv, "hl")) != -1)
    {
        switch(opt)
        {
            case 'l':
            {
                *set_core_limit = true;
                break;
            }
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
}


_Noreturn  static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] [-l]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    fputs("  -l  Set the core dump limit to unlimited\n", stderr);
    exit(exit_code);
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
