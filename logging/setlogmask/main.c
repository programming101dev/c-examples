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


#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>


static void parse_arguments(int argc, char *argv[], char **log_name);
static void handle_arguments(const char *binary_name, const char *log_name);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
void logMessages(void);


int main(int argc, char *argv[])
{
    char *log_name;
    int mask;

    log_name = NULL;
    parse_arguments(argc, argv, &log_name);
    handle_arguments(argv[0], log_name);
    openlog(log_name, LOG_PID, LOG_USER);
    mask = LOG_MASK(LOG_EMERG) | LOG_MASK(LOG_ALERT) | LOG_MASK(LOG_CRIT) |
           LOG_MASK(LOG_ERR) | LOG_MASK(LOG_WARNING) | LOG_MASK(LOG_NOTICE) |
           LOG_MASK(LOG_INFO) | LOG_MASK(LOG_DEBUG);

    if(setlogmask(mask) == -1)
    {
        syslog(LOG_ERR, "Failed to set log mask");
        closelog();
        return EXIT_FAILURE;
    }

    printf("Logging PID %d\n", getpid());
    logMessages();
    mask = LOG_MASK(LOG_EMERG);

    if(setlogmask(mask) == -1)
    {
        syslog(LOG_ERR, "Failed to set log mask");
        closelog();
        return EXIT_FAILURE;
    }

    logMessages();
    closelog();

    return EXIT_SUCCESS;
}



static void parse_arguments(int argc, char *argv[], char **log_name)
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

    *log_name = argv[optind];
}


static void handle_arguments(const char *binary_name, const char *log_name)
{
    if(log_name == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The log name is required.");
    }
}


_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <log name>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}


void logMessages(void)
{
    syslog(LOG_EMERG, "This is an emergency message.");
    syslog(LOG_ALERT, "This is an alert message.");
    syslog(LOG_CRIT, "This is a critical message.");
    syslog(LOG_ERR, "This is an error message.");
    syslog(LOG_WARNING, "This is a warning message.");
    syslog(LOG_NOTICE, "This is a notice message.");
    syslog(LOG_INFO, "This is an informational message.");
    syslog(LOG_DEBUG, "This is a debug message.");
}