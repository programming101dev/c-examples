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
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void           parse_arguments(int argc, char *argv[], char **seconds);
static void           handle_arguments(const char *binary_name, const char *seconds_str, unsigned int *seconds);
static unsigned int   parse_unsigned_int(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void           setup_signal_handler(void);
static void           sigint_handler(int signal_number);

#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10

int main(int argc, char *argv[])
{
    char        *seconds_str;
    unsigned int seconds;
    sigset_t     mask;
    int          sig;
    pid_t        pid;

    seconds_str = NULL;
    parse_arguments(argc, argv, &seconds_str);
    handle_arguments(argv[0], seconds_str, &seconds);
    setup_signal_handler();

    // Create a signal mask containing SIGINT
    sigemptyset(&mask);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    sigaddset(&mask, SIGINT);
#pragma GCC diagnostic pop

    // Block SIGINT in the main thread
    if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
    {
        perror("Failed to block SIGINT");
        return 1;
    }

    // Fork the process
    pid = fork();

    if(pid < 0)
    {
        perror("Fork failed");
        return EXIT_FAILURE;
    }

    if(pid == 0)
    {
        // Child process
        sleep(seconds);             // Sleep for 3 seconds
        kill(getppid(), SIGINT);    // Send SIGINT to the parent
    }
    else
    {
        // Parent process
        printf("Blocked SIGINT. Waiting for SIGINT using sigwait...\n");

        // Wait for SIGINT using sigwait
        if(sigwait(&mask, &sig) != 0)
        {
            perror("Failed to wait for signal");
            return EXIT_FAILURE;
        }

        // Print the received signal number
        printf("Received signal: %d\n", sig);

        // Wait for the child process to complete
        wait(NULL);
    }

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

static void setup_signal_handler(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = sigint_handler;
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void sigint_handler(int signal_number)
{
}

#pragma GCC diagnostic pop
