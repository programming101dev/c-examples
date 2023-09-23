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
#include <time.h>


static void parse_arguments(int argc, char *argv[], char **seconds, char **nanoseconds);
static void handle_arguments(const char *binary_name, const char *seconds_str, const char *nanoseconds_str, time_t *seconds, long *nanoseconds);
time_t get_time_t_min(void) __attribute__((const));
time_t get_time_t_max(void) __attribute__((const));
time_t parse_time_t(const char *binary_name, time_t min, time_t max, const char *str);
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
        time_t min;
        time_t max;

        min = get_time_t_min();
        max = get_time_t_max();
        *seconds = parse_time_t(binary_name, min, max, seconds_str);
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


time_t get_time_t_min(void)
{
    if(sizeof(time_t) == sizeof(char))
    {
        return CHAR_MIN;
    }
    else if(sizeof(time_t) == sizeof(short))
    {
        return SHRT_MIN;
    }
    else if(sizeof(time_t) == sizeof(int))
    {
        return INT_MIN;
    }
    else if(sizeof(time_t) == sizeof(long))
    {
        return LONG_MIN;
    }
    else if(sizeof(time_t) == sizeof(long long))
    {
        return LLONG_MIN;
    }
    else
    {
        // Handle other sizes or display an error message
        fprintf(stderr, "Unsupported size of time_t\n");
        exit(EXIT_FAILURE);
    }
}


time_t get_time_t_max(void)
{
    if (sizeof(time_t) == sizeof(char))
    {
        return CHAR_MAX;
    }
    else if (sizeof(time_t) == sizeof(short))
    {
        return SHRT_MAX;
    }
    else if (sizeof(time_t) == sizeof(int))
    {
        return INT_MAX;
    }
    else if (sizeof(time_t) == sizeof(long))
    {
        return LONG_MAX;
    }
    else if (sizeof(time_t) == sizeof(long long))
    {
        return LLONG_MAX;
    }
    else
    {
        fprintf(stderr, "Unsupported size of time_t\n");
        exit(EXIT_FAILURE);
    }
}


time_t parse_time_t(const char *binary_name, time_t min, time_t max, const char *str)
{
    char *endptr;
    intmax_t parsed_value;

    errno = 0;
    parsed_value = strtoimax(str, &endptr, 10);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing time.");
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    if(parsed_value < min || parsed_value > max)
    {
        usage(binary_name, EXIT_FAILURE, "Unsigned integer out of range for time.");
    }

    return (time_t)parsed_value;
}


static long parse_long(const char *binary_name, const char *str)
{
    char *endptr;
    intmax_t parsed_value;

    errno = 0;
    parsed_value = strtoimax(str, &endptr, 10);

    if (errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing unsigned integer.");
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for signed long
    if (parsed_value < LONG_MIN || parsed_value > LONG_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Unsigned integer out of range for signed long.");
    }

    return (long)parsed_value;
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
