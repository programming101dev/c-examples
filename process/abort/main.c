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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>


static void parse_arguments(int argc, char *argv[], char **limit);
static void handle_arguments(const char *binary_name, const char *limit_str, rlim_t *limit);
rlim_t parse_rlim_t(const char *binary_name, const char *str);
long long detect_rlim_t_size(void);
static long long parse_long_long(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void abort_handler(void);
static void set_core_dump_limit(rlim_t limit);


int main(int argc, char *argv[])
{
    char *limit_str;
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

    // This line will not be reached due to the abort call
    printf("Main function finished.\n");
    return EXIT_SUCCESS;
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


rlim_t parse_rlim_t(const char *binary_name, const char *str)
{
    long long parsed_value = parse_long_long(binary_name, str);
    long long rlim_min, rlim_max;
    long long rlim_bits = detect_rlim_t_size();

    if (rlim_bits == 8)
    {
        rlim_min = INT8_MIN;
        rlim_max = INT8_MAX;
    }
    else if (rlim_bits == 16)
    {
        rlim_min = INT16_MIN;
        rlim_max = INT16_MAX;
    }
    else if (rlim_bits == 32)
    {
        rlim_min = INT32_MIN;
        rlim_max = INT32_MAX;
    }
    else if (rlim_bits == 64)
    {
        rlim_min = INT64_MIN;
        rlim_max = INT64_MAX;
    }
    else
    {
        fprintf(stderr, "Unknown size of rlim_t\n");
        exit(EXIT_FAILURE);
    }

    // Check if the parsed value is within the valid range for rlim_t
    if (parsed_value < rlim_min || parsed_value > rlim_max)
    {
        fprintf(stderr, "rlim_t value out of range: %lld\n", parsed_value);
        exit(EXIT_FAILURE);
    }

    return (rlim_t)parsed_value;
}


long long detect_rlim_t_size(void)
{
    switch (sizeof(rlim_t))
    {
        case 1: return 8;   // 8 bits
        case 2: return 16;  // 16 bits
        case 4: return 32;  // 32 bits
        case 8: return 64;  // 64 bits
        default: return -1; // Unknown size
    }
}


static long long parse_long_long(const char *binary_name, const char *str)
{
    char *endptr;
    long long parsed_value;

    errno = 0;
    parsed_value = strtoll(str, &endptr, 10);

    if (errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing long.");
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    return parsed_value;
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
