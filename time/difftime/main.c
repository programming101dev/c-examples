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
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **seconds);
static void           handle_arguments(const char *binary_name, const char *seconds_str, unsigned int *seconds);
static unsigned int   parse_unsigned_int(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);

#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10

int main(int argc, char *argv[])
{
    char        *seconds_str;
    unsigned int seconds;
    time_t       start_time;
    time_t       end_time;
    double       diff_time;

    seconds_str = NULL;
    parse_arguments(argc, argv, &seconds_str);
    handle_arguments(argv[0], seconds_str, &seconds);

    // Get the start time
    if(time(&start_time) == -1)
    {
        perror("Error getting start time");
        return EXIT_FAILURE;
    }

    // Simulate some time passing
    sleep(seconds);    // Sleep for 5 seconds

    // Get the end time
    if(time(&end_time) == -1)
    {
        perror("Error getting end time");
        return EXIT_FAILURE;
    }

    // Calculate the time difference
    if(end_time == (time_t)-1 || start_time == (time_t)-1)
    {
        perror("Invalid time values");
        return EXIT_FAILURE;
    }

    diff_time = difftime(end_time, start_time);
    printf("Time difference: %.2f seconds\n", diff_time);

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **seconds)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
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

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The seconds are required");
    }

    if(optind < argc - 1)
    {
        usage(argv[0], EXIT_FAILURE, "Too many arguments.");
    }

    *seconds = argv[optind];
}

static void handle_arguments(const char *binary_name, const char *seconds_str, unsigned int *seconds)
{
    if(seconds_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The seconds are required.");
    }

    *seconds = parse_unsigned_int(binary_name, seconds_str);
}

static unsigned int parse_unsigned_int(const char *binary_name, const char *str)
{
    char     *endptr;
    uintmax_t parsed_value;

    errno        = 0;
    parsed_value = strtoumax(str, &endptr, BASE_TEN);

    if(errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing unsigned integer.");
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for unsigned int
    if(parsed_value > UINT_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Unsigned integer out of range.");
    }

    return (unsigned int)parsed_value;
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <seconds>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}
