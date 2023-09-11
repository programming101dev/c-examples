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
#include <time.h>


static void parse_arguments(int argc, char *argv[], char **seconds, char **nanoseconds);
static void handle_arguments(const char *binary_name, const char *seconds_str, const char *nanoseconds_str, time_t *seconds, long *nanoseconds);
long long detect_time_t_size(void);
static time_t parse_time_t(const char *binary_name, const char *str);
static long parse_long(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);


int main(int argc, char *argv[])
{
    char *seconds_str;
    char *nanoseconds_str;
    time_t seconds;
    long nanoseconds;
    struct timespec tim;
    struct timespec rem;

    seconds_str = NULL;
    nanoseconds_str = NULL;
    parse_arguments(argc, argv, &seconds_str, &nanoseconds_str);
    handle_arguments(argv[0], seconds_str, nanoseconds_str, &seconds, &nanoseconds);
    tim.tv_sec = seconds;
    tim.tv_nsec = nanoseconds;
    printf("Starting program...\n");
    printf("Sleeping for %ld.%09ld seconds...\n", seconds, tim.tv_nsec);

    while(nanosleep(&tim, &rem) == -1)
    {
        if(errno == EINTR)
        {
            tim = rem; // If interrupted by a signal, assign the remaining time to tim
            continue;
        }
        else
        {
            perror("nanosleep");
            return EXIT_FAILURE;
        }
    }

    printf("Program woke up!\n");

    return EXIT_SUCCESS;
}


static void parse_arguments(int argc, char *argv[], char **seconds, char **nanoseconds)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hs:n:")) != -1)
    {
        switch(opt)
        {
            case 's':
            {
                *seconds = optarg;
                break;
            }
            case 'n':
            {
                *nanoseconds = optarg;
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

    if(optind != argc)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }
}


static void handle_arguments(const char *binary_name, const char *seconds_str, const char *nanoseconds_str, time_t *seconds, long *nanoseconds)
{
    if(seconds_str == NULL && nanoseconds_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "seconds or nanoseconds are required.");
    }

    if(seconds_str)
    {
        *seconds = parse_time_t(binary_name, seconds_str);
    }
    else
    {
        *seconds = 0;
    }

    if(nanoseconds_str)
    {
        *nanoseconds = parse_long(binary_name, nanoseconds_str);
    }
    else
    {
        *nanoseconds = 0;
    }
}


long long detect_time_t_size(void)
{
    switch (sizeof(time_t))
    {
        case 1: return 8;   // 8 bits
        case 2: return 16;  // 16 bits
        case 4: return 32;  // 32 bits
        case 8: return 64;  // 64 bits
        default: return -1; // Unknown size
    }
}


time_t parse_time_t(const char *binary_name, const char *str)
{
    long long parsed_value = parse_long(binary_name, str);

    long long time_t_min, time_t_max;
    long long time_t_bits = detect_time_t_size();

    if (time_t_bits == 8)
    {
        time_t_min = INT8_MIN;
        time_t_max = INT8_MAX;
    }
    else if (time_t_bits == 16)
    {
        time_t_min = INT16_MIN;
        time_t_max = INT16_MAX;
    }
    else if (time_t_bits == 32)
    {
        time_t_min = INT32_MIN;
        time_t_max = INT32_MAX;
    }
    else if (time_t_bits == 64)
    {
        time_t_min = INT64_MIN;
        time_t_max = INT64_MAX;
    }    else
    {
        fprintf(stderr, "Unknown size of time_t\n");
        exit(EXIT_FAILURE);
    }

    // Check if the parsed value is within the valid range for time_t
    if (parsed_value < time_t_min || parsed_value > time_t_max)
    {
        fprintf(stderr, "time_t value out of range: %lld\n", parsed_value);
        exit(EXIT_FAILURE);
    }

    return (time_t)parsed_value;
}


static long parse_long(const char *binary_name, const char *str)
{
    char *endptr;
    long parsed_value;

    errno = 0;
    parsed_value = strtol(str, &endptr, 10);

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

    fprintf(stderr, "Usage: %s [-h] [-s <seconds>] [-n nanoseconds]\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h                Display this help message\n", stderr);
    fputs("  -s <seconds>      \n", stderr);
    fputs("  -n <nanoseconds>  \n", stderr);
    exit(exit_code);
}
