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
#include <getopt.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>


static void parse_arguments(int argc, char *argv[], char **limit);
static void handle_arguments(const char *binary_name, const char *limit_str, rlim_t *limit);
static rlim_t get_rlim_t_max(void);
static rlim_t parse_rlim_t(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void abort_handler(void);
static void set_core_dump_limit(rlim_t limit);


#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10


int main(int argc, char *argv[])
{
    char   *limit_str;
    rlim_t limit;

    limit_str = NULL;
    parse_arguments(argc, argv, &limit_str);
    handle_arguments(argv[0], limit_str, &limit);
    set_core_dump_limit(limit);

    if(atexit(abort_handler) != 0)
    {
        perror("Error setting abort handler");
        return EXIT_FAILURE;
    }

    printf("Main function executing.\n");
    abort();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunreachable-code"
    printf("Main function finished.\n");
    return EXIT_SUCCESS;
#pragma GCC diagnostic pop
}


static void parse_arguments(int argc, char *argv[], char **limit)
{
    int opt;

    opterr = 0;

    // Parse command-line options
    while((opt = getopt(argc, argv, "hl:")) != -1)
    {
        switch(opt)
        {
            case 'l':
            {
                *limit = optarg;
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


static void handle_arguments(const char *binary_name, const char *limit_str, rlim_t *limit)
{
    if(limit_str == NULL)
    {
        *limit = RLIM_INFINITY;
    }
    else
    {
        *limit = parse_rlim_t(binary_name, limit_str);
    }
}


static rlim_t get_rlim_t_max(void)
{
    rlim_t value;

    if(sizeof(rlim_t) == sizeof(unsigned char))
    {
        value = (rlim_t)UCHAR_MAX;
    }
    else if(sizeof(rlim_t) == sizeof(unsigned short))
    {
        value = (rlim_t)USHRT_MAX;
    }
    else if(sizeof(rlim_t) == sizeof(unsigned int))
    {
        value = (rlim_t)UINT_MAX;
    }
    else if(sizeof(rlim_t) == sizeof(unsigned long))
    {
        value = (rlim_t)ULONG_MAX;
    }
    else if(sizeof(rlim_t) == sizeof(unsigned long long))
    {
        value = (rlim_t)ULLONG_MAX;
    }
    else
    {
        // Handle other sizes or display an error message
        fprintf(stderr, "Unsupported size of uid_t\n");
        exit(EXIT_FAILURE);
    }

    return value;
}


static rlim_t parse_rlim_t(const char *binary_name, const char *str)
{
    rlim_t    max;
    char      *endptr;
    uintmax_t parsed_value;

    max           = get_rlim_t_max();
    errno         = 0;
    parsed_value  = strtoumax(str, &endptr, BASE_TEN);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing uid_t.");
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    if(parsed_value > (uintmax_t)max)
    {
        usage(binary_name, EXIT_FAILURE, "uid_t value out of range.");
    }

    return (rlim_t)parsed_value;
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] [-l limit]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    fputs("  -l  Set the core dump limit to unlimited\n", stderr);
    exit(exit_code);
}


static void abort_handler(void)
{
    printf("Abort handler called.\n");
}


static void set_core_dump_limit(rlim_t limit)
{
    struct rlimit rlim;

    rlim.rlim_cur = limit;
    rlim.rlim_max = limit;

    if(setrlimit(RLIMIT_CORE, &rlim) == -1)
    {
        perror("Error setting core dump limit");
        exit(EXIT_FAILURE);
    }

    printf("Core dump limit set to: \"unlimited\"\n");
}
