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
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **seconds);
static void           handle_arguments(const char *binary_name, char *seconds_str, unsigned int *seconds);
static unsigned int   parse_unsigned_int(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void           print_process_info(const char *name);

#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10

int main(int argc, char *argv[])
{
    char        *seconds_str;
    unsigned int seconds;
    pid_t        pid;

    seconds_str = NULL;
    parse_arguments(argc, argv, &seconds_str);
    handle_arguments(argv[0], seconds_str, &seconds);
    pid = fork();

    if(pid == -1)
    {
        perror("Error creating child process");
        return EXIT_FAILURE;
    }

    if(pid == 0)
    {
        // This is the child process
        print_process_info("Child");
        printf("Child process finished.\n");
    }
    else
    {
        // This is the parent process
        print_process_info("Parent");
        sleep(seconds);
        printf("Parent process finished.\n");
    }

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **seconds)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "hs:")) != -1)
    {
        switch(opt)
        {
            case 's':
            {
                *seconds = optarg;
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

static void handle_arguments(const char *binary_name, char *seconds_str, unsigned int *seconds)
{
    if(seconds_str == NULL)
    {
        *seconds = 0;
    }
    else
    {
        *seconds = parse_unsigned_int(binary_name, seconds_str);
    }
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

static void print_process_info(const char *name)
{
    printf("Name: %s, Process: PID=%d, Parent PID=%d\n", name, getpid(), getppid());
}
