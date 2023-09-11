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
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **seconds);
static void handle_arguments(const char *binary_name, const char *seconds_str, unsigned int *seconds);
static unsigned int parse_unsigned_int(const char *binary_name, const char *str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void signal_handler(int signal_number);


int main(int argc, char *argv[])
{
    char *seconds_str;
    unsigned int seconds;
    sigset_t mask;
    int sig;
    struct sigaction sa;

    seconds_str = NULL;
    parse_arguments(argc, argv, &seconds_str);
    handle_arguments(argv[0], seconds_str, &seconds);

    // Set up signal handler for SIGINT (empty, not used in this example)
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Create a signal mask containing SIGINT
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    // Block SIGINT in the main thread
    if(sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
    {
        perror("Failed to block SIGINT");
        return 1;
    }

    // Fork the process
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("Fork failed");
        return EXIT_FAILURE;
    }
    else if(pid == 0)
    {
        // Child process
        sleep(seconds); // Sleep for 3 seconds
        kill(getppid(), SIGINT); // Send SIGINT to the parent
        exit(0);
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

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The group id is required");
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
    char *endptr;
    uintmax_t parsed_value;

    errno = 0;
    parsed_value = strtoumax(str, &endptr, 10);

    if (errno != 0)
    {
        usage(binary_name, EXIT_FAILURE, "Error parsing unsigned integer.");
    }

    // Check if there are any non-numeric characters in the input string
    if (*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for unsigned int
    if (parsed_value > UINT_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Unsigned integer out of range.");
    }

    // Now we will verify that the parsed_value fits within an unsigned int.
    if (parsed_value > (uintmax_t)UINT_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "Unsigned integer does not fit within an unsigned int.");
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


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void signal_handler(int signal_number)
{
}
#pragma GCC diagnostic pop
